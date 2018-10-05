#ifndef CACHEREDRESERVATION_H
#define CACHEREDRESERVATION_H

#include "cachebase.h"

#define cid_red_reservation 45

typedef struct : CI_Base {
    QString fRoom;
    QString fInvoice;
    QString fBalance;
    QString fEntry;
    QString fDeparture;
} CI_RedReservation;
Q_DECLARE_METATYPE(CI_RedReservation*)

class CacheRedReservation : public CacheBase<CI_RedReservation>
{
public:
    static CacheRedReservation *instance();
    virtual void load();
protected:
    virtual void updateItem(const QString &id);
private:
    static CacheRedReservation *fInstance;
    CacheRedReservation();
    ~CacheRedReservation();
};

#endif // CACHEREDRESERVATION_H
