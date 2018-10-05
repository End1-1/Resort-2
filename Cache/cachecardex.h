#ifndef CACHECARDEX_H
#define CACHECARDEX_H

#include "cachebase.h"

#define cid_cardex 5

typedef struct : CI_Base {
    QString fVatMode;
    QString fGroup;
    QString fVatName;
    QString fCommission;
    QString fDateTo;
    bool fBookingRequired;
} CI_Cardex;
Q_DECLARE_METATYPE(CI_Cardex*)

class CacheCardex : public CacheBase<CI_Cardex>
{
public:
    static CacheCardex *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheCardex *fInstance;
    CacheCardex();
    ~CacheCardex();
};

#endif // CACHECARDEX_H
