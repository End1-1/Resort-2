#ifndef DLGORDERS_H
#define DLGORDERS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgOrders;
}

class DlgOrders : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgOrders(int staff, QWidget *parent = 0);
    ~DlgOrders();

private slots:
    void on_btnPrintTax_clicked();

    void on_btnCancel_clicked();

    void on_btnDown_clicked();

    void on_btnUp_clicked();

    void on_btnByNumber_clicked();

private:
    Ui::DlgOrders *ui;
    int fStaffId;
};

#endif // DLGORDERS_H
