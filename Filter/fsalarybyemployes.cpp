#include "fsalarybyemployes.h"
#include "ui_fsalarybyemployes.h"
#include "tablemodel.h"
#include "wreportgrid.h"

FSalaryByEmployes::FSalaryByEmployes(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FSalaryByEmployes)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(reportTitle(), ":/images/salary.png");
}

FSalaryByEmployes::~FSalaryByEmployes()
{
    delete ui;
}

void FSalaryByEmployes::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Date"))
            .setColumn(200, "", tr("Position"))
            .setColumn(220, "", tr("Name"))
            .setColumn(80, "", tr("Amount"))
            ;
    QString query = QString("select s.f_date, p.f_am as f_position, concat(u.f_lastname, ' ' , u.f_firstname), s.f_amount "
                    "from salary2 s "
                    "left join users u on u.f_id=s.f_employee "
                    "left join users_groups p on p.f_id=u.f_group "
                    "where s.f_date between %1 and %2")
            .arg(ui->deFrom->dateMySql(), ui->deTo->dateMySql());
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> sumCol;
    sumCol.append(3);
    QList<double> sumVal;
    rg->fModel->sumOfColumns(sumCol, sumVal);
    rg->setTblTotalData(sumCol, sumVal);
}

QWidget *FSalaryByEmployes::firstElement()
{
    return ui->deFrom;
}

QString FSalaryByEmployes::reportTitle()
{
    return tr("Salary of employes");
}
