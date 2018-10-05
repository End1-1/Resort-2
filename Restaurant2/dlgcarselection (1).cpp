#include "dlgcarselection.h"
#include "ui_dlgcarselection.h"
#include "dlgreservation.h"
#include "rdishcomment.h"

DlgCarSelection::DlgCarSelection(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgCarSelection)
{
    ui->setupUi(this);
}

DlgCarSelection::~DlgCarSelection()
{
    delete ui;
}

void DlgCarSelection::selectCar(int &model, QString &govNum)
{
    DlgCarSelection *d = new DlgCarSelection();
    d->exec();
    model = d->fModel;
    govNum = d->ui->leGovNum->text().toUpper();
    delete d;
}

void DlgCarSelection::on_btnModelSelection_clicked()
{
    DlgReservation *d = new DlgReservation();
    d->loadCars();
    d->exec();
    fModel = d->fItemCode;
    ui->leCar->setText(d->fReservationId);
    fDbBind[":f_id"] = fModel;
    DatabaseResult dr;
    dr.select(fDb, "select concat(f_model, ' ', f_class) as car from d_car_model where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        ui->leCar->setText(dr.value("car").toString());
    }
    delete d;
}

void DlgCarSelection::on_btnCancel_clicked()
{
    reject();
}

void DlgCarSelection::on_btnSave_clicked()
{
    accept();
}

void DlgCarSelection::on_btnNumberSelection_clicked()
{
    QString comment;
    if (RDishComment::getComment(comment, this)) {
        ui->leGovNum->setText(comment.toUpper());
    }
}
