#ifndef DWSELECTORRESTPRINTER_H
#define DWSELECTORRESTPRINTER_H

#include "dwselector.h"
#include "cacherestprinters.h"

class DWSelectorRestPrinter : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRestPrinter(QWidget *parent = 0);
    virtual void configure();
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void printer(CI_RestPrinter *p);
};

#endif // DWSELECTORRESTPRINTER_H
