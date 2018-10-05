#include "dlgdebtpay.h"
#include "ui_dlgdebtpay.h"
#include "dwselectordebtcostumer.h"

#define SEL_DEBT_COST 1

DlgDebtPay::DlgDebtPay(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgDebtPay)
{
    ui->setupUi(this);
    DWSelectorDebtCostumer *dockDebt = new DWSelectorDebtCostumer(this);
    dockDebt->configure();
    dockDebt->setSelector(ui->leCostCode);
    dockDebt->setDialog(this, SEL_DEBT_COST);
}

DlgDebtPay::~DlgDebtPay()
{
    delete ui;
}

void DlgDebtPay::debtPay(int id)
{
    DlgDebtPay *d = new DlgDebtPay(fPreferences.getDefaultParentForMessage());
    if (id > 0) {
        d->fDbBind[":f_id"] = id;
        DatabaseResult dr;
        dr.select(d->fDb, "select * from o_debt_pay where f_id=:f_id", d->fDbBind);
        if (dr.rowCount() > 0) {
            d->ui->leCode->setInt(id);
            d->ui->leDate->setDate(dr.value("f_date").toDate());
            d->ui->leCostCode->setInt(dr.value("f_holder").toInt());
            d->ui->leAmount->setDouble(dr.value("f_amount").toDouble());
            d->dockResponse<CI_DebtCostumer, CacheDebtCostumer>(d->ui->leCostCode, d->ui->leCostName, 0);
        }
    }
    d->exec();
    delete d;
}

void DlgDebtPay::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_DEBT_COST: {
        CI_DebtCostumer *c = value.value<CI_DebtCostumer*>();
        dockResponse<CI_DebtCostumer, CacheDebtCostumer>(ui->leCostCode, ui->leCostName, c);
    }
    }
}

void DlgDebtPay::on_btnCancel_clicked()
{
    reject();
}

void DlgDebtPay::on_btnOk_clicked()
{
    if (ui->leCode->asInt() == 0) {
        ui->leCode->setInt(fDb.insert("o_debt_pay", fDbBind));
    }
    fDbBind[":f_date"] = ui->leDate->date();
    fDbBind[":f_holder"] = ui->leCostCode->asInt();
    fDbBind[":f_amount"] = ui->leAmount->asDouble();
    fDb.update("o_debt_pay", fDbBind, where_id(ui->leCode->asInt()));
    message_info(tr("Saved"));
    accept();
}
