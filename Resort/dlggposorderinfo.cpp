#include "dlggposorderinfo.h"
#include <QAbstractItemView>
#include <QInputDialog>
#include <QPrintDialog>
#include <QPrinter>
#include "baseorder.h"
#include "cachepaymentmode.h"
#include "cacherights.h"
#include "database2.h"
#include "databaseresult.h"
#include "dlgdishhistory.h"
#include "dlggetidname.h"
#include "dlgtracking.h"
#include "paymentmode.h"
#include "storeoutput.h"
#include "ui_dlggposorderinfo.h"

DlgGPOSOrderInfo::DlgGPOSOrderInfo(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgGPOSOrderInfo),
    fReadOnly(false)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                300, 80, 80, 0, 0, 0, 0, 30, 0, 30, 30);
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
               "o.f_adgt, d.f_id, o.f_price, '', if((o.f_complex=0 or (o.f_complex>0 and o.f_complexId>0)),0,1), '', '' "
               "from o_dish o "
               "inner join r_dish d on d.f_id=o.f_dish "
               "where o.f_header=:f_header and o.f_state=:f_state",
               fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
    addRowButtons();

    fDbBind[":f_header"] = id;
    fDb.select("select oh.f_dateCash, u.f_username, oh.f_paymentMode, oh.f_paymentModeComment, oh.f_tax from o_header oh "
               "inner join users u on u.f_id=oh.f_staff "
               "where oh.f_id=:f_header", fDbBind, fDbRows);

    if(fDbRows.count() == 0) {
        message_info_tr("No order for this voucher");
        return;
    }

    ui->deDate->setDate(fDbRows.at(0).at(0).toDate());
    ui->leStaff->setText(fDbRows.at(0).at(1).toString());
    ui->lePaymentComment->setText(fDbRows.at(0).at(3).toString());
    ui->leFiscal->setText(fDbRows.at(0).at(4).toString());
    CI_PaymentMode *pm = CachePaymentMode::instance()->get(fDbRows.at(0).at(2).toString());

    if(pm) {
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayment, pm);
    }

    countTotal();
}

void DlgGPOSOrderInfo::setVaucher(const QString &id)
{
    ui->leOrder->setText(id);
    fDbBind[":f_id"] = id;
    fDb.select("select f_id from m_register where f_id=:f_id", fDbBind, fDbRows);

    if(fDbRows.count() > 0) {
        setOrder(fDbRows.at(0).at(0).toString());
    } else {
        setOrder(id);
    }
}

void DlgGPOSOrderInfo::setReadOnly(bool readOnly)
{
    fReadOnly = readOnly;
    ui->btnSetFiscalNumber->setVisible(!readOnly);
    ui->btnTracking->setVisible(!readOnly);
    ui->lePayment->setReadOnly(true);
    ui->tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void DlgGPOSOrderInfo::addRowButtons()
{
    if(fReadOnly) {
        return;
    }

    for(int i = 0; i < ui->tblData->rowCount(); i++) {
        ui->tblData->addButton(i, 7, SLOT(showDishHistory(int)), this, QIcon(":/images/update.png"));
        ui->tblData->addButton(i, 9, SLOT(deleteDishRow(int)), this, QIcon(":/images/garbage.png"));
        ui->tblData->addButton(i, 10, SLOT(replaceDishRow(int)), this, QIcon(":/images/edit.png"));
    }
}

void DlgGPOSOrderInfo::showDishHistory(int tag)
{
    QString dishId = ui->tblData->toString(tag, 3);
    DlgDishHistory *d = new DlgDishHistory(dishId, this);
    d->exec();
    delete d;
}

void DlgGPOSOrderInfo::deleteDishRow(int row)
{
    if(row < 0 || row >= ui->tblData->rowCount()) {
        return;
    }

    if(message_confirm_tr("Confirm to delete selected row") != QDialog::Accepted) {
        return;
    }

    const int orderId = ui->leOrder->text().toInt();
    const int recId = ui->tblData->toInt(row, 3);

    if(orderId == 0 || recId == 0) {
        message_error(tr("Invalid order or dish row"));
        return;
    }

    fDb.fDb.transaction();
    fDbBind[":f_id"] = recId;
    fDb.select("delete from o_dish where f_id=:f_id", fDbBind, fDbRows);
    ui->tblData->removeRow(row);
    updateOrderTotal();
    recalculateStoreForOrder();
    fDb.fDb.commit();
    addRowButtons();
    message_info_tr("Saved");
}

void DlgGPOSOrderInfo::replaceDishRow(int row)
{
    if(row < 0 || row >= ui->tblData->rowCount()) {
        return;
    }

    QString dishId;
    QString dishName;

    if(!DlgGetIDName::get(dishId, dishName, idname_dish_defstore, this)) {
        return;
    }

    const int orderId = ui->leOrder->text().toInt();
    const int recId = ui->tblData->toInt(row, 3);
    const int newDishId = dishId.toInt();
    const int store = storeForDish(newDishId, orderId);

    if(orderId == 0 || recId == 0 || newDishId == 0) {
        message_error(tr("Invalid order or dish"));
        return;
    }

    if(store == 0) {
        message_error(tr("Store is not defined for selected dish"));
        return;
    }

    fDb.fDb.transaction();
    fDbBind[":f_dish"] = newDishId;
    fDbBind[":f_store"] = store;
    fDb.update("o_dish", fDbBind, where_id(recId));
    recalculateStoreForOrder();
    fDb.fDb.commit();

    ui->tblData->item(row, 0)->setData(Qt::EditRole, dishName);
    ui->tblData->item(row, 5)->setData(Qt::EditRole, newDishId);
    message_info_tr("Saved");
}

int DlgGPOSOrderInfo::storeForDish(int dishId, int orderId)
{
    fDbBind[":f_dish"] = dishId;
    fDbBind[":f_header"] = orderId;
    fDb.select("select coalesce(bs.f_alias, d.f_defstore) "
               "from r_dish d "
               "inner join o_header h on h.f_id=:f_header "
               "left join r_branch_storemap bs on bs.f_store=d.f_defstore and bs.f_branch=h.f_branch "
               "where d.f_id=:f_dish",
               fDbBind, fDbRows);

    if(fDbRows.count() == 0) {
        return 0;
    }

    return fDbRows.at(0).at(0).toInt();
}

void DlgGPOSOrderInfo::updateOrderTotal()
{
    countTotal();
    fDbBind[":f_total"] = ui->leTotal->asDouble();
    fDb.update("o_header", fDbBind, where_id(ap(ui->leOrder->text())));
    fDbBind[":f_amountAmd"] = ui->leTotal->asDouble();
    fDb.update("m_register", fDbBind, where_id(ap(ui->leOrder->text())));
}

void DlgGPOSOrderInfo::recalculateStoreForOrder()
{
    const int orderId = ui->leOrder->text().toInt();

    if(orderId == 0) {
        message_error(tr("Invalid order id: 0"));
        return;
    }

    StoreOutput so(fDb, 0);
    so.rollbackSale(fDb, orderId);
    BaseOrder bo(0);
    bo.calculateOutput(fDb, orderId);
}

void DlgGPOSOrderInfo::on_btnOk_clicked()
{
    accept();
}

void DlgGPOSOrderInfo::on_btnSave_clicked()
{
    for(int i = 0; i < ui->tblData->rowCount(); i++) {
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

    for(int i = 0; i < ui->tblData->rowCount(); i++) {
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

    if(pd.exec() != QDialog::Accepted) {
        return;
    }
}

void DlgGPOSOrderInfo::on_btnTracking_clicked()
{
    DlgTracking::showTracking(TRACK_REST_ORDER, ui->leOrder->text());
}

void DlgGPOSOrderInfo::on_btnSetFiscalNumber_clicked()
{
    bool ok;
    int num = QInputDialog::getInt(this, tr("Fiscal"), "", 0, 0, 2147483647, 1, &ok);

    if(!ok) {
        return;
    }

    Database2 db;

    if(!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }

    ui->leFiscal->setText(QString::number(num));
    db[":f_id"] = ui->leOrder->text().toInt();
    db[":f_tax"] = ui->leFiscal->text().toInt();
    db.exec("update o_header set f_tax=:f_tax where f_id=:f_id");
}
