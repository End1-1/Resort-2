#ifndef CACHERESTHALL_H
#define CACHERESTHALL_H

#include "cachebase.h"

#define cid_rest_hall 25

typedef struct : CI_Base {
    QString fDefaultMenu;
} CI_RestHall;
Q_DECLARE_METATYPE(CI_RestHall*)

class CacheRestHall : public CacheBase<CI_RestHall>
{
public:
    static CacheRestHall *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheRestHall *fInstance;
    CacheRestHall();
    ~CacheRestHall();
};

#endif // CACHERESTHALL_H
