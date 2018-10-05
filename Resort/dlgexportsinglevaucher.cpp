#include "dlgexportsinglevaucher.h"
#include "ui_dlgexportsinglevaucher.h"
#include "dlgremotinvoices.h"
#include "vauchers.h"

DlgExportSingleVaucher::DlgExportSingleVaucher(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgExportSingleVaucher)
{
    ui->setupUi(this);
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return;
    }
    fSDb.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!fSDb.open()) {
        message_error_tr("Cannot connect to second database");
        return;
    }
    connect(ui->leInvoice, SIGNAL(customButtonClicked(bool)), this, SLOT(showInvoices(bool)));
}

DlgExportSingleVaucher::~DlgExportSingleVaucher()
{
    delete ui;
}

void DlgExportSingleVaucher::setupVaucher(const QString &id)
{
    fVaucherId = id;
    fDbBind[":f_id"] = id;
    fVaucher.select(fDb, "select * from m_register where f_id=:f_id", fDbBind);
    ui->leFinalName->setText(fVaucher.value(0, "f_finalName").toString());
    ui->leAmount->setText(float_str(fVaucher.value(0, "f_amountAmd").toDouble(), 0));
    ui->deDate->setDate(fVaucher.value(0, "f_wdate").toDate());
    ui->teTime->setTime(fVaucher.value(0, "f_time").toTime());
}

int DlgExportSingleVaucher::exportSingleVaucher(const QString &id)
{
    DlgExportSingleVaucher *d = new DlgExportSingleVaucher(fPreferences.getDefaultParentForMessage());
    d->setupVaucher(id);
    int result = d->exec();
    delete d;
    return result;
}

void DlgExportSingleVaucher::showInvoices(bool v)
{
    Q_UNUSED(v)
    DlgRemotInvoices *d = new DlgRemotInvoices(this);
    if (d->exec() == QDialog::Accepted) {
        ui->leInvoice->setText(d->fResult);
        on_leInvoice_returnPressed();
    }
    delete d;
}

void DlgExportSingleVaucher::on_btnCancel_clicked()
{
    reject();
}

void DlgExportSingleVaucher::on_btnOk_clicked()
{
    fSDb.fDb.transaction();
    fVaucher.getBindValues(0, fDbBind);
    bool ps = fDbBind[":f_source"].toString() == VAUCHER_POINT_SALE_N;
    bool pe = fDbBind[":f_source"].toString() == VAUCHER_EVENT_N;
    QString id = fDbBind[":f_id"].toString();
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_wdate"] = ui->deDate->date();
    fDbBind[":f_rdate"] = ui->deDate->date();
    fDbBind[":f_time"] = ui->teTime->time();
    fDbBind[":f_finalName"] = ui->leFinalName->text();
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    if (!fSDb.insertWithoutId("m_register", fDbBind)) {
        message_error(fSDb.fLastError);
        fSDb.fDb.rollback();
        return;
    }
    if (ps) {
        DatabaseResult dr;
        dr.select(fDb, "select * from o_header where f_id=" + ap(id), fDbBind);
        dr.getBindValues(0, fDbBind);
        fDbBind[":f_dateCash"] = ui->deDate->date();
        fDbBind[":f_dateOpen"] = ui->deDate->date();
        fDbBind[":f_dateClose"] = ui->deDate->date();
        fDbBind[":f_roomComment"] = QString("%1, %2").arg(ui->leRoom->text())
                .arg(ui->leGuest->text());
        fDbBind[":f_paymentModeComment"] = fDbBind[":f_roomComment"];
        if (!fSDb.insertWithoutId("o_header", fDbBind)) {
            message_error(fSDb.fLastError);
            fSDb.fDb.rollback();
            return;
        }
        dr.select(fDb, "select * from o_dish where f_header=" + ap(id), fDbBind);
        for (int i = 0; i < dr.rowCount(); i++) {
            dr.getBindValues(i, fDbBind);
            if (!fSDb.insertWithoutId("o_dish", fDbBind)) {
                message_error(fSDb.fLastError);
                fSDb.fDb.rollback();
                return;
            }
        }
    }
    if (pe) {
        DatabaseResult dr;
        dr.select(fDb, "select * from o_event where f_id=" + ap(id), fDbBind);
        for (int i = 0; i < dr.rowCount(); i++) {
            dr.getBindValues(i, fDbBind);
            fDbBind[":f_wdate"] = ui->deDate->date();
            fDbBind[":f_rdate"] = ui->deDate->date();
            fDbBind[":f_time"] = ui->teTime->time();
            if (!fSDb.insertWithoutId("o_event", fDbBind)) {
                message_error(fSDb.fLastError);
                fSDb.fDb.rollback();
                return;
            }
        }
    }
    fDbBind[":p"] = 1;
    fDb.update("m_register", fDbBind, where_id(ap(fVaucherId)));
    fSDb.fDb.commit();
    message_info_tr("Exported successfully");
    accept();
}

void DlgExportSingleVaucher::on_leInvoice_returnPressed()
{
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fSDb.select("select r.f_room, concat(g.f_firstName, ' ', g.f_lastName) "
                "from f_reservation r "
                "left join f_guests g on g.f_id=r.f_guest "
                "where r.f_invoice=:f_invoice", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Invalid invoice number");
        return;
    }
    ui->leRoom->setText(fDbRows.at(0).at(0).toString());
    ui->leGuest->setText(fDbRows.at(0).at(1).toString());
}
