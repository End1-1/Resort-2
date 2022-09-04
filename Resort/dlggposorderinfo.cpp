#include "dlggposorderinfo.h"
#include "ui_dlggposorderinfo.h"
#include "cacherights.h"
#include "pprintreceipt.h"
#include "dlgdishhistory.h"
#include "databaseresult.h"
#include "cachepaymentmode.h"
#include "cacheinvoiceitem.h"
#include "paymentmode.h"
#include "dlgtracking.h"
#include <QPrintDialog>
#include <QPrinter>

DlgGPOSOrderInfo::DlgGPOSOrderInfo(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgGPOSOrderInfo)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                300, 80, 80, 0, 0, 0, 0, 30);
    ui->btnSave->setVisible(RIGHT(WORKING_USERGROUP, cr__super_correction));
    ui->deDate->setReadOnly(!r__(cr__super_correction));
    ui->lePayment->setEnabled(r__(cr__super_correction));
    connect(ui->lePayment, SIGNAL(customButtonClicked(bool)), this, SLOT(selectPaymentMode(bool)));
    fTrackControl = new TrackControl(TRACK_REST_ORDER);
}

DlgGPOSOrderInfo::~DlgGPOSOrderInfo()
{
    delete ui;
}

void DlgGPOSOrderInfo::setOrder(const QString &id)
{
    ui->leOrder->setText(id);
    setWindowTitle(QString("%1 %2")
                   .arg(tr("Order"))
                   .arg(id));
    fDbBind[":f_header"] = id;
    fDbBind[":f_state"] = DISH_STATE_READY;
    fDb.select("select d.f_" + def_lang + ", o.f_qty, o.f_total, o.f_id, "
               "o.f_adgt, d.f_id, o.f_price, '', if((o.f_complex=0 or (o.f_complex>0 and o.f_complexId>0)),0,1) "
               "from o_dish o "
               "inner join r_dish d on d.f_id=o.f_dish "
               "where o.f_header=:f_header and o.f_state=:f_state",
               fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        ui->tblData->addButton(i, 7, SLOT(showDishHistory(int)), this, QIcon(":/images/update.png"));
    }
    fDbBind[":f_header"] = id;
    fDb.select("select oh.f_dateCash, u.f_username, oh.f_paymentMode, oh.f_paymentModeComment from o_header oh "
               "inner join users u on u.f_id=oh.f_staff "
               "where oh.f_id=:f_header", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_info_tr("No order for this voucher");
        return;
    }
    ui->deDate->setDate(fDbRows.at(0).at(0).toDate());
    ui->leStaff->setText(fDbRows.at(0).at(1).toString());
    ui->lePaymentComment->setText(fDbRows.at(0).at(3).toString());
    CI_PaymentMode *pm = CachePaymentMode::instance()->get(fDbRows.at(0).at(2).toString());
    if (pm) {
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayment, pm);
    }
    countTotal();
}

void DlgGPOSOrderInfo::setVaucher(const QString &id)
{
    ui->leOrder->setText(id);
    fDbBind[":f_id"] = id;
    fDb.select("select f_id from m_register where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        setOrder(fDbRows.at(0).at(0).toString());
    } else {
        setOrder(id);
    }
}

void DlgGPOSOrderInfo::showDishHistory(int tag)
{
    QString dishId = ui->tblData->toString(tag, 3);
    DlgDishHistory *d = new DlgDishHistory(dishId, this);
    d->exec();
    delete d;
}

void DlgGPOSOrderInfo::on_btnOk_clicked()
{
    accept();
}

void DlgGPOSOrderInfo::on_btnSave_clicked()
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        fDbBind[":f_qty"] = ui->tblData->item(i, 1)->data(Qt::EditRole).toDouble();
        fDbBind[":f_total"] = ui->tblData->item(i, 2)->data(Qt::EditRole).toDouble();
        fDb.update("o_dish", fDbBind, where_id(ap(ui->tblData->item(i, 3)->data(Qt::EditRole).toString())));
        fDbBind[":f_id"] = ui->tblData->item(i, 3)->data(Qt::EditRole).toString();
        fDb.select("update o_dish set f_price=:f_total/:f_qty where f_id=:f_id", fDbBind, fDbRows);
    }
    fDbBind[":f_dateCash"] = ui->deDate->date();
    fDbBind[":f_total"] = ui->leTotal->asDouble();
    fDb.update("o_header", fDbBind, where_id(ap(ui->leOrder->text())));
    fDbBind[":f_amountAmd"] = ui->leTotal->asDouble();
    fDbBind[":f_wdate"] = ui->deDate->date();
    fDb.update("m_register", fDbBind, where_id(ap(ui->leOrder->text())));
    message_info_tr("Saved");
}

void DlgGPOSOrderInfo::countTotal()
{
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        total += ui->tblData->item(i, 2)->data(Qt::EditRole).toDouble();
    }
    ui->leTotal->setDouble(total);
}

void DlgGPOSOrderInfo::on_tblData_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    countTotal();
}

void DlgGPOSOrderInfo::on_btnPrint_clicked()
{
    QPrinter p;
    QPrintDialog pd(&p, this);
    if (pd.exec() != QDialog::Accepted) {
        return;
    }
    PPrintReceipt::printOrder(p.printerName(), ui->leOrder->text(), WORKING_USERID);
}

void DlgGPOSOrderInfo::on_btnPrintTax_clicked()
{
    DatabaseResult dbh;
    fDbBind[":f_id"] = ui->leOrder->text();
    dbh.select(fDb, "select f_paymentMode from o_header where f_id=:f_id", fDbBind);
    QString taxDept;
    fDbBind[":f_id"] = ui->leOrder->text();
    fDb.select("select f_itemCode from m_register where f_id=:f_id", fDbBind, fDbRows);
    taxDept = CacheInvoiceItem::instance()->get(fDbRows.at(0).at(0).toString())->fVatDept;
    if (fPreferences.getDb(def_tax_port).toInt() == 0) {
        message_error(tr("Setup tax printer first"));
        return;
    }

}

void DlgGPOSOrderInfo::on_btnTracking_clicked()
{
    DlgTracking::showTracking(TRACK_REST_ORDER, ui->leOrder->text());
}
