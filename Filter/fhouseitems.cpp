#include "fhouseitems.h"
#include "ui_fhouseitems.h"
#include "wreportgrid.h"

FHouseItems::FHouseItems(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FHouseItems)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("House inventory"), ":/images/inventory.png");
}

FHouseItems::~FHouseItems()
{
    delete ui;
}

QString FHouseItems::reportTitle()
{
    return QString("%1").arg(tr("House inventory"));
}

QWidget *FHouseItems::firstElement()
{
    return 0;
}

void FHouseItems::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(80, "", tr("Code"))
            .setColumn(300, "", tr("Name"));
    QString sql = "select f_id, f_name from f_house_item";
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
}
