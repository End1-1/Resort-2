#ifndef WACCINVOICE_H
#define WACCINVOICE_H

#include "basewidget.h"
#include "dwselectorcheckoutinvoice.h"
#include "dwselectorcardex.h"
#include "dwselectorvatmode.h"

namespace Ui {
class WAccInvoice;
}

class WReportGrid;

class WAccInvoice : public BaseWidget
{
    Q_OBJECT

public:
    explicit WAccInvoice(QWidget *parent = 0);
    ~WAccInvoice();
    void load(const QString &id);
    virtual void setupTab();
    virtual void selector(int selectorNumber, const QVariant &value);
protected:
    virtual bool event(QEvent *event);
private:
    Ui::WAccInvoice *ui;
    WReportGrid *fReport;
    QString invoiceId();
    void viewEntries();
    void countAmounts();
    int countNights();
    DWSelectorCheckoutInvoice *fDockInvoice;
    DWSelectorCardex *fDockCardex;
    DWSelectorVATMode *fDockVat;
    int fCurrentInvoicePosition;
private slots:
    void cardex(CI_Cardex *c);
    void vatMode(CI_VATMode *c);
    void handleValues(const QList<QVariant> &values);
    void on_btnNext_clicked();
    void on_btnPreviouse_clicked();
    void on_btnTracking_clicked();
    void on_rbEntryName_clicked();
    void on_rbNoGroup_clicked();
    void on_chPaymentByGuest_clicked();
    void on_chPaymentByCompany_clicked();
    void on_btnPrint_clicked();
    void on_btnTaxPrint_clicked();
    void on_leInvoice_returnPressed();
    void on_btnTaxBack_clicked();
    void on_btnViewCorrections_clicked();
    void on_btnEliminate_clicked();
    void on_tblData_doubleClicked(const QModelIndex &index);
    void on_deDeparture_userDateChanged(const QDate &date);
    void on_deEntry_userDateChanged(const QDate &date);
    void on_btnPostingCharges_clicked();
    void on_btnMoveItem_clicked();
    void on_btnChangeCL_clicked();
    void on_btnNewVaucher_clicked();
    void on_deEntry_dateChanged(const QDate &date);
    void on_deCheckIn_dateChanged(const QDate &date);
    void on_btnEditReserv_clicked();
    void on_btnExcel_clicked();
    void on_btnEditRowVaucher_clicked();
    void on_chViewAllVauchers_clicked();
    void on_btnViewReserv_clicked();
    void on_btnGuestInfo_clicked();
};

#endif // WACCINVOICE_H
