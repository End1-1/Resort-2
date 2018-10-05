#ifndef DLGTAXBACK_H
#define DLGTAXBACK_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgTaxBack;
}

class DlgTaxBack : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTaxBack(QWidget *parent = 0);
    ~DlgTaxBack();
    QString fInvoice;
    static void taxBack(const QString &invoice, QList<QList<QVariant> > &rows);
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::DlgTaxBack *ui;
};

#endif // DLGTAXBACK_H
