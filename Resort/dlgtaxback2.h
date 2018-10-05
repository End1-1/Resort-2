#ifndef DLGTAXBACK2_H
#define DLGTAXBACK2_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgTaxBack2;
}

class DlgTaxBack2 : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTaxBack2(QWidget *parent = 0);
    ~DlgTaxBack2();
    static void taxBack(const QString &invoice, const QList<QVariant> &numbers);
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::DlgTaxBack2 *ui;
    QString fInvoice;
    void load(const QString &ids);
};

#endif // DLGTAXBACK2_H
