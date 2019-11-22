#include "dlgcheckcard.h"
#include "ui_dlgcheckcard.h"

DlgCheckCard::DlgCheckCard(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgCheckCard)
{
    ui->setupUi(this);
}

DlgCheckCard::~DlgCheckCard()
{
    delete ui;
}

void DlgCheckCard::on_leCode_returnPressed()
{
    ui->teResult->clear();
    DatabaseResult dr;
    fDbBind[":f_card"] = ui->leCode->text();
    dr.select(fDb, "select * from d_car_client where f_card=:f_card", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->teResult->append(dr.value(i, 0).toString());
        ui->teResult->append(dr.value(i, 1).toString());
        ui->teResult->append(dr.value(i, 2).toString());
        ui->teResult->append(dr.value(i, 3).toString());
        ui->teResult->append(dr.value(i, 4).toString());
        ui->teResult->append(dr.value(i, 5).toString());
        ui->teResult->append(dr.value(i, 6).toString());
        ui->teResult->append("---------");
    }
    ui->leCode->clear();
}

void DlgCheckCard::on_btnClose_clicked()
{
    close();
}
