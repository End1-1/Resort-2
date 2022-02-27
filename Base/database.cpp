#include "database.h"
#include "defines.h"
#include <QSqlError>
#include <QDateTime>
#include <QSqlRecord>
#include <QDebug>
#include <QFile>

int Database::fDbNum = 0;

Database::Database(QObject *parent) : QObject(parent)
{
    fDbId = QString::number(fDbNum++);
    fDb = QSqlDatabase::addDatabase("QMYSQL", fDbId);
}

Database::~Database()
{
    fDb = QSqlDatabase::addDatabase("QMYSQL");
    QSqlDatabase::removeDatabase(fDbId);
}

void Database::setConnectionParams(const QString &host, const QString &database, const QString &user, const QString &password)
{
    bool opened = false;
    if (fDb.isOpen()) {
        fDb.close();
        opened = true;
    }
    fDb.setHostName(host);
    fDb.setDatabaseName(database);
    fDb.setUserName(user);
    fDb.setPassword(password);
    if (opened) {
        fDb.open();
    }
}

void Database::copyConnectionParamsFrom(Database &db)
{
    fDb.setHostName(db.fDb.hostName());
    fDb.setDatabaseName(db.fDb.databaseName());
    fDb.setUserName(db.fDb.userName());
    fDb.setPassword(db.fDb.password());
}

bool Database::open()
{
    if (!fDb.open()) {
        logError(fDb);
        return false;
    }
    return true;
}

void Database::close()
{
    fDb.close();
}

int Database::query(const QString &sql, QMap<QString, QVariant> &bindValues)
{
    QSqlQuery q(fDb);
    if (!q.prepare(sql)) {
        logError(q);
        bindValues.clear();
        return -1;
    }
    for (QMap<QString, QVariant>::const_iterator it = bindValues.begin(); it != bindValues.end(); it++)
        q.bindValue(it.key(), it.value());
    bindValues.clear();
    if (!q.exec()) {
        logError(q);
        return -1;
    }
    logQuery(q);
    int result = 0;
    if (sql.contains("insert", Qt::CaseInsensitive)) {
        q.exec("select last_insert_id()");
        q.next();
        result = q.value(0).toInt();
        logQuery(q);
    }
    return result;
}

bool Database::insertId(const QString &table, const QString &id)
{
    QSqlQuery *q = select(QString("insert into %1 (f_id) values ('%2')").arg(table).arg(id));
    bool result = q;
    delete q;
    return result;
}

int Database::insert(const QString &table, QMap<QString, QVariant> &values)
{
    QString sql = "insert into %1 (%2) values (%3)";
    QString fields = "";
    QString bind = "";
    bool first = true;
    for (QMap<QString, QVariant>::const_iterator it = values.begin(); it != values.end(); it++) {
        if (first) {
            first = false;
        } else {
            fields += ",";
            bind += ",";
        }
        fields += QString(it.key()).remove(0, 1);
        bind += it.key();
    }
    sql = sql.arg(table).arg(fields).arg(bind);
    return query(sql, values);
}

bool Database::insertWithoutId(const QString &table, QMap<QString, QVariant> &values)
{
    QString sql = "insert into %1 (%2) values (%3)";
    QString fields = "";
    QString bind = "";
    bool first = true;
    for (QMap<QString, QVariant>::const_iterator it = values.begin(); it != values.end(); it++) {
        if (first) {
            first = false;
        } else {
            fields += ",";
            bind += ",";
        }
        fields += QString(it.key()).remove(0, 1);
        bind += it.key();
    }
    sql = sql.arg(table).arg(fields).arg(bind);

    QSqlQuery q(fDb);
    if (!q.prepare(sql)) {
        logError(q);
        values.clear();
        return false;
    }
    for (QMap<QString, QVariant>::const_iterator it = values.begin(); it != values.end(); it++)
        q.bindValue(it.key(), it.value());
    values.clear();
    if (!q.exec()) {
        logError(q);
        return false;
    }
    logQuery(q);
    return true;
}

bool Database::queryDirect(const QString &query)
{
    QSqlQuery q(fDb);
    if (!q.exec(query)) {
        logError(q);
        return false;
    }
    logQuery(q);
    return true;
}

bool Database::update(const QString &table, QMap<QString, QVariant> &values, const QString &where)
{
    QString sql = "update " + table + " set ";
    bool first = true;
    for (QMap<QString, QVariant>::const_iterator it = values.begin(); it != values.end(); it++) {
        if (first) {
            first = false;
        } else {
            sql += ",";
        }
        sql += QString(it.key()).remove(0, 1) + "=" + it.key();
    }
    sql += " " + where;
    return query(sql, values) != -1;
}

int Database::select(const QString &sql, QMap<QString, QVariant> &values, QList<QList<QVariant> > &out)
{
    out.clear();
    QSqlQuery q(fDb);
    if (!q.prepare(sql)) {
        logError(q);
        values.clear();
        return -1;
    }
    for (QMap<QString, QVariant>::const_iterator it = values.begin(); it != values.end(); it++) {
        q.bindValue(it.key(), it.value());
    }
    values.clear();
    if (!q.exec()) {
        logError(q);
        return -1;
    }
    int cols = q.record().count();
    while (q.next()) {
        QList<QVariant> row;
        for (int i = 0; i < cols; i++)
            row << q.value(i);
        out.append(row);
    }
    logQuery(q);
    return out.count();
}

QSqlQuery *Database::select(QSqlQuery *q, const QString &sql, QMap<QString, QVariant> &values)
{
    if (!q)
        q = new QSqlQuery(fDb);
    if (!q->prepare(sql)) {
        logError(*q);
        values.clear();
        delete q;
        q = 0;
        return 0;
    }
    for (QMap<QString, QVariant>::const_iterator it = values.begin(); it != values.end(); it++) {
        q->bindValue(it.key(), it.value());
    }
    values.clear();
    if (!q->exec()) {
        logError(*q);
        delete q;
        q = 0;
        return 0;
    }
    logQuery(*q);
    return q;
}

bool Database::deleteEntry(const QString &table, const QVariant &id)
{
    QString sql = "delete from " + table + " where f_id=:f_id";
    QMap<QString, QVariant> v;
    v[":f_id"] = id;
    QList<QList<QVariant> > o;
    return select(sql, v, o) != -1;
}

QSqlQuery *Database::select(const QString &query)
{
    QSqlQuery *q = new QSqlQuery(fDb);
    if (q->exec(query)) {
        logQuery(*q);
        return q;
    } else {
        logError(*q);
        delete q;
        return 0;
    }
}


void Database::logError(QSqlQuery &query)
{
    fLastError = query.lastError().databaseText();
#ifdef QT_DEBUG
    qDebug()
            << "Sql error"
            << query.lastError().databaseText()
            << "SQL query"
            << lastQuery(query);
#endif
    QFile  f("log.txt");
    f.open(QIODevice::Append);
    f.write(lastQuery(query).toUtf8());
    f.write("\r\n");
    f.write(query.lastError().databaseText().toUtf8());
    f.write("\r\n");
    f.close();
}

void Database::logError(QSqlDatabase &db)
{
    fLastError = db.lastError().databaseText();
#ifdef QT_DEBUG
    qDebug()
            << "Database error"
            << db.lastError().databaseText();
#endif
}

void Database::logQuery(QSqlQuery &query)
{
#ifdef QT_DEBUG
    qDebug() << lastQuery(query);
    QFile  f("log.txt");
    f.open(QIODevice::WriteOnly);
    f.write(lastQuery(query).toUtf8());
    f.write("\r\n");
    f.close();
#else
    Q_UNUSED(query)
#endif
}

Db Database::db()
{
    Db d;
    memset(&d, 0, sizeof(d));
    Preferences::qstringToChar(d.dc_main_host, fDb.hostName());
    Preferences::qstringToChar(d.dc_main_path, fDb.databaseName());
    Preferences::qstringToChar(d.dc_main_user, fDb.userName());
    Preferences::qstringToChar(d.dc_main_pass, fDb.password());
    return d;
}

QString Database::lastQuery(QSqlQuery &q)
{
    QString sql = q.lastQuery();
    QMapIterator<QString, QVariant> it(q.boundValues());
    while (it.hasNext()) {
        it.next();
        QVariant value = it.value();
        switch (it.value().type()) {
        case QVariant::String:
            value = QString("'%1'").arg(value.toString().replace("'", "''"));
            break;
        case QVariant::Date:
            value = QString("'%1'").arg(value.toDate().toString(def_mysql_date_format));
            break;
        case QVariant::DateTime:
            value = QString("'%1'").arg(value.toDateTime().toString(def_mysql_datetime_format));
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
        sql.replace(QRegExp(it.key() + "\\b"), value.toString());
    }
    return sql;
}
