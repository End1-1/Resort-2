#include "rerestmenupart.h"
#include "ui_rerestmenupart.h"
#include "cacherestdishpart.h"

RERestMenuPart::RERestMenuPart(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_MENU_PART, parent),
    ui(new Ui::RERestMenuPart)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leNameEn, "Name, en");
    addWidget(ui->leNameRu, "Name, ru");
    fTable = "r_dish_part";
    fCacheId = cid_rest_dish_part;
}

RERestMenuPart::~RERestMenuPart()
{
    delete ui;
}

void RERestMenuPart::on_btnCancel_clicked()
{
    reject();
}

void RERestMenuPart::on_btnOk_clicked()
{
    save();
}
