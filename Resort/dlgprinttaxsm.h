#ifndef DLGPRINTTAXSM_H
#define DLGPRINTTAXSM_H

#include "baseextendeddialog.h"
#include "printtaxn.h"
#include <QTimer>

#define TAX_OK 1
#define TAX_FAIL 2
#define TAX_NONEED 0

#define tm_receipt 1
#define tm_advance 2
#define tm_taxback 3

namespace Ui {
class DlgPrintTaxSM;
}

class DlgPrintTaxSM : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPrintTaxSM(QWidget *parent = 0);
    ~DlgPrintTaxSM();
    void addGoods(const QString &dep, const QString &adgt, const QString &code, const QString &name, double price, double qty);
    virtual int exec();
    int execAdvance();
    int execTaxback();
    QString fOrder;
    double fCashAmount;
    double fCardAmount;
    double fPrepaid;
    int fTaxCode;
    int fTaxback;
    static bool printAdvance(double amountCash, double amountCard, const QString &vaucher, int &taxCode);
    static int printTaxback(int number, const QString &vaucher, int &taxCode);
private slots:
    void on_btnCancel_clicked();

    void on_btnRetry_clicked();

    void on_btnNoNeed_clicked();

private:
    Ui::DlgPrintTaxSM *ui;
    int fPrintMode;
    QTimer fTimer;
    QString fDep;
    PrintTaxN fPrintTax;
    void load();
    void loadAdvance();
    void loadTaxback();
};

#endif // DLGPRINTTAXSM_H
