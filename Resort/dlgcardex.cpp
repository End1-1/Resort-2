#include "dlgcardex.h"
#include "ui_dlgcardex.h"
#include "cachecardex.h"

DlgCardex::DlgCardex(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_PARTNERS, parent),
    ui(new Ui::DlgCardex)
{
    ui->setupUi(this);
    ui->leCommision->setValidator(new QDoubleValidator(0, 999, 2));
    ui->leCLCode->setValidator(new QIntValidator());
    ui->leVATModeCode->setValidator(new QIntValidator());
    ui->btnRemove->setVisible(r__(cr__super_correction));
    addWidget(ui->leCode, "Code");
    addWidget(ui->leCardexGroupName, "Group");
    addWidget(ui->leCardexGroupCode, "");
    addWidget(ui->leCardex, "Cardex");
    addWidget(ui->leCLCode, "CityLedger");
    addWidget(ui->leName, "Name");
    addWidget(ui->leAddr1, "Addr1");
    addWidget(ui->leAddr2, "Addr2");
    addWidget(ui->leContact, "Contact");
    addWidget(ui->leContactPos, "Position");
    addWidget(ui->leEmail, "Email");
    addWidget(ui->leTel, "Tel");
    addWidget(ui->leFax, "Fax");
    addWidget(ui->deValidFrom, "Valid from");
    addWidget(ui->deValidTo, "Valid to");
    addWidget(ui->leCommision, "Commission");
    addWidget(ui->leVATModeCode, "");
    addWidget(ui->leVATModeName, "VAT");
    addWidget(ui->chBookingRequire, "Require booking");
    addWidget(ui->leExtra1, "Extra1");
    addWidget(ui->leExtra2, "Extra2");
    fTable = "f_cardex";
    fCacheId = cid_cardex;
    fDockVAT = new DWSelectorVATMode(this);
    fDockVAT->configure();
    connect(fDockVAT, &DWSelectorVATMode::vatMode, [this](CI_VATMode *v) {
       dockResponse<CI_VATMode, CacheVatMode>(ui->leVATModeCode, ui->leVATModeName, v);
    });
    fDockGroup = new DWSelectorCardexGroup(this);
    fDockGroup->configure();
    fDockGroup->setSelector(ui->leCardexGroupCode);
    connect(fDockGroup, SIGNAL(cardexGroup(CI_CardexGroup*)), this, SLOT(cardexGroup(CI_CardexGroup*)));
    CI_VATMode *cv = CacheVatMode::instance()->get(VAT_INCLUDED);
    ui->leVATModeCode->setText(cv->fCode);
    ui->leVATModeName->setText(cv->fName);
}

DlgCardex::~DlgCardex()
{
    delete ui;
}

bool DlgCardex::isDataCorrect()
{
    fDbBind[":f_cardex"] = ui->leCardex->text();
    fDb.select("select f_id from f_cardex where f_cardex=:f_cardex", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        if (fDbRows.at(0).at(0).toInt() != ui->leCode->asInt()) {
            fDataErrors.append(tr("This cardex already exists"));
        }
    }
    if (ui->leVATModeCode->asInt() == 0) {
        fDataErrors.append(tr(""));
    }
    return fDataErrors.count() == 0;
}

void DlgCardex::setValues()
{
    RowEditorDialog::setValues();
    if (isNew) {
        CI_VATMode *cv = CacheVatMode::instance()->get(VAT_INCLUDED);
        ui->leVATModeCode->setText(cv->fCode);
        ui->leVATModeName->setText(cv->fName);
        ui->deValidTo->setDate(QDate::currentDate().addDays(365));
    }
}

void DlgCardex::cardexGroup(CI_CardexGroup *c)
{
    dockResponse<CI_CardexGroup, CacheCardexGroup>(ui->leCardexGroupCode, ui->leCardexGroupName, c);
}

void DlgCardex::on_btnCancel_clicked()
{
    reject();
}

void DlgCardex::on_btnOk_clicked()
{
    if (ui->leCardexGroupCode->isEmpty()) {
        message_error_tr("Cardex group is not selected");
        return;
    }
    if (ui->leVATModeCode->asInt() == 0) {
        message_error_tr("VAT mode is not selected");
        return;
    }
    if (ui->chCityLedger->isChecked()) {
        fDbBind[":f_name"] = ui->leName->text();
        fDbBind[":f_address"] = ui->leAddr1->text();
        fDbBind[":f_phone"] = ui->leTel->text();
        fDbBind[":f_email"] = ui->leEmail->text();
        fDbBind[":f_extra1"] = ui->leExtra1->text();
        fDbBind[":f_extra2"] = ui->leExtra2->text();
        ui->leCLCode->setInt(fDb.insert("f_city_ledger", fDbBind));
    }
    int groupMax = 0;
    if (isNew) {
        fDbBind[":f_id"] = ui->leCardexGroupCode->asInt();
        fDb.select("select f_max from f_cardex_group where f_id=:f_id", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            groupMax = fDbRows.at(0).at(0).toInt();
            ui->leCardex->setText(QString("%1%2").arg(ui->leCardexGroupName->text()).arg(groupMax, 3, 10, QChar('0')));
        }
        if (groupMax > 0) {
            fDbBind[":f_max"]= groupMax + 1;
            fDb.update("f_cardex_group", fDbBind, where_id(ui->leCardexGroupCode->asInt()));
            fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
            fDb.update("f_cardex", fDbBind, where_id(ui->leCode->asInt()));
        } else {
            message_error_tr("Application error. Contact to developer. Message: cardex group max = 0");
        }
    }
    save();
    BroadcastThread::cmdRefreshCache(cid_city_ledger, ui->leCLCode->text());
    BroadcastThread::cmdRefreshCache(cid_cardex, ui->leCode->text());
}

void DlgCardex::on_btnRemove_clicked()
{
    if (message_confirm_tr("Confirm to delete") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_cardex"] = ui->leCardex->text();
    fDb.select("select count(f_id) from f_reservation where f_cardex=:f_cardex", fDbBind, fDbRows);
    if (fDbRows.at(0).at(0).toInt() > 0) {
        message_info_tr("This cardex in use and cannot be deleted");
        return;
    }
    fDbBind[":f_cardex"] = ui->leCardex->text();
    fDb.select("delete from f_cardex where f_cardex=:f_cardex", fDbBind, fDbRows);
    message_info_tr("Deleted");
    accept();
}
