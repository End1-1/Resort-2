#ifndef CACHERESTDISHTYPE_H
#define CACHERESTDISHTYPE_H

#include "cachebase.h"

#define cid_rest_dish_type 29

typedef struct : CI_Base {
} CI_RestDishType;
Q_DECLARE_METATYPE(CI_RestDishType*)

class CacheRestDishType : public CacheBase<CI_RestDishType>
{
public:
    static CacheRestDishType *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheRestDishType *fInstance;
    CacheRestDishType();
    ~CacheRestDishType();
};

#endif // CACHERESTDISHTYPE_H
