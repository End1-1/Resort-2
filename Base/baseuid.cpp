#include "baseuid.h"
#include <QDebug>
#include <QHostInfo>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
//#include <QRandomGenerator>

#define SRC QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define len 8
#define low 0
#define high 35
#define maxtries 200

int BaseUID::fUserId = 0;
BaseUID::BaseUID()
{

}

QString BaseUID::genID(const QString &prefix, Database &db)
{
    return INTID(prefix, db);
}

QString BaseUID::INTID(const QString &prefix, Database &db)
{
    int totaltrynum = 0;
    if (!db.fDb.isOpen()) {
        db.open();
    }
    db.fDb.transaction();
    bool done = false;
    QString result;
    do {
        QSqlQuery q(db.fDb);
        QString query = QString ("select f_max, f_zero from serv_id_counter where f_id='%1' for update").arg(prefix);
        if (!q.exec(query)) {
            QMessageBox::critical(0, "ID ERROR", "<H1><font color=\"red\">COUNTER ID GENERATOR FAIL</font></h1><br>" + q.lastError().databaseText());
            exit(0);
        }
        if (q.next()) {
            int max = q.value(0).toInt() + 1;
            int zero = q.value(1).toInt();
            query = "update serv_id_counter set f_max=:f_max where f_id=:f_id";
            q.prepare(query);
            q.bindValue(":f_max", max);
            q.bindValue(":f_id", prefix);
            q.exec();
            result = QString("%1").arg(max, zero, 10, QChar('0'));
        } else {
            query = "insert into serv_id_counter (f_id, f_max, f_zero) values (" + ap(prefix.toUpper()) + ", 0, 6)";
            q.exec(query);
            totaltrynum++;
            continue;
        }
        q.exec(QString("insert into airwick.f_id (f_value, f_try, f_comp, f_user, f_date, f_time, f_db) values ('%1-%2', %3, '%4', '%5', '%6', '%7', database())")
                                             .arg(prefix.toUpper())
                                             .arg(result).arg(totaltrynum)
                                             .arg(QHostInfo::localHostName().toUpper())
                                             .arg(fUserId)
                                             .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                                             .arg(QTime::currentTime().toString("HH:mm:ss")));
        if (q.lastError().databaseText().toLower().contains("duplicate entry")) {
            totaltrynum++;
        } else {
            done = true;
        }
        if (totaltrynum > 20) {
            QMessageBox::critical(0, "ID ERROR", "<H1><font color=\"red\">COUNTER ID GENERATOR FAIL, GIVE UP</font></h1>");
            exit(0);
        }
    } while (!done);
    db.fDb.commit();
    return prefix + "-" + result;
}
