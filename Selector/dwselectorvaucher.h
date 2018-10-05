#ifndef DWSELECTORVAUCHER_H
#define DWSELECTORVAUCHER_H

#include "dwtemplateselector.h"
#include "cachevaucher.h"

class DWSelectorVaucher : public DWTemplateSelector<CI_Vaucher, CacheVaucher>
{
    Q_OBJECT
public:
    DWSelectorVaucher(QWidget *parent = 0);
    virtual void configure();
};

#endif // DWSELECTORVAUCHER_H
