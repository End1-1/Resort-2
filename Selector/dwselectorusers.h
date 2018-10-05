#ifndef DWSELECTORUSERS_H
#define DWSELECTORUSERS_H

#include "dwselector.h"
#include "cacheusers.h"

class DWSelectorUsers : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorUsers(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void user(CI_User *c);
};

#endif // DWSELECTORUSERS_H
