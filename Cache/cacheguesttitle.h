#ifndef CACHEGUESTTITLE_H
#define CACHEGUESTTITLE_H

#include "cachebase.h"

#define cid_guest_title 23

typedef struct : CI_Base {
} CI_GuestTitle;
Q_DECLARE_METATYPE(CI_GuestTitle*)

class CacheGuestTitle : public CacheBase<CI_GuestTitle>
{
public:
    static CacheGuestTitle *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheGuestTitle *fInstance;
    CacheGuestTitle();
    CacheGuestTitle(CacheGuestTitle &g);
    ~CacheGuestTitle();
};

#endif // CACHEGUESTTITLE_H
