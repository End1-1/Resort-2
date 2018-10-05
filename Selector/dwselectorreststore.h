#ifndef DWSELECTORRESTSTORE_H
#define DWSELECTORRESTSTORE_H

#include "dwselector.h"
#include "cachereststore.h"

class DWSelectorRestStore : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRestStore(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void store(CI_RestStore *s);
};

#endif // DWSELECTORRESTSTORE_H
