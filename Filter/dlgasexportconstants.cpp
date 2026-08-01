#include "dlgasexportconstants.h"
#include "ui_dlgasexportconstants.h"
#include "message.h"

DlgAsExportConstants::DlgAsExportConstants(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAsExportConstants)
{
    ui->setupUi(this);
    setWindowTitle(tr("ArmSoft export constants"));
}

DlgAsExportConstants::~DlgAsExportConstants()
{
    delete ui;
}

void DlgAsExportConstants::setValues(const AsExportConstants &values)
{
    ui->leDocNumberStart->setText(QString::number(values.docNumberStart));
    ui->leBuyer->setText(values.buyer);
    ui->leBuyerAccount->setText(values.buyerAccount);
    ui->lePrepayAccount->setText(values.prepayAccount);
    ui->leCashlessAccount->setText(values.cashlessAccount);
    ui->leCashlessAmount->setText(values.cashlessAmount);
    ui->lePrepayUsage->setText(values.prepayUsage);
    ui->leComment->setText(values.comment);
    ui->leVatCalcMethod->setText(values.vatCalcMethod);
    ui->leVatAccount->setText(values.vatAccount);
    ui->leIssueMethod->setText(values.issueMethod);
    ui->leDocStatus->setText(values.docStatus);
    ui->leVatLine->setText(values.vatLine);
    ui->leTransType->setText(values.transType);
    ui->leEcoTax->setText(values.ecoTax);
    ui->leExpenseAccount->setText(values.expenseAccount);
    ui->leRevenueAccount->setText(values.revenueAccount);
}

void DlgAsExportConstants::fillValues(AsExportConstants &values) const
{
    values.docNumberStart = ui->leDocNumberStart->text().trimmed().toLongLong();
    values.buyer = ui->leBuyer->text().trimmed();
    values.buyerAccount = ui->leBuyerAccount->text().trimmed();
    values.prepayAccount = ui->lePrepayAccount->text().trimmed();
    values.cashlessAccount = ui->leCashlessAccount->text().trimmed();
    values.cashlessAmount = ui->leCashlessAmount->text().trimmed();
    values.prepayUsage = ui->lePrepayUsage->text().trimmed();
    values.comment = ui->leComment->text().trimmed();
    values.vatCalcMethod = ui->leVatCalcMethod->text().trimmed();
    values.vatAccount = ui->leVatAccount->text().trimmed();
    values.issueMethod = ui->leIssueMethod->text().trimmed();
    values.docStatus = ui->leDocStatus->text().trimmed();
    values.vatLine = ui->leVatLine->text().trimmed();
    values.transType = ui->leTransType->text().trimmed();
    values.ecoTax = ui->leEcoTax->text().trimmed();
    values.expenseAccount = ui->leExpenseAccount->text().trimmed();
    values.revenueAccount = ui->leRevenueAccount->text().trimmed();
}

void DlgAsExportConstants::on_btnCancel_clicked()
{
    reject();
}

void DlgAsExportConstants::on_btnOk_clicked()
{
    AsExportConstants values;
    fillValues(values);

    QString error;
    if(!values.isValid(&error)) {
        message_error(error);
        return;
    }

    accept();
}
