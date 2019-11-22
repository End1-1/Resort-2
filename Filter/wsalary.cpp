#include "wsalary.h"
#include "ui_wsalary.h"
#include "wreportgrid.h"

WSalary::WSalary(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::WSalary)
{
    ui->setupUi(this);
    ui->rb1->setVisible(false);
    ui->rb2->setVisible(false);
    fReportGrid->setupTabTextAndIcon(tr("Salary"), ":/images/groupreservation.png");
}

WSalary::~WSalary()
{
    delete ui;
}

void WSalary::apply(WReportGrid *rg)
{
    QString table = ui->rb1->isChecked() ? "salary" : "salary2";
    QString query = "select ";
    QList<int> cols;
    QList<double> vals;
    rg->fModel->clearColumns();
    if (ui->chGroupNames->isChecked()) {
        cols.append(1);
        query += " concat(u.f_lastname, ' ', u.f_firstname), sum(f_amount) ";
        rg->fModel->setColumn(300, "", tr("Employee"))
                .setColumn(100, "", tr("Amount"));
    } else {
        cols.append(2);
        query += " f_date, concat(u.f_lastname, ' ', u.f_firstname), f_amount ";
        rg->fModel->setColumn(120, "", tr("Date"))
                .setColumn(300, "", tr("Employee"))
                .setColumn(100, "", tr("Amount"));
    }
    query += "from " + table + " s ";
    query += QString("left join users u on u.f_id=s.f_employee "
            "where s.f_date between %1 and %2 ").arg(ui->deStart->dateMySql()).arg(ui->deEnd->dateMySql());
    if (ui->chGroupNames->isChecked()) {
        query += " group by 1 ";
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

QString WSalary::reportTitle()
{
    return QString("%1, %2 - %3").arg(tr("Salary"));
}

QWidget *WSalary::firstElement()
{
    return ui->deStart;
}
