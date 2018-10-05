#ifndef CACHERESERVATION_H
#define CACHERESERVATION_H

#include "cachebase.h"

#define cid_reservation 34

typedef struct : CI_Base {
    QString fId;
    int fState;
    int fArrangement;
    int fGroup;
    QString fName;
    QString fRoom;
    QString fRoomShort;
    QString fCardex;
    QString fDateCreate;
    QDate fDateStart;
    QDate fDateEnd;
    QString fGuest;
    QString fCardexName;
    QString fRemarks;
    int fAuthor;
    QString fInvoice;
    int fStatus;
    QString fStatusName;
    QString fAdvance;
    QString fCredit;
    QString fDebet;
    QString fBalance;
} CI_Reservation;
Q_DECLARE_METATYPE(CI_Reservation*)

class CacheReservation : public CacheBase<CI_Reservation>
{
public:
    static CacheReservation *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
    bool hasNext(CI_Reservation *c);
    bool hasNext(CI_Reservation *c, CI_Reservation *&next);
    bool hasPrev(CI_Reservation *c);
    void check(const QDate &start, const QDate &end, int room, QMap<QString, CI_Reservation *> &out, bool &startOk, bool &endOk, const QString &reserveId);
    void exludeList(const QDate &start, const QDate &end, QSet<int> &excludeRooms);
private:
    static CacheReservation *fInstance;
    CacheReservation();
    CacheReservation(CacheReservation &c);
    ~CacheReservation();
};

#endif // CACHERESERVATION_H
