#include "wendofdayaciton.h"
#include "ui_wendofdayaciton.h"

WEndOfDayAciton::WEndOfDayAciton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WEndOfDayAciton)
{
    ui->setupUi(this);
}

WEndOfDayAciton::~WEndOfDayAciton()
{
    delete ui;
}
