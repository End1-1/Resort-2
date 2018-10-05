#include "cacheroom.h"

CacheRoom *CacheRoom::fInstance = 0;

CacheRoom *CacheRoom::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheRoom();
        fCacheOne.insert(cid_room, fInstance);
    }
    return fInstance;
}

CI_Room *CacheRoom::room(const QString &code)
{
    return fInstance->get(code);
}

void CacheRoom::updateItem(const QString &id)
{
    QString sid = "0";
    if (id.toInt() > 0) {
        sid = id;
    }
    QSqlQuery *q = prepareDb(fQuery + " where r.f_id=" + sid);
    if (q->next()) {
        CI_Room *c;
        if (fStaticCache[fCacheId].contains(id)) {
            c = fStaticCache[cid_room][id];
        } else {
            c = new CI_Room();
        }
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fState = q->value(2).toInt();
        c->fCategoryShort = q->value(3).toString();
        c->fCategory = q->value(4).toString();
        c->fBed = q->value(5).toString();
        c->fSmoke = q->value(6).toInt();
        c->fView = q->value(7).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheRoom::CacheRoom() :
    CacheBase()
{
    fQuery = "select r.f_id, r.f_short, r.f_state, c.f_short, c.f_description, r.f_bed, r.f_smoke, "
            "r.f_rate, r.f_floor, v.f_" + def_lang + " "
            "from f_room r "
            "inner join f_room_classes c on c.f_id=r.f_class "
            "inner join f_room_view v on v.f_id=r.f_view ";
    fCacheId = cid_room;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoom::~CacheRoom()
{
    fInstance = 0;
}

void CacheRoom::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_Room *c = new CI_Room();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fState = q->value(2).toInt();
        c->fCategoryShort = q->value(3).toString();
        c->fCategory = q->value(4).toString();
        c->fBed = q->value(5).toString();
        c->fSmoke = q->value(6).toInt();
        c->fPrice = q->value(7).toString();
        c->fFloor = q->value(8).toInt();
        c->fView = q->value(7).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    fDb.close();
    delete q;
}

bool CI_Romm_compare(CI_Room *left, CI_Room *right)
{
    return left->fCode.toInt() < right->fCode.toInt();
}
