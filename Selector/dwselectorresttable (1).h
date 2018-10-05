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
};

#endif // DWSELECTORRESTTABLE_H
