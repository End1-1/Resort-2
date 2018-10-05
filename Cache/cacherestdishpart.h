#ifndef CACHERESTDISHPART_H
#define CACHERESTDISHPART_H

#include "cachebase.h"

#define cid_rest_dish_part 28

typedef struct : CI_Base {
} CI_RestDishPart;
Q_DECLARE_METATYPE(CI_RestDishPart*)

class CacheRestDishPart : public CacheBase<CI_RestDishPart>
{
public:
    static CacheRestDishPart *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheRestDishPart *fInstance;
    CacheRestDishPart();
    ~CacheRestDishPart();
};

#endif // CACHERESTDISHPART_H
