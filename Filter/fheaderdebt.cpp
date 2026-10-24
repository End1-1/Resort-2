#include "fheaderdebt.h"
#include "ui_fheaderdebt.h"
#include "wreportgrid.h"
#include "dlggposorderinfo.h"
#include "defines.h"

namespace {
enum DebtStateFilter {
    DEBT_STATE_ALL = 0,
    DEBT_STATE_UNPAID = 1,
    DEBT_STATE_PAID = 2
};

QString sqlLiteral(const QString &text)
{
    return QStringLiteral("'") + QString(text).replace(QLatin1Char('\''), QStringLiteral("''")) + QLatin1Char('\'');
}
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
    connect(ui->chLedger, &QCheckBox::toggled, [this](bool) {
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
    const QString dateFrom = ui->wd->ds1();
    const QString dateTo = ui->wd->ds2();
    const QString debtKind = sqlLiteral(tr("Debt"));
    const QString payKind = sqlLiteral(tr("Payment"));
    QString query;

    rg->fModel->clearColumns();

    if(ui->chLedger->isChecked()) {
        rg->fModel->setColumn(80, "", tr("Order"))
                .setColumn(120, "", tr("Gov number"))
                .setColumn(150, "", tr("Date"))
                .setColumn(100, "", tr("Type"))
                .setColumn(100, "", tr("Amount"))
                .setColumn(120, "", tr("Payment mode"));

        query = "select d.f_order, d.f_govnumber, "
                "date_format(d.f_datetime, '%Y-%m-%d %H:%i:%s') as f_dt, "
                "case when d.f_debt > 0 then " + debtKind + " else " + payKind + " end as f_kind, "
                "abs(d.f_debt) as f_amount, "
                "coalesce(pt.f_" + def_lang + ", '') as f_pm "
                "from o_header_debt d "
                "left join f_payment_type pt on pt.f_id=d.f_payment_mode "
                "where date(d.f_datetime) between " + dateFrom + " and " + dateTo + " ";

        switch(ui->cbDebtState->currentIndex()) {
        case DEBT_STATE_UNPAID:
            query += "and d.f_debt > 0.001 "
                     "and d.f_order in ( "
                     "select f_order from o_header_debt group by f_order having sum(f_debt) > 0.001) ";
            break;
        case DEBT_STATE_PAID:
            query += "and d.f_order in ( "
                     "select f_order from o_header_debt group by f_order having sum(f_debt) <= 0.001) ";
            break;
        default:
            break;
        }

        query += "order by d.f_datetime, d.f_order, d.f_id";
    } else {
        rg->fModel->setColumn(80, "", tr("Order"))
                .setColumn(120, "", tr("Gov number"))
                .setColumn(150, "", tr("Debt date"))
                .setColumn(150, "", tr("Payment date"))
                .setColumn(100, "", tr("Debt"))
                .setColumn(100, "", tr("Payment"))
                .setColumn(100, "", tr("Balance"));

        query =
                "select t.f_order, t.f_govnumber, t.f_debt_dt, t.f_pay_dt, "
                "t.f_debt_in, t.f_debt_pay, t.f_balance "
                "from ( "
                "select d.f_order, "
                "max(d.f_govnumber) as f_govnumber, "
                "date_format(min(case when d.f_debt > 0 then d.f_datetime end), '%Y-%m-%d %H:%i:%s') as f_debt_dt, "
                "date_format(max(case when d.f_debt < 0 then d.f_datetime end), '%Y-%m-%d %H:%i:%s') as f_pay_dt, "
                "sum(case when d.f_debt > 0 then d.f_debt else 0 end) as f_debt_in, "
                "sum(case when d.f_debt < 0 then -d.f_debt else 0 end) as f_debt_pay, "
                "max(bal.f_balance) as f_balance "
                "from o_header_debt d "
                "inner join ( "
                "select f_order, sum(f_debt) as f_balance "
                "from o_header_debt "
                "group by f_order "
                ") bal on bal.f_order = d.f_order "
                "where d.f_order in ( "
                "select f_order from o_header_debt "
                "where date(f_datetime) between " + dateFrom + " and " + dateTo + " "
                ") "
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

        query += ") t order by coalesce(t.f_pay_dt, t.f_debt_dt), t.f_order";
    }

    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);

    if(!ui->chLedger->isChecked()) {
        QList<int> cols;
        cols << 4 << 5 << 6;
        QList<double> sums;
        rg->fModel->sumOfColumns(cols, sums);
        rg->setTblTotalData(cols, sums);
    }
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
