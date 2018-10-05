#include "rehouseitem.h"
#include "ui_rehouseitem.h"

REHouseItem::REHouseItem(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::REHouseItem)
{
    ui->setupUi(this);
}

REHouseItem::~REHouseItem()
{
    delete ui;
}
