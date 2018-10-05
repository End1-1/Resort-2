#ifndef DLGPARTNERPAYMENT_H
#define DLGPARTNERPAYMENT_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgPartnerPayment;
}

class DlgPartnerPayment : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPartnerPayment(QWidget *parent = 0);
    ~DlgPartnerPayment();

private:
    Ui::DlgPartnerPayment *ui;
};

#endif // DLGPARTNERPAYMENT_H
