#ifndef CACHEONE_H
#define CACHEONE_H

#include "database.h"
#include "preferences.h"
#include <QMap>

class CacheOne : public QObject
{
    Q_OBJECT
public:
    CacheOne();
    static void setDatabase(const Db &db);
    virtual void clear() = 0;
    static void clearCache(int id);
    static void clearAll();
    static void updateCache(int cache, const QString &id);
    int fCacheId;
protected:
    QString fQuery;
    static QString fDbHost;
    static QString fDbName;
    static QString fDbUser;
    static QString fDbPass;
    virtual void updateItem(const QString &id);
    QSqlQuery *prepareDb(QString query = "");
    void closeDb(QSqlQuery *q);
    Database fDb;
    void emitUpdated(const QString &id);
    static QMap<int, CacheOne*> fCacheOne;
signals:
    void cacheUpdated(const QString &id);
};

#endif // CACHEONE_H
