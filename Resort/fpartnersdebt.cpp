#include "fpartnersdebt.h"
#include "ui_fpartnersdebt.h"
#include "wreportgrid.h"
#include "dlgcashop.h"
#include "cachecashdoc.h"

FPartnersDebt::FPartnersDebt(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FPartnersDebt)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Partners debt"), ":/images/category.png");
    fReportGrid->setBtnNewVisible(true);
    connect(fReportGrid, SIGNAL(newBtn()), this, SLOT(btnNew()));
}

FPartnersDebt::~FPartnersDebt()
{
    delete ui;
}

QString FPartnersDebt::reportTitle()
{
    return QString("%1 %2  - %3")
            .arg(tr("Partners debts"))
            .arg(ui->deFrom->text())
            .arg(ui->deTo->text());
}

QWidget *FPartnersDebt::firstElement()
{
    return ui->deFrom;
}

void FPartnersDebt::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Code"))
            .setColumn(300, "", tr("Partner"))
            .setColumn(150, "", tr("Cash, paid"))
            .setColumn(150, "", tr("Cash, no paid"))
            .setColumn(150, "", tr("Card, paid"))
            .setColumn(150, "", tr("Card, no paid"))
            .setColumn(150, "", tr("Total, no paid"))
            .setColumn(150, "", tr("Paid"))
            .setColumn(150, "", tr("Debt"));
    QString query = "select distinct(r.f_partner), p.f_name, coalesce(cp.amount, 0), coalesce(cnp.amount, 0), "
            "coalesce(cap.amount, 0), coalesce(canp.amount, 0), coalesce(cnp.amount, 0) + coalesce(canp.amount, 0) as cdiff, "
            "coalesce(cc.amount, 0) as paid, coalesce(cnp.amount, 0) + coalesce(canp.amount, 0) - coalesce(cc.amount, 0) as debt "
            "from r_docs r "
            "left join r_partners p on p.f_id=r.f_partner "
            "left join (select d.f_partner, sum(d.f_amount) as amount from r_docs d where f_state=1 and f_payment=1 group by 1) cp on cp.f_partner=r.f_partner "
            "left join (select d.f_partner, sum(d.f_amount) as amount from r_docs d where f_state=1 and f_payment=3 group by 1) cnp on cnp.f_partner=r.f_partner "
            "left join (select d.f_partner, sum(d.f_amount) as amount from r_docs d where f_state=1 and f_payment=2 group by 1) cap on cap.f_partner=r.f_partner "
            "left join (select d.f_partner, sum(d.f_amount) as amount from r_docs d where f_state=1 and f_payment=4 group by 1) canp on canp.f_partner=r.f_partner "
            "left join (select c.f_partner, sum(abs(c.f_amount)) as amount from c_cash c where c.f_docType=5 group by 1) cc on cc.f_partner=r.f_partner ";
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FPartnersDebt::btnNew()
{
    QList<QVariant> out;
    int row;
    if ((row = fReportGrid->fillRowValuesOut(out)) < 0) {
        message_info(tr("Nothing was selected"));
        return;
    }
    DlgCashOp::cashWithParams(CASHDOC_DEBT, out.at(0).toInt());
}
