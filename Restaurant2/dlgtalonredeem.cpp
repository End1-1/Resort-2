#include "dlgtalonredeem.h"
#include "ui_dlgtalonredeem.h"
#include <QtMath>

namespace {
const double AMOUNT_TOLERANCE = 1.0;

bool amountsMatch(double talonPrice, double orderAmount)
{
    return qAbs(talonPrice - orderAmount) < AMOUNT_TOLERANCE;
}
}

DlgTalonRedeem::DlgTalonRedeem(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgTalonRedeem)
{
    ui->setupUi(this);
}

DlgTalonRedeem::~DlgTalonRedeem()
{
    delete ui;
}

bool DlgTalonRedeem::confirm(const TalonRedeemInfo &info, double orderAmount, QWidget *parent)
{
    DlgTalonRedeem dlg(parent);
    const QString number = info.code.length() > 4 ? info.code.right(4) : info.code;

    dlg.ui->lbNumber->setText(number);
    dlg.ui->lbTalonPrice->setText(QString::number(info.price, 'f', 0));
    dlg.ui->lbOrderAmount->setText(QString::number(orderAmount, 'f', 0));

    const bool match = amountsMatch(info.price, orderAmount);

    if(match) {
        dlg.ui->lbMessage->setText(dlg.tr("Amounts match."));
        dlg.ui->btnApply->setVisible(true);
        dlg.ui->btnCancel->setVisible(true);
        dlg.ui->btnOk->setVisible(false);
        return dlg.exec() == QDialog::Accepted;
    }

    dlg.ui->lbMessage->setText(dlg.tr("Amounts do not match. The talon cannot be applied."));
    dlg.ui->btnApply->setVisible(false);
    dlg.ui->btnCancel->setVisible(false);
    dlg.ui->btnOk->setVisible(true);
    dlg.exec();
    return false;
}

void DlgTalonRedeem::on_btnApply_clicked()
{
    accept();
}

void DlgTalonRedeem::on_btnCancel_clicked()
{
    reject();
}

void DlgTalonRedeem::on_btnOk_clicked()
{
    reject();
}
