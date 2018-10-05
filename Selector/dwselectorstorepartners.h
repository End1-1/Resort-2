#ifndef DWSELECTORSTOREPARTNERS_H
#define DWSELECTORSTOREPARTNERS_H

#include "dwtemplateselector.h"
#include "cachestorepartners.h"

class DWSelectorStorePartners : public DWTemplateSelector<CI_StorePartners, CacheStorePartners>
{
    Q_OBJECT
public:
    DWSelectorStorePartners(QWidget *parent = 0);
    virtual void configure();
};

#endif // DWSELECTORSTOREPARTNERS_H
