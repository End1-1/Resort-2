#ifndef GODAILY_H
#define GODAILY_H

#include "gowidget.h"

namespace Ui {
class GODaily;
}

class GODaily : public GOWidget
{
    Q_OBJECT

public:
    explicit GODaily(const QString &name, QWidget *parent = 0);
    ~GODaily();
    virtual void save();
private slots:

private:
    Ui::GODaily *ui;
};

#endif // GODAILY_H
