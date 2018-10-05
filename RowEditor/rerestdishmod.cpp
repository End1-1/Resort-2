#include "rerestdishmod.h"
#include "ui_rerestdishmod.h"
#include "cacherestdishmod.h"

RERestDishMod::RERestDishMod(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_DISH_MOD, parent),
    ui(new Ui::RERestDishMod)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leNameEn, "Name, en");
    addWidget(ui->leNameRu, "Name, ru");
    fTable = "r_dish_mod";
    fCacheId = cid_rest_dish_mod;;
}

RERestDishMod::~RERestDishMod()
{
    delete ui;
}

void RERestDishMod::on_btnCancel_clicked()
{
    reject();
}

void RERestDishMod::on_btnOk_clicked()
{
    save();
}
