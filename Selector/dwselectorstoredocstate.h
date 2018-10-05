#ifndef DWSELECTORSTOREDOCSTATE_H
#define DWSELECTORSTOREDOCSTATE_H

#include "dwtemplateselector.h"
#include "cachestoredocstate.h"

class DWSelectorStoreDocState : public DWTemplateSelector<CI_StoreDocState, CacheStoreDocState>
{
    Q_OBJECT
public:
    DWSelectorStoreDocState(QWidget *parent = 0);
};

#endif // DWSELECTORSTOREDOCSTATE_H
