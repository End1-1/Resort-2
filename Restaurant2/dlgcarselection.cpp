#include "dlgcarselection.h"
#include "ui_dlgcarselection.h"
#include "dlgreservation.h"
#include "rdishcomment.h"
#include "dlgdeptholder.h"
#include "rmessage.h"
#include "cacherestdebtholder.h"
#include "cachecar.h"

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

void DlgCarSelection::selectCar(int &model, QString &govNum, int &costumer)
{
    DlgCarSelection *d = new DlgCarSelection();
    d->ui->leGovNum->setText(govNum);
    CI_RestDebtHolder *ch = CacheRestDebtHolder::instance()->get(costumer);
    if (ch) {
        d->ui->leCostumer->setText(ch->fName);
        d->ui->leCostumer->fHiddenText = ch->fCode;
    }
    d->fModel = model;
    if (model > 0) {
        CI_Car *cc = CacheCar::instance()->get(model);
        if (cc) {
            d->ui->leCar->setText(cc->fName);
        }
    }
    if (d->exec() == QDialog::Accepted) {
        model = d->fModel;
        govNum = d->ui->leGovNum->text().toUpper();
        costumer = d->ui->leCostumer->fHiddenText.toInt();
    }
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
    if (ui->leGovNum->isEmpty()) {
        message_error(tr("Gov number cannot be empty"));
        return;
    }
    if (ui->leCostumer->isEmpty()) {
        message_error(tr("Costumer name cannot be emtpy"));
        return;
    }
    accept();
}

void DlgCarSelection::on_btnNumberSelection_clicked()
{
    QString comment;
    if (RDishComment::getComment(comment, this)) {
        ui->leGovNum->setText(comment.toUpper());
    }
}

void DlgCarSelection::on_btnCostumer_clicked()
{
    int debtId;
    QString debtName;
    if (!DlgDeptHolder::getHolder(debtId, debtName)) {
        return;
    }
    ui->leCostumer->setText(debtName);
    ui->leCostumer->fHiddenText = QString::number(debtId);
}
