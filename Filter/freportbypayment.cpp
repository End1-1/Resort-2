#include "freportbypayment.h"
#include "ui_freportbypayment.h"
#include "wreportgrid.h"
#include "dlggetidname.h"

FReportByPayment::FReportByPayment(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FReportByPayment)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Report by payment"), ":/images/category.png");
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FReportByPayment::branchEditDoubleClick);
    connect(ui->leHall, &EQLineEdit::customButtonClicked, this, &FReportByPayment::hallEditDoubleClick);
}

FReportByPayment::~FReportByPayment()
{
    delete ui;
}

QString FReportByPayment::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Report by payment"))
            .arg(ui->leDateFrom->text())
            .arg(ui->leDateTo->text());
}

void FReportByPayment::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(120, "", tr("Total"))
            .setColumn(150, "", tr("Cash"))
            .setColumn(150, "", tr("Card"))
            .setColumn(150, "", tr("Coupon"))
            .setColumn(150, "", "Փոխանցում")
            .setColumn(150, "", tr("Discount"))
            .setColumn(150, "", "Ավտոկտրոն");
    QString where;
    where += ui->leBranch->fHiddenText.isEmpty() ? "" : QString(" and h.f_branch in (%1) ").arg(ui->leBranch->fHiddenText);
    where += ui->leHall->fHiddenText.isEmpty() ? "" : QString(" and h.f_hall in (%1) ").arg(ui->leHall->fHiddenText);
    QString query = QString("select h.f_dateCash, sum(h.f_total), sum(p.f_cash), sum(p.f_card), "
            "sum(f_coupon), sum(f_debt), sum(f_discount), sum(p.f_couponservice) "
            "from o_header_payment p "
            "left join o_header h on h.f_id=p.f_id "
            "where h.f_state=2 and h.f_dateCash between " + ui->leDateFrom->dateMySql()
            + " and " + ui->leDateTo->dateMySql())
            + where
            + QString("group by 1 "
            "order by 1 ");
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

QWidget *FReportByPayment::firstElement()
{
    return ui->leDateFrom;
}

void FReportByPayment::branchEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}

void FReportByPayment::hallEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_hall, this)) {
        ui->leHall->setText(name);
        ui->leHall->fHiddenText = id;
    }
}
