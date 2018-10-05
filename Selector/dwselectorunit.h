#ifndef DWSELECTORUNIT_H
#define DWSELECTORUNIT_H

#include "dwtemplateselector.h"
#include "cacheunit.h"

class DWSelectorUnit : public DWTemplateSelector<CI_Unit, CacheUnit>
{
    Q_OBJECT
public:
    DWSelectorUnit(QWidget *parent = 0);
};

#endif // DWSELECTORUNIT_H
