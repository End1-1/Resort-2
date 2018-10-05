#ifndef DWSELECTORTRACKINGTABLES_H
#define DWSELECTORTRACKINGTABLES_H

#include "dwselector.h"
#include "cachetrackingtables.h"

class DWSelectorTrackingTables : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorTrackingTables(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void tracking(CI_TrackingTables *c);
};

#endif // DWSELECTORTRACKINGTABLES_H
