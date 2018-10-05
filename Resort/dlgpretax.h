#ifndef DLGPRETAX_H
#define DLGPRETAX_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgPreTax;
}

class DlgPreTax : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPreTax(QWidget *parent = 0);
    ~DlgPreTax();
    void addRow(const QList<QVariant> &data);
    void setInvoice(const QString &invoice);
private slots:
    void on_tblData_clicked(const QModelIndex &index);
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgPreTax *ui;
    QString fInvoice;
    void countTotal();
};

#endif // DLGPRETAX_H
