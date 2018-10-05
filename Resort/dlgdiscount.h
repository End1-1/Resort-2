#ifndef DLGDISCOUNT_H
#define DLGDISCOUNT_H

#include "baseextendeddialog.h"
#include "dwselectoractiveroom.h"
#include "dwselectorinvoiceitem.h"
#include "dwselectorcityledger.h"

namespace Ui {
class DlgDiscount;
}

class DlgDiscount : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgDiscount(QWidget *parent = 0);
    ~DlgDiscount();
    void setParams(const QString &room, double guestAmount, double companyAmount);
    static void openVaucher(const QString &id);
private slots:
    void discount(CI_InvoiceItem *c);
    void roomChanged(CI_ActiveRoom *c);
    void cityLedger(CI_CityLedger *c);
    void on_btnCancel_clicked();

    void on_leValue_textChanged(const QString &arg1);

    void on_rbAmount_clicked();

    void on_rbPercent_clicked();

    void on_gbGuest_clicked();

    void on_rbCompany_clicked();

    void on_btnOk_clicked();

    void on_rbCityLedger_clicked();

    void on_rbGuest_clicked();

    void on_btnPrint_clicked();

private:
    Ui::DlgDiscount *ui;
    QString fInvoice;
    double fGuestAmount;
    double fCompanyAmount;
    DWSelectorActiveRoom *fDockRoom;
    DWSelectorInvoiceItem *fDockDisc;
    DWSelectorCityLedger *fDockCL;
    void countDiscount();
    void sideChanged(bool guest);
};

#endif // DLGDISCOUNT_H
