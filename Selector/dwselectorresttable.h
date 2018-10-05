#ifndef DWSELECTORRESTTABLE_H
#define DWSELECTORRESTTABLE_H

#include "dwtemplateselector.h"
#include "cacheresttable.h"

class DWSelectorRestTable : public DWTemplateSelector<CI_RestTable, CacheRestTable>
{
    Q_OBJECT
public:
    DWSelectorRestTable(QWidget *parent = 0);
    virtual void configure();
    int fHall;
    inline virtual bool otherFilter(CI_Base *b) {CI_RestTable *r = static_cast<CI_RestTable*>(b); return fHall == 0 ? true : fHall == r->fHall;}
};

#endif // DWSELECTORRESTTABLE_H
