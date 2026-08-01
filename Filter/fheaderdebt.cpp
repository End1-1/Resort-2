#include "fheaderdebt.h"
#include "ui_fheaderdebt.h"
#include "wreportgrid.h"
#include "dlggposorderinfo.h"

namespace {
enum DebtStateFilter {
    DEBT_STATE_ALL = 0,
    DEBT_STATE_UNPAID = 1,
    DEBT_STATE_PAID = 2
};
}

FHeaderDebt::FHeaderDebt(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FHeaderDebt)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Order debts"), ":/images/refund.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
    connect(ui->cbDebtState, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int) {
        apply(fReportGrid);
    });
}

FHeaderDebt::~FHeaderDebt()
{
    delete ui;
}

QString FHeaderDebt::reportTitle()
{
    return tr("Order debts %1 - %2").arg(ui->wd->ds1(), ui->wd->ds2());
}

QWidget *FHeaderDebt::firstElement()
{
    return ui->wd->fw();
}

void FHeaderDebt::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(80, "", tr("Order"))
            .setColumn(120, "", tr("Gov number"))
            .setColumn(140, "", tr("Date"))
            .setColumn(100, "", tr("Debt"))
            .setColumn(100, "", tr("Payment"))
            .setColumn(100, "", tr("Balance"));

    QString query =
            "select d.f_order, "
            "max(d.f_govnumber) as f_govnumber, "
            "min(d.f_datetime) as f_datetime, "
            "sum(case when d.f_debt > 0 then d.f_debt else 0 end) as f_debt_in, "
            "sum(case when d.f_debt < 0 then -d.f_debt else 0 end) as f_debt_pay, "
            "max(bal.f_balance) as f_balance "
            "from o_header_debt d "
            "inner join ( "
            "select f_order, sum(f_debt) as f_balance "
            "from o_header_debt "
            "group by f_order "
            ") bal on bal.f_order = d.f_order "
            "where date(d.f_datetime) between " + ui->wd->ds1() + " and " + ui->wd->ds2() + " "
            "group by d.f_order ";

    switch(ui->cbDebtState->currentIndex()) {
    case DEBT_STATE_UNPAID:
        query += "having max(bal.f_balance) > 0.001 ";
        break;
    case DEBT_STATE_PAID:
        query += "having max(bal.f_balance) <= 0.001 ";
        break;
    default:
        break;
    }

    query += "order by min(d.f_datetime), d.f_order";

    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);

    QList<int> cols;
    cols << 3 << 4 << 5;
    QList<double> sums;
    rg->fModel->sumOfColumns(cols, sums);
    rg->setTblTotalData(cols, sums);
    rg->fTableView->resizeColumnsToContents();
}

void FHeaderDebt::doubleClickOnRow(const QList<QVariant> &row)
{
    if(row.isEmpty()) {
        return;
    }

    const int orderId = row.at(0).toInt();

    if(orderId <= 0) {
        return;
    }

    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(QString::number(orderId));
    d->exec();
    delete d;
}
