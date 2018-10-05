#include "dlgchartdaterange.h"
#include "ui_dlgchartdaterange.h"

DlgChartDateRange::DlgChartDateRange(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgChartDateRange)
{
    ui->setupUi(this);
    ui->deStart->setDate(WORKING_DATE);
}

DlgChartDateRange::~DlgChartDateRange()
{
    delete ui;
}

void DlgChartDateRange::getResult(QDate &date)
{
    date = ui->deStart->date();
}

bool DlgChartDateRange::getDate(QDate &date)
{
    bool result = false;
    DlgChartDateRange *d = new DlgChartDateRange(fPreferences.getDefaultParentForMessage());
    d->ui->deStart->setDate(date);
    if (d->exec() == QDialog::Accepted) {
        result = true;
        d->getResult(date);
    }
    delete d;
    return result;
}
