#include "fsalesbycar.h"
#include "ui_fsalesbycar.h"
#include "wreportgrid.h"
#include "dlggetidname.h"

FSalesByCar::FSalesByCar(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FSalesByCar)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Sales by cars"), ":/images/car.png");
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FSalesByCar::branchEditDoubleClick);
    connect(ui->leHall, &EQLineEdit::customButtonClicked, this, &FSalesByCar::hallEditDoubleClick);
}

FSalesByCar::~FSalesByCar()
{
    delete ui;
}

QString FSalesByCar::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Sales by cars"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FSalesByCar::firstElement()
{
    return ui->deStart;
}

void FSalesByCar::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Gov num"))
            .setColumn(120, "", tr("Qty"))
            .setColumn(120, "", tr("Amount"));
    QString where;
    where += ui->leBranch->fHiddenText.isEmpty() ? "" : QString(" and o.f_branch in (%1) ").arg(ui->leBranch->fHiddenText);
    where += ui->leHall->fHiddenText.isEmpty() ? "" : QString(" and o.f_hall in (%1) ").arg(ui->leHall->fHiddenText);
    QString sql = "select c.f_govNumber, count(o.f_id), sum(o.f_total) "
            "from o_header o "
            "left join o_car c on c.f_order=o.f_id "
            "where o.f_dateCash between :date1 and :date2 and o.f_state=2 "
            + where
            + "group by 1";
    sql.replace(":date1", ui->deStart->dateMySql());
    sql.replace(":date2", ui->deEnd->dateMySql());
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 1 << 2;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FSalesByCar::branchEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}

void FSalesByCar::hallEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_hall, this)) {
        ui->leHall->setText(name);
        ui->leHall->fHiddenText = id;
    }
}
