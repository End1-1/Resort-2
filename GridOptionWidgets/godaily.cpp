#include "godaily.h"
#include "ui_godaily.h"

GODaily::GODaily(const QString &name, QWidget *parent) :
    GOWidget(name, parent),
    ui(new Ui::GODaily)
{
    ui->setupUi(this);
    ui->cbSort->setItemData(0, "m.f_time, m.f_room");
    ui->cbSort->setItemData(1, "m.f_room, m.f_time");
    if (fSettings->value("sort order").toString().isEmpty()) {
        fSettings->setValue("sort order", ui->cbSort->currentData());
    }
    ui->cbSort->setCurrentIndex(ui->cbSort->findData(value("sort order", name)));
}

GODaily::~GODaily()
{
    delete ui;
}

void GODaily::save()
{
    fSettings->setValue("sort order", ui->cbSort->currentData());
}

