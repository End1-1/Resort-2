#include "rerestmenunames.h"
#include "ui_rerestmenunames.h"
#include "cacherestmenu.h"

RERestMenuNames::RERestMenuNames(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_MENU_NAME, parent),
    ui(new Ui::RERestMenuNames)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leNameEn, "Name, en");
    addWidget(ui->leNameRu, "Name, ru");
    fTable = "r_menu_names";
    fCacheId = cid_rest_menu;
}

RERestMenuNames::~RERestMenuNames()
{
    delete ui;
}

void RERestMenuNames::on_btnCancel_clicked()
{
    reject();
}

void RERestMenuNames::on_btnOk_clicked()
{
    save();
}
