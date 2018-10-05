#ifndef DLGINVOICECANCELATION_H
#define DLGINVOICECANCELATION_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgInvoiceCancelation;
}

class DlgInvoiceCancelation : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgInvoiceCancelation(QWidget *parent = 0);
    ~DlgInvoiceCancelation();
    void setTrackWindowId(const QString &id);
    void addRow(const QList<QVariant> &data);

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgInvoiceCancelation *ui;
    QString fTrackId;
    bool fRaise;
};

#endif // DLGINVOICECANCELATION_H
