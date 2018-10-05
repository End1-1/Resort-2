#ifndef CACHEACTIVEROOM_H
#define CACHEACTIVEROOM_H

#include "cachebase.h"

#define cid_active_room 21

typedef struct : CI_Base {
    QString fRoomCode;
    QString fRoomName;
    QString fInvoice;
    QString fGuestName;
} CI_ActiveRoom;
Q_DECLARE_METATYPE(CI_ActiveRoom*)

class CacheActiveRoom : public CacheBase<CI_ActiveRoom>
{
public:
    static CacheActiveRoom *instance();
    virtual void load();
protected:
    virtual void updateItem(const QString &id);
private:
    static CacheActiveRoom *fInstance;
    CacheActiveRoom();
    ~CacheActiveRoom();
};

#endif // CACHEACTIVEROOM_H
