#include "doubledatabase.h"
#include <QMutexLocker>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDate>
#include <QDebug>

int DoubleDatabase::fCounter = 0;
QMutex fMutex;
QString __dd1Host;
QString __dd1Database;
QString __dd1Username;
QString __dd1Password;

QString __dd2Host;
QString __dd2Database;
QString __dd2Username;
QString __dd2Password;

DoubleDatabase::DoubleDatabase()
{
    QMutexLocker ml(&fMutex);
    ++fCounter;
    fDbName1 = QString("DB1-%1").arg(fCounter);
    fDbName2 = QString("DB2-%1").arg(fCounter);
    fDb1 = QSqlDatabase::addDatabase(_DBDRIVER_, fDbName1);
    fDb2 = QSqlDatabase::addDatabase(_DBDRIVER_, fDbName2);

    fDb1.setHostName(__dd1Host);
    fDb1.setDatabaseName(__dd1Database);
    fDb1.setUserName(__dd1Username);
    fDb1.setPassword(__dd1Password);

    fDb2.setHostName(__dd2Host);
    fDb2.setDatabaseName(__dd2Database);
    fDb2.setUserName(__dd2Username);
    fDb2.setPassword(__dd2Password);
}

DoubleDatabase::~DoubleDatabase()
{
    fDb1 = QSqlDatabase::addDatabase(_DBDRIVER_);
    QSqlDatabase::removeDatabase(fDbName1);
    fDb2 = QSqlDatabase::addDatabase(_DBDRIVER_);
    QSqlDatabase::removeDatabase(fDbName2);
}

QVariant &DoubleDatabase::operator[](const QString &name)
{
    return fBindValues[name];
}

void DoubleDatabase::setDatabase(const QString &host, const QString &db, const QString &user, const QString &password, int dbNum)
{
    switch (dbNum) {
    case 1:
        configureDatabase(fDb1, host, db, user, password);
        break;
    case 2:
        configureDatabase(fDb2, host, db, user, password);
        break;
    default:
        break;
    }
}

bool DoubleDatabase::open(bool db1, bool db2)
{
    bool isOpened = true;
    if (db1) {
        if (!fDb1.isOpen()) {
            if (fDb1.open()) {
                fDb1.transaction();
            } else {
                isOpened = false;
                fLastError = "#1: " + fDb1.lastError().databaseText();
                logEvent(fLastError);
            }
        }
    }
    if (db2) {
        if (!fDb2.isOpen()) {
            if (fDb2.open()) {
                fDb2.transaction();
            } else {
                isOpened = false;
                fLastError += "#2 " + fDb2.lastError().databaseText();
                logEvent(fLastError);
            }
        }
    }
    fExecFlagMaster = db1;
    fExecFlagSlave = db2;
    return isOpened;
}

void DoubleDatabase::close(bool commit)
{
    if (commit) {
        fDb1.commit();
        fDb2.commit();
    } else {
        fDb1.rollback();
        fDb2.rollback();
    }
    fDb1.close();
    fDb2.close();
}

bool DoubleDatabase::exec(const QString &sqlQuery)
{
    QSqlQuery *q1 = new QSqlQuery(fDb1);
    bool isSelect;
    bool result = true;
    if (fExecFlagMaster) {
        if (!exec(q1, sqlQuery, isSelect)) {
            delete q1;
            return false;
        }
    }
    if (fExecFlagSlave) {
        QSqlQuery *q2 = new QSqlQuery(fDb2);
        result = exec(q2, sqlQuery, isSelect);
        delete q2;
    }
    if (isSelect) {
        int colCount = q1->record().count();
        fDefColNames.clear();
        for (int i = 0; i < colCount; i++) {
            fDefColNames[i] = q1->record().fieldName(i);
        }
        fDbRows.clear();
        while (q1->next()) {
            QList<QVariant> row;
            for (int i = 0; i < colCount; i++) {
                row << q1->value(i);
            }
            fDbRows << row;
        }
    }
    return result;
}

int DoubleDatabase::rowCount()
{
    return fDbRows.count();
}

void DoubleDatabase::configureDatabase(QSqlDatabase &cn, const QString &host, const QString &db, const QString &user, const QString &password)
{
    cn.setHostName(host);
    cn.setDatabaseName(db);
    cn.setUserName(user);
    cn.setPassword(password);
}

void DoubleDatabase::logEvent(const QString &event)
{
    qDebug() << event;
}

QString DoubleDatabase::lastQuery(QSqlQuery *q)
{
    QString sql = q->lastQuery();
    QMapIterator<QString, QVariant> it(q->boundValues());
    while (it.hasNext()) {
        it.next();
        QVariant value = it.value();
        switch (it.value().type()) {
        case QVariant::String:
            value = QString("'%1'").arg(value.toString().replace("'", "''"));
            break;
        case QVariant::Date:
            value = QString("'%1'").arg(value.toDate().toString("yyyy-MM-dd"));
            break;
        case QVariant::DateTime:
            value = QString("'%1'").arg(value.toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            break;
        case QVariant::Double:
            value = QString("%1").arg(value.toDouble());
            break;
        case QVariant::Int:
            value = QString("%1").arg(value.toInt());
            break;
        default:
            break;
        }
        sql.replace(it.key(), value.toString());
    }
    return sql;
}

bool DoubleDatabase::exec(QSqlQuery *q, const QString &sqlQuery, bool &isSelect)
{
    if (!q->prepare(sqlQuery)) {
        fLastError = q->lastError().databaseText();
        logEvent(fLastError);
        logEvent(sqlQuery);
        return false;
    }
    for (QMap<QString, QVariant>::const_iterator it = fBindValues.begin(); it != fBindValues.end(); it++) {
        q->bindValue(it.key(), it.value());
    }
    if (!q->exec()) {
        fLastError = q->lastError().databaseText();
        logEvent(fLastError);
        logEvent(lastQuery(q));
        return false;
    }
    if (q->isSelect()) {
        isSelect = true;
    }
#ifdef QT_DEBUG
    logEvent(lastQuery(q));
#endif
    return true;
}
