#ifndef DWSELECTORSTOREDOCTYPE_H
#define DWSELECTORSTOREDOCTYPE_H

#include "dwtemplateselector.h"
#include "cachestoredoc.h"

class DWSelectorStoreDocType : public DWTemplateSelector<CI_StoreDocType, CacheStoreDocType>
{
    Q_OBJECT
public:
    DWSelectorStoreDocType(QWidget *parent = 0);
};

#endif // DWSELECTORSTOREDOCTYPE_H
