#ifndef DWSELECTORHALL_H
#define DWSELECTORHALL_H

#include "dwselector.h"
#include "cacheresthall.h"

class DWSelectorHall : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorHall(QWidget *parent = 0);
    virtual void configure();
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void hall(CI_RestHall *r);
};

#endif // DWSELECTORHALL_H
