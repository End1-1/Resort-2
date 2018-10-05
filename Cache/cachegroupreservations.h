#ifndef CACHEGROUPRESERVATIONS_H
#define CACHEGROUPRESERVATIONS_H

#include "cachebase.h"

#define cid_group_reservation 117

typedef struct : CI_Base {

} CI_GroupReservation;
Q_DECLARE_METATYPE(CI_GroupReservation*)

class CacheGroupReservations : public CacheBase<CI_GroupReservation>
{
public:
    static CacheGroupReservations *instance();
    virtual void updateItem(const QString &id);
private:
    CacheGroupReservations();
    ~CacheGroupReservations();
    static CacheGroupReservations *fInstance;
};

#endif // CACHEGROUPRESERVATIONS_H
