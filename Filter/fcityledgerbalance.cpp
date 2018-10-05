#include "fcityledgerbalance.h"
#include "ui_fcityledgerbalance.h"
#include "wreportgrid.h"
#include "rgcityledgerdetailedbalance.h"

FCityLedgerBalance::FCityLedgerBalance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCityLedgerBalance)
{
    ui->setupUi(this);
    ui->deStart->setDate(WORKING_DATE);
    fReportGrid->fRgDoubleClick = new RGCityLedgerDetailedBalance(this);
    fReportGrid->setupTabTextAndIcon(tr("C/L Total balance"), ":/images/balance.png");
    fReportGrid->fStaticQuery = " \
            select m.f_cityLedger, cl.f_name,  \
            sum(m.f_amountAmd*m.f_sign)*-1, truncate(sum((m.f_amountAmd/m.f_amountUsd)*m.f_sign)*-1, 1)  \
            from m_register m \
            inner join f_city_ledger cl on cl.f_id=m.f_cityLedger \
            where m.f_finance=1 and  cl.f_id>0 and f_canceled=0 \
            and m.f_wdate <= :f_wdate1  \
            group by 1, 2 \
            :having \
            order by 1 \
             ";
    fReportGrid->fModel->setColumn(100, "", tr("Code"))
            .setColumn(400, "", tr("Name"))
            .setColumn(100, "", tr("Amount AMD"))
            .setColumn(100, "", tr("Amount USD"));
}

FCityLedgerBalance::~FCityLedgerBalance()
{
    delete ui;
}

void FCityLedgerBalance::apply(WReportGrid *rg)
{
    QString query = rg->fStaticQuery;
    query = query.replace(":f_wdate1", ui->deStart->dateMySql());

    if (!ui->chZeroBalance->isChecked()) {
        query.replace(":having", " having truncate(sum((m.f_amountAmd/m.f_amountUsd)*m.f_sign), 1) <> 0 ");
    } else {
        query.replace(":having", "");
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> c;
    QList<double> v;
    c << 2;
    rg->fModel->sumOfColumns(c, v);
    rg->setTblTotalData(c, v);
}

QWidget *FCityLedgerBalance::firstElement()
{
    return ui->deStart;
}

QString FCityLedgerBalance::reportTitle()
{
    return QString("%1, %2")
            .arg(tr("Total city ledger balance at"))
            .arg(ui->deStart->text());
}

void FCityLedgerBalance::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCityLedgerBalance *clb = new FCityLedgerBalance(rg);
    rg->addFilterWidget(clb);
    clb->apply(rg);
}

