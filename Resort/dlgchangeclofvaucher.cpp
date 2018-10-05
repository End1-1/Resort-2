#include "dlgchangeclofvaucher.h"
#include "ui_dlgchangeclofvaucher.h"

DlgChangeCLOfVaucher::DlgChangeCLOfVaucher(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgChangeCLOfVaucher)
{
    ui->setupUi(this);
    DWSelectorCityLedger *fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leNewCLCode);
    connect(fDockCL, &DWSelectorCityLedger::cityLedger, [this](CI_CityLedger *ci) {
       dockResponse<CI_CityLedger, CacheCityLedger>(ui->leNewCLCode, ui->leNewCLName, ci);
    });
}

DlgChangeCLOfVaucher::~DlgChangeCLOfVaucher()
{
    delete ui;
}

bool DlgChangeCLOfVaucher::changeCL(const QString &vaucher, const QString &oldCL, const QString &name, const QString &amount)
{
    DlgChangeCLOfVaucher *d = new DlgChangeCLOfVaucher(fPreferences.getDefaultParentForMessage());
    d->ui->leVaucher->setText(vaucher);
    d->ui->leOldCLCode->setText(oldCL);
    CI_CityLedger *ci = CacheCityLedger::instance()->get(oldCL);
    if (ci) {
        d->ui->leOldCLName->setText(ci->fName);
    }
    d->ui->leName->setText(name);
    d->ui->leAmount->setText(amount);
    DatabaseResult dr;
    dr.select(d->fDb, "select * from m_register where f_id=" + ap(d->ui->leVaucher->text()), d->fDbBind);
    if (dr.rowCount() > 0) {
        d->ui->leRoom->setText(dr.value("f_room").toString());
    }
    bool result = d->exec() == QDialog::Accepted;
    delete d;
    return result;
}

void DlgChangeCLOfVaucher::on_btnCancel_clicked()
{
    reject();
}

void DlgChangeCLOfVaucher::on_btnOk_clicked()
{
    if (ui->leNewCLCode->isEmpty()) {
        return;
    }
    if (ui->leRoom->text() == "-" || ui->leRoom->isEmpty()) {
        fDbBind[":f_guest"] = ui->leNewCLName->text();
        fDbBind[":f_paymentComment"] = ui->leNewCLName->text();
    }
    fDbBind[":f_cityLedger"] = ui->leNewCLCode->asInt();
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    accept();
}
