#include "cacheone.h"

QMap<int, CacheOne*> CacheOne::fCacheOne;
QString CacheOne::fDbHost;
QString CacheOne::fDbName;
QString CacheOne::fDbUser;
QString CacheOne::fDbPass;

CacheOne::CacheOne()
{
    fCacheId = 0;
    fQuery = "";
    fDb.setConnectionParams(fDbHost, fDbName, fDbUser, fDbPass);
}

void CacheOne::setDatabase(const Db &db)
{
    fDbHost = db.dc_main_host;
    fDbName = db.dc_main_path;
    fDbUser = db.dc_main_user;
    fDbPass = db.dc_main_pass;
}

void CacheOne::clearCache(int id)
{
    if (fCacheOne.contains(id)) {
        fCacheOne[id]->clear();
        fCacheOne[id]->emitUpdated("0");
    }
}

void CacheOne::clearAll()
{
    QMapIterator<int, CacheOne*> it(fCacheOne);
    while (it.hasNext()) {
        it.next();
        it.value()->clear();
    }
    qDeleteAll(fCacheOne);
    fCacheOne.clear();
}

void CacheOne::updateItem(const QString &id)
{
    Q_UNUSED(id)
}

QSqlQuery *CacheOne::prepareDb(QString query)
{
    if (!fDb.open()) {
        return new QSqlQuery();
    }
    if (query.isEmpty()) {
        query = fQuery;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        q = new QSqlQuery();
    }
    return q;
}

void CacheOne::closeDb(QSqlQuery *q)
{
    fDb.close();
    delete q;
}

void CacheOne::emitUpdated(const QString &id)
{
    emit cacheUpdated(id);
}

void CacheOne::updateCache(int cache, const QString &id)
{
    if (fCacheOne.contains(cache)) {
        fCacheOne[cache]->updateItem(id);
        fCacheOne[cache]->emitUpdated(id);
    }
}
