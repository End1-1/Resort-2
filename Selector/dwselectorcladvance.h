#ifndef DWSELECTORCLADVANCE_H
#define DWSELECTORCLADVANCE_H

#include "dwtemplateselector.h"
#include "cachecladvance.h"

class DWSelectorCLAdvance : public DWTemplateSelector<CI_CLAdvance, CacheCLAdvance>
{
    Q_OBJECT
public:
    DWSelectorCLAdvance(QWidget *parent = 0);
    virtual void configure();
};

#endif // DWSELECTORCLADVANCE_H
