#ifndef DWSELECTORROOM_H
#define DWSELECTORROOM_H

#include "dwselector.h"
#include "cacheroom.h"

class DWSelectorRoom : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRoom(QWidget *parent = 0);
    virtual void configure();
protected:
    void select(const QVariant &value);
    void selectorFocusOut();
signals:
    void room(CI_Room *c);
};

#endif // DWSELECTORROOM_H
