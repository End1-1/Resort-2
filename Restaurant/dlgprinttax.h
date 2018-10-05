#ifndef DLGPRINTTAX_H
#define DLGPRINTTAX_H

#include "baseextendeddialog.h"
#include "printtaxn.h"
#include <QTimer>

#define TAX_OK 1
#define TAX_FAIL 2
#define TAX_NONEED 0

namespace Ui {
class DlgPrintTax;
}

class DlgPrintTax : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPrintTax(QWidget *parent = 0);
    ~DlgPrintTax();
    static int printTax(const QString &dep, const QString &order, double cardAmount, int &taxCode);
    virtual int exec();
private slots:
    void on_btnCancel_clicked();

    void on_btnRetry_clicked();

    void on_btnNoNeed_clicked();

private:
    Ui::DlgPrintTax *ui;
    QTimer fTimer;
    QString fDep;
    QString fOrder;
    int fTaxCode;
    double fCardAmount;
    void load(const QString &dep, const QString &order);
};

#endif // DLGPRINTTAX_H
