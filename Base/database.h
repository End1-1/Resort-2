#ifndef DATABASE_H
#define DATABASE_H

#include "preferences.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableWidget>

#define ap(x) QString("'%1'").arg(x)
#define where_id(id) QString("where f_id=%1").arg(id)
#define where_field(field, value) QString("where %1=%2").arg(field).arg(value)
#define foreach_rows for(QList<QList<QVariant> >::const_iterator it = fDbRows.constBegin(); it != fDbRows.constEnd(); it++)

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    ~Database();
    QSqlDatabase fDb;
    void setConnectionParams(const QString &host, const QString &database, const QString &user, const QString &password);
    void copyConnectionParamsFrom(Database &db);
    bool open();
    void close();
    int query(const QString &sql, QMap<QString, QVariant>& bindValues);
    bool insertId(const QString &table, const QString &id);
    int insert(const QString &table, QMap<QString, QVariant>& values);
    bool insertWithoutId(const QString &table, QMap<QString, QVariant>& values);
    bool queryDirect(const QString &query);
    bool update(const QString &table, QMap<QString, QVariant>& values, const QString &where);
    int select(const QString &sql, QTableWidget *tableWidget);
    int select(const QString &sql, QMap<QString, QVariant>& values, QList<QList<QVariant> >& out);
    QSqlQuery* select(QSqlQuery *q, const QString &sql, QMap<QString, QVariant>& values);
    bool deleteEntry(const QString &table, const QVariant &id);
    QSqlQuery* select(const QString &query);
    void logError(QSqlQuery &query);
    void logError(QSqlDatabase &db);
    inline void logQuery(QSqlQuery &query);
    QString fLastError;
    Db db();
private:
    QString lastQuery(QSqlQuery &q);
    QString fDbId;
    static int fDbNum;
};

#endif // DATABASE_H
