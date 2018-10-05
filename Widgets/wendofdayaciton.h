#ifndef WENDOFDAYACITON_H
#define WENDOFDAYACITON_H

#include <QWidget>

namespace Ui {
class WEndOfDayAciton;
}

class WEndOfDayAciton : public QWidget
{
    Q_OBJECT

public:
    explicit WEndOfDayAciton(QWidget *parent = 0);
    ~WEndOfDayAciton();

private:
    Ui::WEndOfDayAciton *ui;
};

#endif // WENDOFDAYACITON_H
