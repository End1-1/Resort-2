#ifndef CACHEROOMCATEGORY_H
#define CACHEROOMCATEGORY_H

#include "cachebase.h"

#define cid_room_category 54

typedef struct : CI_Base {
    QString fFullName;
} CI_RoomCategory;
Q_DECLARE_METATYPE(CI_RoomCategory*)

class CacheRoomCategory : public CacheBase<CI_RoomCategory>
{
public:
    static CacheRoomCategory *instance();
    virtual void load();
public:
    static CacheRoomCategory *fInstance;
    CacheRoomCategory();
    ~CacheRoomCategory();
};

#endif // CACHEROOMCATEGORY_H
