#ifndef DWSELECTORBED_H
#define DWSELECTORBED_H

#include "dwselector.h"
#include "cachebed.h"

class DWSelectorBed : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorBed(QWidget *parent = 0);
    virtual void configure();
protected slots:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void bed(CI_Bed *c);
};

#endif // DWSELECTORBED_H
