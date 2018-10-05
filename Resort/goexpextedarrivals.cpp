#include "goexpextedarrivals.h"
#include "ui_goexpextedarrivals.h"

GOExpextedArrivals::GOExpextedArrivals(const QString &name, QWidget *parent) :
    GOWidget(name, parent),
    ui(new Ui::GOExpextedArrivals)
{
    ui->setupUi(this);
    ui->cbSort->setItemData(0, "r.f_startDate,  r.f_room, cx.f_name");
    ui->cbSort->setItemData(1, "r.f_startDate, cx.f_name, r.f_room");
    if (fSettings->value("sort order").toString().isEmpty()) {
        fSettings->setValue("sort order", ui->cbSort->currentData());
    }
    ui->cbSort->setCurrentIndex(ui->cbSort->findData(value("sort order", name)));
}

GOExpextedArrivals::~GOExpextedArrivals()
{
    delete ui;
}

void GOExpextedArrivals::save()
{
    fSettings->setValue("sort order", ui->cbSort->currentData());
}
