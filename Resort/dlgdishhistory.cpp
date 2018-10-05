#include "dlgdishhistory.h"
#include "ui_dlgdishhistory.h"

DlgDishHistory::DlgDishHistory(const QString &dishId, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgDishHistory)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(), 100, 300, 200);
    fDbBind[":f_rec"] = dishId;
    fDb.select("select f_date, f_user, f_info from o_dish_qty where f_rec=:f_rec order by f_date", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
}

DlgDishHistory::~DlgDishHistory()
{
    delete ui;
}
