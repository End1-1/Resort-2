#ifndef DOUBLEDATABASE_H
#define DOUBLEDATABASE_H

#include <QSqlDatabase>
#include <QMap>
#include <QVariant>

class QSqlQuery;

class DoubleDatabase
{
public:
    DoubleDatabase();
    ~DoubleDatabase();
    QVariant &operator[](const QString &name);
    void setDatabase(const QString &host, const QString &db, const QString &user, const QString &password, int dbNum);
    bool open(bool db1, bool db2);
    void close(bool commit = true);
    bool exec(const QString &sqlQuery);
    QList<QList<QVariant> > fDbRows;
    QString fLastError;
    int rowCount();
private:
    static int fCounter;
    bool fExecFlagMaster;
    bool fExecFlagSlave;
    QString fDbName1;
    QString fDbName2;
    QSqlDatabase fDb1;
    QSqlDatabase fDb2;
    QMap<QString, QVariant> fBindValues;
    void configureDatabase(QSqlDatabase &cn, const QString &host, const QString &db, const QString &user, const QString &password);
    void logEvent(const QString &event);
    QString lastQuery(QSqlQuery *q);
    bool exec(QSqlQuery *q, const QString &sqlQuery, bool &isSelect);
};

extern QString __dd1Host;
extern QString __dd1Database;
extern QString __dd1Username;
extern QString __dd1Password;

extern QString __dd2Host;
extern QString __dd2Database;
extern QString __dd2Username;
extern QString __dd2Password;

#endif // DOUBLEDATABASE_H
