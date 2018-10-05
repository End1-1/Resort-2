#ifndef RCHANGEHALL_H
#define RCHANGEHALL_H

#include "baseextendeddialog.h"
#include "hall.h"

namespace Ui {
class RChangeHall;
}

class RChangeHall : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RChangeHall(QWidget *parent = 0);
    ~RChangeHall();
    void setup(QList<HallStruct *> &hall);
    int hall();

private slots:

    void on_tblHall_clicked(const QModelIndex &index);

    void on_btnCancel_clicked();

private:
    Ui::RChangeHall *ui;
    int fHall;
};

#endif // RCHANGEHALL_H
