#include "recityledger.h"
#include "ui_recityledger.h"
#include "cachecityledger.h"
#include "cacherights.h"
#include "dlgclinitialbalance.h"

RECityLedger::RECityLedger(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_CITY_LEDGER, parent),
    ui(new Ui::RECityLedger)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    addWidget(ui->leAddress, "Address");
    addWidget(ui->lePhone, "Phone");
    addWidget(ui->leEmail, "Email");
    addWidget(ui->leExtra1, "Extra1");
    addWidget(ui->leExtra2, "Extra2");
    fCacheId = cid_city_ledger;
    fTable = "f_city_ledger";
    ui->btnRemove->setVisible(r__(cr__super_correction));
    ui->btnInitialBalance->setVisible(r__(cr__super_correction));
}

RECityLedger::~RECityLedger()
{
    delete ui;
}

void RECityLedger::on_btnCancel_clicked()
{
    reject();
}

void RECityLedger::on_btnOk_clicked()
{
    save();
}

void RECityLedger::on_btnRemove_clicked()
{
    if (message_yesnocancel(tr("Are you sure to delete this city ledger record?")) != RESULT_YES) {
        return;
    }
    fDbBind[":f_cityLedger"] = ui->leCode->asInt();
    fDb.select("select * from m_register where f_cityLedger=:f_cityLedger", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        message_error_tr("Cannot remove city ledger. Records found in vouchers history.");
        return;
    }
    fDbBind[":f_cityLedger"] = ui->leCode->asInt();
    fDb.select("delete from f_city_ledger where f_id=:f_cityLedger", fDbBind, fDbRows);
    fTrackControl->insert("Remove city ledger", QString("%1/%2/%3/%4")
                              .arg(ui->leCode->asInt())
                              .arg(ui->leName->text())
                              .arg(ui->leAddress->text())
                              .arg(ui->lePhone->text()), "");
    clearWidgets();
    accept();
    message_info_tr("The city ledger removed from databases, refresh current report");
}

void RECityLedger::on_btnInitialBalance_clicked()
{
    DlgCLInitialBalance *d = new DlgCLInitialBalance(ui->leCode->asInt(), this);
    d->exec();
    delete d;
    reject();
}
