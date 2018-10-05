#ifndef GOEXPEXTEDARRIVALS_H
#define GOEXPEXTEDARRIVALS_H

#include "gowidget.h"

namespace Ui {
class GOExpextedArrivals;
}

class GOExpextedArrivals : public GOWidget
{
    Q_OBJECT

public:
    explicit GOExpextedArrivals(const QString &name, QWidget *parent = 0);
    ~GOExpextedArrivals();
    virtual void save();

private:
    Ui::GOExpextedArrivals *ui;
};

#endif // GOEXPEXTEDARRIVALS_H
