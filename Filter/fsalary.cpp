#include "fsalary.h"
#include "ui_fsalary.h"
#include "wreportgrid.h"

FSalary::FSalary(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FSalary)
{
    ui->setupUi(this);
}

FSalary::~FSalary()
{
    delete ui;
}

QString FSalary::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Employes"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FSalary::firstElement()
{
    return ui->deStart;
}

void FSalary::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(300, "", tr("Employee"));

}
