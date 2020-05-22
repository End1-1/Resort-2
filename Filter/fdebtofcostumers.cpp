#include "fdebtofcostumers.h"
#include "ui_fdebtofcostumers.h"
#include "wreportgrid.h"
#include "dwselectordebtcostumer.h"
#include "dlggposorderinfo.h"
#include "dlgdebtpay.h"

#define SEL_DEBT 1

FDebtOfCostumers::FDebtOfCostumers(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FDebtOfCostumers)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Debts of costumers"), ":/images/refung.png");
    fReportGrid->addToolBarButton(":/images/refund.png", tr("Payment"), SLOT(payment()), this);
    fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removePay()), this);
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(dblClick(QList<QVariant>)));
    ui->deStart->setDate(QDate::currentDate().addDays(-31));
    DWSelectorDebtCostumer *dc = new DWSelectorDebtCostumer(this);
    dc->configure();
    dc->setSelector(ui->leCostCode);
    dc->setDialog(this, SEL_DEBT);
}

FDebtOfCostumers::~FDebtOfCostumers()
{
    delete ui;
}

QString FDebtOfCostumers::reportTitle()
{
    return QString("%1").arg(tr("Debts of costumers"));
}

QWidget *FDebtOfCostumers::firstElement()
{
    return ui->leCostCode;
}

void FDebtOfCostumers::apply(WReportGrid *rg)
{
    if (ui->chTotal->isChecked()) {
        rg->fModel->clearColumns();
        rg->fModel->setColumn(300, "", tr("Costumer"))
                .setColumn(100, "", tr("Amount"));
        QString query = "select t.f_name, sum(t.f_debt) "
                "from (select d.f_name, v.f_debt "
                "from o_header_payment v "
                "left join o_header h on h.f_id=v.f_id "
                "left join o_debt_holder d on d.f_id=v.f_debtHolder "
                "where v.f_debt>0 and h.f_state=2 "
                " union "
                " select d.f_name, p.f_amount * -1 as f_debt "
                "from o_debt_pay p "
                "left join o_debt_holder d on d.f_id=p.f_holder ) t "
                "group by 1 ";

        rg->fModel->setSqlQuery(query);
        rg->fModel->apply(rg);
        QList<int> cols;
        cols << 1;
        QList<double> vals;
        rg->fModel->sumOfColumns(cols, vals);
        rg->setTblTotalData(cols, vals);
    } else {
        rg->fModel->clearColumns();
        rg->fModel->setColumn(120, "", tr("Date"))
                .setColumn(120, "", tr("Order"))
                .setColumn(300, "", tr("Costumer"))
                .setColumn(100, "", tr("Amount"));
        QString query = "select h.f_dateCash, h.f_id, d.f_name, v.f_debt "
                "from o_header_payment v "
                "left join o_header h on h.f_id=v.f_id "
                "left join o_debt_holder d on d.f_id=v.f_debtHolder "
                "where v.f_debt>0 and h.f_state=2 ";
        query += QString(" and h.f_datecash between %1 and %2 ").arg(ui->deStart->dateMySql()).arg(ui->deEnd->dateMySql());
        if (ui->leCostCode->asInt() > 0) {
            query += " and v.f_debtHolder=" + ui->leCostCode->text();
        }
        query += " union ";
        query += " select p.f_date, p.f_id, d.f_name, p.f_amount * -1 "
                "from o_debt_pay p "
                "left join o_debt_holder d on d.f_id=p.f_holder ";
        if (ui->leCostCode->asInt() > 0) {
            query += " where p.f_holder=" + ui->leCostCode->text();
        }
        query += QString(" and p.f_date between %1 and %2 ").arg(ui->deStart->dateMySql()).arg(ui->deEnd->dateMySql());

        rg->fModel->setSqlQuery(query);
        rg->fModel->apply(rg);
        QList<int> cols;
        cols << 3;
        QList<double> vals;
        rg->fModel->sumOfColumns(cols, vals);
        rg->setTblTotalData(cols, vals);
    }
}

void FDebtOfCostumers::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_DEBT:
        dock<CI_DebtCostumer, CacheDebtCostumer>(ui->leCostCode, ui->leCostName, value.value<CI_DebtCostumer*>());
        break;
    }
}

void FDebtOfCostumers::payment()
{
    DlgDebtPay::debtPay(0);
}

void FDebtOfCostumers::removePay()
{
    if (fReportGrid->fModel->columnCount() == 2){
        return;
    }
    QList<QVariant> r;
    int row;
    if ((row = fReportGrid->fillRowValuesOut(r)) < 0) {
        return;
    }
    if (r.at(3).toDouble() > 0) {
        return;
    }
    if (message_confirm(tr("Confirm to remove")) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] = r.at(1).toInt();
    fDb.select("delete from o_debt_pay where f_id=:f_id", fDbBind, fDbRows);
    fReportGrid->fModel->removeRow(row);
}

void FDebtOfCostumers::dblClick(const QList<QVariant> &r)
{
    if (fReportGrid->fModel->columnCount() == 2){
        return;
    }
    if (r.count() == 0) {
        return;
    }
    if (r.at(3).toDouble() < 0) {
        DlgDebtPay::debtPay(r.at(1).toInt());
    } else {
        DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
        d->setOrder(r.at(1).toString());
        d->exec();
        delete d;
    }
}
