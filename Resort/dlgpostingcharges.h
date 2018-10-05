#ifndef DLGPOSTINGCHARGES_H
#define DLGPOSTINGCHARGES_H

#include "baseextendeddialog.h"
#include "dwselectorinvoiceitem.h"
#include "dwselectoractiveroom.h"
#include "dwselectorcityledger.h"

namespace Ui {
class DlgPostingCharges;
}

class DlgPostingCharges : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPostingCharges(QWidget *parent = 0);
    ~DlgPostingCharges();
    void loadVaucher(const QString &id);
    void setRoom(const QString &room);
    void setInvoice(const QString &id);
    void setSaleItem(const QString &code, bool disableChanges = false);
    int fLastInsertId;
private slots:
    void cityLedger(CI_CityLedger *c);
    void invoiceItem(CI_InvoiceItem *c);
    void roomChanged(CI_ActiveRoom *c);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_leRoomCode_returnPressed();
    void on_leItemCode_returnPressed();
    void on_leAmount_textEdited(const QString &arg1);
    void on_cbVAT_currentIndexChanged(int index);
    void on_chPayGuest_clicked(bool checked);
    void on_chPayByComp_clicked(bool checked);
    void on_rbRoom_clicked(bool checked);
    void on_rbDirect_clicked(bool checked);
    void on_rbCity_clicked(bool checked);
    void on_btnPrint_clicked();

    void on_btnPrintTax_clicked();

    void on_leTaxCash_textChanged(const QString &arg1);

    void on_leTaxCard_textChanged(const QString &arg1);

    void on_leTaxPre_textChanged(const QString &arg1);

    void on_btnNew_clicked();

private:
    Ui::DlgPostingCharges *ui;
    QStringList fCompanySide;
    DWSelectorInvoiceItem *fDockInvoiceItem;
    DWSelectorActiveRoom *fDockActiveRoom;
    DWSelectorCityLedger *fDockCity;
    void postRoom();
    void correctAmounts(EQLineEdit *l1, EQLineEdit *l2, EQLineEdit *l3);
    void disablePayment();
    void enablePayment();
};

#endif // DLGPOSTINGCHARGES_H
