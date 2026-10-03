#include "resortlog.h"

#include "base.h"
#include "preferences.h"
#include "trackcontrol.h"

#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlRecord>

namespace {

class ResortLogBase : public Base
{
};

QJsonObject queryRowObject(QSqlDatabase &sqlDb, const QString &sql, const QMap<QString, QVariant> &bind)
{
    QJsonObject o;
    QSqlQuery q(sqlDb);
    if(!q.prepare(sql)) {
        return o;
    }
    for(QMap<QString, QVariant>::const_iterator it = bind.constBegin(); it != bind.constEnd(); ++it) {
        q.bindValue(it.key(), it.value());
    }
    if(!q.exec() || !q.next()) {
        return o;
    }
    const QSqlRecord rec = q.record();
    for(int i = 0; i < rec.count(); i++) {
        o[rec.fieldName(i)] = QJsonValue::fromVariant(rec.value(i));
    }
    return o;
}

} // namespace

void ResortLog::insertRow(const QString &entityType, const QString &entityId, const QString &action,
                          const QString &field, const QString &valueOld, const QString &valueNew,
                          const QString &snapshot)
{
    ResortLogBase base;
    if(!base.fDb.fDb.isOpen()) {
        return;
    }

    QMap<QString, QVariant> bind;
    bind[":f_datetime"] = QDateTime::currentDateTime();
    bind[":f_user_id"] = Base::fPreferences.getLocal(def_working_user_id).toInt();
    bind[":f_user"] = Base::fPreferences.getLocal(def_working_username).toString();
    bind[":f_host"] = QHostInfo::localHostName();
    bind[":f_entity_type"] = entityType;
    bind[":f_entity_id"] = entityId;
    bind[":f_action"] = action;
    bind[":f_field"] = field;
    bind[":f_value_old"] = valueOld;
    bind[":f_value_new"] = valueNew;
    bind[":f_snapshot"] = snapshot;
    base.fDb.insert("r_resort_log", bind);
}

void ResortLog::logFieldChange(const QString &entityType, const QString &entityId,
                               const QString &field, const QString &oldValue, const QString &newValue)
{
    insertRow(entityType, entityId, QStringLiteral("field_change"), field, oldValue, newValue, QString());
}

void ResortLog::logSnapshot(const QString &entityType, const QString &entityId, const QString &action,
                            const QString &snapshotBefore, const QString &snapshotAfter)
{
    insertRow(entityType, entityId, action, QString(), snapshotBefore, snapshotAfter, snapshotBefore);
}

void ResortLog::logJobEvent(const QString &runId, const QString &action, const QJsonObject &data)
{
    if(runId.isEmpty()) {
        return;
    }
    const QString payload = QString::fromUtf8(QJsonDocument(data).toJson(QJsonDocument::Compact));
    insertRow(QStringLiteral("store_recalc"), runId, action, QString(), QString(), payload, payload);
}

void ResortLog::logTrackField(int trackType, const QString &recordId,
                              const QString &field, const QString &oldValue, const QString &newValue)
{
    logFieldChange(entityTypeFromTrack(trackType), recordId, field, oldValue, newValue);
}

QString ResortLog::entityTypeFromTrack(int trackType)
{
    switch(trackType) {
    case TRACK_DISH: return QStringLiteral("dish");
    case TRACK_DISH_COMPLEX: return QStringLiteral("dish_complex");
    case TRACK_DISH_MOD: return QStringLiteral("dish_mod");
    case TRACK_DISH_TYPE: return QStringLiteral("dish_type");
    case TRACK_MENU_NAME: return QStringLiteral("menu_name");
    case TRACK_MENU_PART: return QStringLiteral("menu_part");
    case TRACK_REST_STORE: return QStringLiteral("store");
    case TRACK_REST_HALL: return QStringLiteral("hall");
    case TRACK_REST_TABLE: return QStringLiteral("table");
    case TRACK_REST_PRINTERS: return QStringLiteral("printer");
    case TRACK_BRANCH: return QStringLiteral("branch");
    case TRACK_STORE_PARTNERS: return QStringLiteral("store_partner");
    default:
        return QStringLiteral("track_%1").arg(trackType);
    }
}

QString ResortLog::dishSnapshotJson(Database &db, int dishId)
{
    if(dishId <= 0) {
        return QString();
    }

    QJsonObject root;
    QMap<QString, QVariant> bind;
    bind[":f_id"] = dishId;

    root[QStringLiteral("dish")] = queryRowObject(db.fDb,
                                                QStringLiteral("select * from r_dish where f_id=:f_id"),
                                                bind);

    QList<QList<QVariant> > rows;

    db.select("select m.f_menu, mn.f_en, m.f_price, m.f_print1, m.f_state "
              "from r_menu m "
              "inner join r_menu_names mn on mn.f_id=m.f_menu "
              "where m.f_dish=:f_id order by m.f_menu", bind, rows);
    QJsonArray menu;
    for(QList<QList<QVariant> >::const_iterator it = rows.constBegin(); it != rows.constEnd(); ++it) {
        QJsonObject line;
        line[QStringLiteral("menu")] = QJsonValue::fromVariant(it->at(0));
        line[QStringLiteral("menu_name")] = it->at(1).toString();
        line[QStringLiteral("price")] = QJsonValue::fromVariant(it->at(2));
        line[QStringLiteral("print1")] = it->at(3).toString();
        line[QStringLiteral("enabled")] = it->at(4).toInt();
        menu.append(line);
    }
    root[QStringLiteral("menu")] = menu;

    db.select("select r.f_part, d.f_en, r.f_qty "
              "from r_recipe r "
              "left join r_dish d on d.f_id=r.f_part "
              "where r.f_dish=:f_id order by r.f_part", bind, rows);
    QJsonArray recipe;
    for(QList<QList<QVariant> >::const_iterator it = rows.constBegin(); it != rows.constEnd(); ++it) {
        QJsonObject line;
        line[QStringLiteral("part_id")] = it->at(0).toString();
        line[QStringLiteral("part_name")] = it->at(1).toString();
        line[QStringLiteral("qty")] = it->at(2).toString();
        recipe.append(line);
    }
    root[QStringLiteral("recipe")] = recipe;

    db.select("select m.f_mod, d.f_en "
              "from r_dish_mod_required m "
              "inner join r_dish_mod d on d.f_id=m.f_mod "
              "where m.f_dish=:f_id order by m.f_mod", bind, rows);
    QJsonArray mods;
    for(QList<QList<QVariant> >::const_iterator it = rows.constBegin(); it != rows.constEnd(); ++it) {
        QJsonObject line;
        line[QStringLiteral("mod_id")] = it->at(0).toString();
        line[QStringLiteral("mod_name")] = it->at(1).toString();
        mods.append(line);
    }
    root[QStringLiteral("modifiers")] = mods;

    db.select("select f_code from r_dish_scancode where f_dish=:f_id order by f_code", bind, rows);
    QJsonArray scans;
    for(QList<QList<QVariant> >::const_iterator it = rows.constBegin(); it != rows.constEnd(); ++it) {
        scans.append(it->at(0).toString());
    }
    root[QStringLiteral("scancodes")] = scans;

    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

QString ResortLog::storeDocSnapshotJson(Database &db, int docId)
{
    if(docId <= 0) {
        return QString();
    }

    QJsonObject root;
    QMap<QString, QVariant> bind;
    bind[":f_id"] = docId;

    QList<QList<QVariant> > rows;
    db.select("select f_id, f_date, f_type, f_state, f_partner, f_inv, f_invDate, f_amount, f_remarks, f_op, f_payment "
              "from r_docs where f_id=:f_id", bind, rows);
    if(!rows.isEmpty()) {
        const QList<QVariant> &r = rows.first();
        QJsonObject header;
        header[QStringLiteral("f_id")] = r.at(0).toString();
        header[QStringLiteral("f_date")] = r.at(1).toString();
        header[QStringLiteral("f_type")] = r.at(2).toString();
        header[QStringLiteral("f_state")] = r.at(3).toString();
        header[QStringLiteral("f_partner")] = r.at(4).toString();
        header[QStringLiteral("f_inv")] = r.at(5).toString();
        header[QStringLiteral("f_invDate")] = r.at(6).toString();
        header[QStringLiteral("f_amount")] = r.at(7).toString();
        header[QStringLiteral("f_remarks")] = r.at(8).toString();
        header[QStringLiteral("f_op")] = r.at(9).toString();
        header[QStringLiteral("f_payment")] = r.at(10).toString();
        root[QStringLiteral("header")] = header;
    }

    bind[":f_doc"] = docId;
    db.select("select f_id, f_store, f_material, f_sign, f_qty, f_price, f_total, f_vat "
              "from r_body where f_doc=:f_doc order by f_id", bind, rows);
    QJsonArray goods;
    for(QList<QList<QVariant> >::const_iterator it = rows.constBegin(); it != rows.constEnd(); ++it) {
        QJsonObject line;
        line[QStringLiteral("f_id")] = it->at(0).toString();
        line[QStringLiteral("f_store")] = it->at(1).toString();
        line[QStringLiteral("f_material")] = it->at(2).toString();
        line[QStringLiteral("f_sign")] = it->at(3).toString();
        line[QStringLiteral("f_qty")] = it->at(4).toString();
        line[QStringLiteral("f_price")] = it->at(5).toString();
        line[QStringLiteral("f_total")] = it->at(6).toString();
        line[QStringLiteral("f_vat")] = it->at(7).toString();
        goods.append(line);
    }
    root[QStringLiteral("goods")] = goods;

    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

QString ResortLog::giftCardSnapshotJson(Database &db, int cardId)
{
    if(cardId <= 0) {
        return QString();
    }

    QJsonObject root;
    QMap<QString, QVariant> bind;
    bind[":f_id"] = cardId;

    const QJsonObject card = queryRowObject(db.fDb,
                                            QStringLiteral("select * from d_gift_cart where f_id=:f_id"),
                                            bind);
    root[QStringLiteral("card")] = card;

    const QString code = card.value(QStringLiteral("f_code")).toString();
    if(!code.isEmpty()) {
        bind.clear();
        bind[":f_code"] = code;
        QList<QList<QVariant> > rows;
        db.select("select f_id, f_order, f_amount from d_gift_cart_use where f_code=:f_code order by f_id",
                  bind, rows);
        QJsonArray use;
        for(QList<QList<QVariant> >::const_iterator it = rows.constBegin(); it != rows.constEnd(); ++it) {
            QJsonObject line;
            line[QStringLiteral("f_id")] = it->at(0).toString();
            line[QStringLiteral("f_order")] = it->at(1).toString();
            line[QStringLiteral("f_amount")] = QJsonValue::fromVariant(it->at(2));
            use.append(line);
        }
        root[QStringLiteral("use")] = use;
    }

    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}
