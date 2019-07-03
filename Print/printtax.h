#ifndef PRINTTAX_H
#define PRINTTAX_H

#include "baseextendeddialog.h"

namespace Ui {
class PrintTax;
}

class PrintTax : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit PrintTax(QWidget *parent = nullptr);
    ~PrintTax();
    QString fAmountPre;
    QString fAmountCash;
    QString fAmountCard;
    QString fInvoice;
    QStringList fDept;
    QStringList fRecList;
    QStringList fCodeList;
    QStringList fNameList;
    QStringList fQtyList;
    QStringList fPriceList;
    QStringList fAdgCode;
    QStringList fTaxNameList;
    void setPrepaid(const QString &amount);
    void build();
    bool print();
    static void printAdvance(double amount, int paymentMode, const QString &doc);

private slots:
    void on_btnCancel_clicked();
    void on_btnPrint_clicked();
    void on_tblData_clicked(const QModelIndex &index);
    void on_leCard_textEdited(const QString &arg1);
    void on_leCash_textEdited(const QString &arg1);
    void on_lePre_textEdited(const QString &arg1);
    void on_chCheckUncheckAll_clicked(bool checked);
    void on_tblFilter_clicked(const QModelIndex &index);
    void on_btnClearFilter_clicked();

private:
    Ui::PrintTax *ui;
    QString fFilter;
    void correctAmounts(EQLineEdit *l1, EQLineEdit *l2, EQLineEdit *l3);
    void countAmount();
    void filter();
    int fTaxCode;
};

#endif // PRINTTAX_H
