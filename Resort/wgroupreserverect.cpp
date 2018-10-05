#include "wgroupreserverect.h"
#include "ui_wgroupreserverect.h"

WGroupReserveRect::WGroupReserveRect(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WGroupReserveRect)
{
    ui->setupUi(this);
}

WGroupReserveRect::~WGroupReserveRect()
{
    delete ui;
}

void WGroupReserveRect::setRoomName(const QString &code, const QString &name)
{
    fCode = code;
    ui->lbRoom->setText(name);
}

bool WGroupReserveRect::checked()
{
    return ui->chSelected->isChecked();
}

void WGroupReserveRect::uncheck()
{
    ui->chSelected->setChecked(false);
    on_chSelected_clicked(false);
}

QString WGroupReserveRect::code()
{
    return fCode;
}

void WGroupReserveRect::setEnabled(bool v)
{
    BaseWidget::setEnabled(v);
    if (!v) {
        ui->chSelected->setChecked(false);
        setStyleSheet("background-color:grey ");
    } else {
        on_chSelected_clicked(ui->chSelected->isChecked());
    }
}

void WGroupReserveRect::on_chSelected_clicked(bool checked)
{
    if (checked) {
        setStyleSheet("background-color:yellow");
    } else {
        setStyleSheet("background-color:white");
    }
    emit checkClicked(checked);
}
