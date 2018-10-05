#include "wdate2.h"
#include "ui_wdate2.h"

WDate2::WDate2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WDate2)
{
    ui->setupUi(this);
    setMaximumSize(ui->label->width() + ui->label_2->width() + ui->deEnd->width()
                   + ui->deStart->width() + ui->btnLeft->width() + ui->btnRight->width() + 10, 50);
}

WDate2::~WDate2()
{
    delete ui;
}

QDate WDate2::d1()
{
    return ui->deStart->date();
}

QDate WDate2::d2()
{
    return ui->deEnd->date();
}

QString WDate2::ds1()
{
    return ui->deStart->dateMySql();
}

QString WDate2::ds2()
{
    return ui->deEnd->dateMySql();
}

QString WDate2::dss1()
{
    return ui->deStart->text();
}

QString WDate2::dss2()
{
    return ui->deEnd->text();
}

QWidget *WDate2::fw()
{
    return ui->deStart;
}

void WDate2::setD1(const QDate &d1)
{
    ui->deStart->setDate(d1);
}

void WDate2::setD2(const QDate &d2)
{
    ui->deEnd->setDate(d2);
}

void WDate2::changeDate(int day)
{
    ui->deStart->setDate(ui->deStart->date().addDays(day));
    ui->deEnd->setDate(ui->deEnd->date().addDays(day));
    emit changed();
}

void WDate2::on_btnLeft_clicked()
{
    changeDate(-1);
}

void WDate2::on_btnRight_clicked()
{
    changeDate(1);
}
