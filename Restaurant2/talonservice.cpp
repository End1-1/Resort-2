#include "talonservice.h"
#include "base.h"
#include "database2.h"
#include "defines.h"
#include "orderlog.h"
#include "preferences.h"
#include <QDate>
#include <QObject>

static bool openDb(Database2 &db2)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    return db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
}

QString TalonService::normalizeCode(const QString &raw)
{
    QString code = raw.trimmed();
    code.remove('?');
    code.remove(';');

    static const QStringList prefixes = {
        "tel:",
        "sms:",
        "mailto:",
        "http://",
        "https://",
        "ftp://"
    };

    for(;;) {
        bool changed = false;

        for(const QString &prefix : prefixes) {
            if(code.startsWith(prefix, Qt::CaseInsensitive)) {
                code = code.mid(prefix.length());
                changed = true;
            }
        }

        if(!changed) {
            break;
        }
    }

    return code.trimmed();
}

bool TalonService::talonExists(const QString &normalizedCode)
{
    if(normalizedCode.isEmpty()) {
        return false;
    }

    Database2 db2;

    if(!openDb(db2)) {
        return false;
    }

    db2[":f_code"] = normalizedCode;
    db2.exec("select f_id from talon_service where f_code=:f_code limit 1");
    return db2.next();
}

bool TalonService::redeemForOrderInTx(Database2 &db2,
                                      int orderId,
                                      const QString &normalizedCode,
                                      TalonRedeemInfo &info,
                                      QString &error)
{
    if(orderId <= 0) {
        error = QObject::tr("Invalid order");
        return false;
    }

    if(normalizedCode.isEmpty()) {
        error = QObject::tr("Talon code is empty");
        return false;
    }

    db2[":f_code"] = normalizedCode;
    db2.exec("select t.*, p.f_name as f_partnername "
             "from talon_service t "
             "left join r_partners p on p.f_id=t.f_partner "
             "where t.f_code=:f_code");

    if(!db2.next()) {
        error = QObject::tr("Invalid code");
        return false;
    }

    if(db2.integer("f_trsale") == 0) {
        error = QObject::tr("This coupon not sold");
        return false;
    }

    if(db2.integer("f_used") != 0) {
        error = QObject::tr("Talon used");
        return false;
    }

    const QDate validTo = db2.date("f_validto");

    if(validTo.isValid()) {
        const QDate workingDate = Base::fPreferences.getLocalDate(def_working_day);

        if(workingDate > validTo) {
            error = QObject::tr("Talon expired");
            return false;
        }
    }

    info.code = normalizedCode;
    info.partnerId = db2.integer("f_partner");
    info.partnerName = db2.string("f_partnername");
    info.price = db2.doubleValue("f_price");

    db2[":f_costumer"] = info.partnerId;
    db2[":f_order"] = orderId;

    if(!db2.exec("update o_car set f_costumer=:f_costumer where f_order=:f_order")) {
        error = db2.lastDbError();
        return false;
    }

    db2[":f_couponservice"] = 1;
    db2[":f_id"] = orderId;

    if(!db2.exec("update o_header set f_couponservice=:f_couponservice where f_id=:f_id")) {
        error = db2.lastDbError();
        return false;
    }

    db2[":f_used"] = 1;
    db2[":f_code"] = normalizedCode;
    db2[":f_order"] = orderId;

    if(!db2.exec("update talon_service set f_used=:f_used, f_order=:f_order where f_code=:f_code")) {
        error = db2.lastDbError();
        return false;
    }

    return true;
}

bool TalonService::redeemForOrder(int orderId,
                                  const QString &rawCode,
                                  TalonRedeemInfo &info,
                                  QString &error)
{
    const QString code = normalizeCode(rawCode);

    Database2 db2;

    if(!openDb(db2)) {
        error = QObject::tr("Database error");
        return false;
    }

    if(!db2.startTransaction()) {
        error = db2.lastDbError();
        return false;
    }

    if(!redeemForOrderInTx(db2, orderId, code, info, error)) {
        db2.rollback();
        return false;
    }

    if(!db2.commit()) {
        db2.rollback();
        error = db2.lastDbError();
        return false;
    }

    OrderLog::write(orderId, OrderLog::ACTION_DISCOUNT_OK,
                    QString("code=%1;result=ok;reason=talon_service;amount=%2")
                    .arg(code)
                    .arg(info.price));
    return true;
}
