#ifndef DESELECTORMENUDISHES_H
#define DESELECTORMENUDISHES_H

#include "dwselector.h"
#include "cachemenudishes.h"

class DESelectorMenuDishes : public DWSelector
{
    Q_OBJECT
public:
    DESelectorMenuDishes(QWidget *parent = 0);
    virtual void configure();
    void setMenu(const QString &menu);
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
private:
    QString fMenu;
signals:
    void menuDish(CI_MenuDishes *c);
};

#endif // DESELECTORMENUDISHES_H
