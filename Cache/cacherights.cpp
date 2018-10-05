#include "cacherights.h"

CacheRights *CacheRights::fInstance = 0;
QMap<int, QMap<int, int> > CacheRights::fCacheRights;

CacheRights *CacheRights::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheRights();
        fCacheOne[cid_user_rights] = fInstance;
    }
    return fInstance;
}

void CacheRights::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_UserRights *c = new CI_UserRights();
        c->fCode = q->value(0).toString();
        c->fFlag = q->value(1).toInt() == 1;
        fCacheRights[q->value(2).toInt()][q->value(0).toInt()] = q->value(1).toInt();
    }
    closeDb(q);
}

void CacheRights::clear()
{
    fCacheRights.clear();
}

bool CacheRights::get(int group, int code)
{
    if (fCacheRights.contains(group)) {
        if (fCacheRights[group].contains(code)) {
            return fCacheRights[group][code] == 1;
        }
    }
    return false;
}

CacheRights::CacheRights() :
    CacheBase()
{
    fQuery = "select f_right, f_flag, f_group from users_rights";
    fCacheId = cid_user_rights;
    if (fCacheRights.isEmpty()) {
        load();
    }
}

CacheRights::~CacheRights()
{
    fInstance = 0;
}

CI_UserRights *CacheRights::get(const QString &code)
{
    Q_UNUSED(code)
    return 0;
}

CI_UserRights *CacheRights::get(int code)
{
    Q_UNUSED(code)
    return 0;
}
