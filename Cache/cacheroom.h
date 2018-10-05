#ifndef CACHEROOM_H
#define CACHEROOM_H

#include "cachebase.h"

#define cid_room 33

typedef struct : CI_Base {
    int fState;
    QString fCategoryShort;
    QString fCategory;
    QString fBed;
    QString fView;
    int fFloor;
    int fSmoke;
    QString fPrice;
} CI_Room;
Q_DECLARE_METATYPE(CI_Room*)

class CacheRoom : public CacheBase<CI_Room>
{
public:
    static CacheRoom *instance();
    static CI_Room *room(const QString &code);
protected:
    virtual void updateItem(const QString &id);
private:
    CacheRoom();
    CacheRoom(CacheRoom &);
    ~CacheRoom();
    virtual void load();
    static CacheRoom *fInstance;
};

bool CI_Romm_compare(CI_Room* left, CI_Room* right);

#endif // CACHEROOM_H
