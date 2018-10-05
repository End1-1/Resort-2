#include "dlgorders.h"
#include "ui_dlgorders.h"
#include "rmessage.h"
#include "printtax.h"
#include "databaseresult.h"
#include "hall.h"
#include "paymentmode.h"
#include "cacherights.h"
#include "cacheinvoiceitem.h"
#include <QScrollBar>
#include <QInputDialog>

DlgOrders::DlgOrders(int staff, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgOrders)
{
    ui->setupUi(this);
    //ui->btnByNumber->setVisible(r__(cr__super_correction));
    fStaffId = staff;
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                100, 200, 100, 400, 100, 50, 0);
    fDbBind[":f_dateCash"] = WORKING_DATE;
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDb.select("select oh.f_id, h.f_name, t.f_name, concat(u.f_firstName, ' ', u.f_lastName),"
               "oh.f_total, oh.f_tax, oh.f_paymentMode, '' "
               "from o_header oh "
               "left join r_hall h on h.f_id=oh.f_hall "
               "left join r_table t on t.f_id=oh.f_table "
               "left join users u on u.f_id=oh.f_staff "
               "where oh.f_dateCash=:f_dateCash and oh.f_state=:f_state", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
    showFullScreen();
}

DlgOrders::~DlgOrders()
{
    delete ui;
}

void DlgOrders::on_btnPrintTax_clicked()
{
    int row = ui->tblData->currentRow();
    if (row < 0) {
        message_error(tr("Nothing is selected"));
        return;
    }
    DatabaseResult db;
    fDbBind[":f_id"] = ui->tblData->item(row, 0)->data(Qt::EditRole).toString();
    db.select(fDb, "select f_hall from o_header where f_id=:f_id", fDbBind);
    QString vatDept = Hall::fHallMap[db.value("f_hall").toInt()]->fVatDept;

    PrintTax *pt = new PrintTax(this);

    fDbBind[":f_header"] = ui->tblData->item(row, 0)->data(Qt::EditRole).toString();
    fDbBind[":f_state"] = DISH_STATE_READY;
    fDb.select("select d.f_id, d.f_am, d.f_adgt, od.f_qty, od.f_price "
               "from o_dish od "
               "inner join r_dish d on d.f_id=od.f_dish "
               "where od.f_header=:f_header and od.f_state=:f_state ", fDbBind, fDbRows);
    foreach_rows {
        pt->fDept.append(vatDept);
        pt->fRecList.append(it->at(0).toString());
        pt->fCodeList.append(it->at(0).toString());
        pt->fNameList.append(it->at(1).toString());
        pt->fPriceList.append(it->at(4).toString());
        pt->fQtyList.append(it->at(3).toString());
        pt->fAdgCode.append(it->at(2).toString());
        pt->fTaxNameList.append(it->at(1).toString());        

        fDbBind[":f_vaucher"] = ui->tblData->item(row, 0)->data(Qt::EditRole).toInt() * -1;
        fDbBind[":f_invoice"] = 0;
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_name"] = it->at(1).toString();
        fDbBind[":f_amountCash"] = ui->tblData->item(row, 6)->data(Qt::EditRole).toInt() == PAYMENT_CASH ? ui->tblData->item(row, 4)->data(Qt::EditRole).toDouble() : 0;
        fDbBind[":f_amountCard"] = ui->tblData->item(row, 6)->data(Qt::EditRole).toInt() == PAYMENT_CASH ? 0 : ui->tblData->item(row, 4)->data(Qt::EditRole).toDouble();
        fDbBind[":f_amountPrepaid"] = 0;
        fDbBind[":f_user"] = fStaffId;
        fDb.insert("m_tax_history", fDbBind);
    }

    pt->fInvoice = "R" + ui->tblData->item(row, 0)->data(Qt::EditRole).toString();
    pt->build();
    switch (ui->tblData->item(row, 6)->data(Qt::EditRole).toInt()) {
    case PAYMENT_CASH:
        pt->fAmountCash = float_str(ui->tblData->item(row, 4)->data(Qt::EditRole).toDouble(), 2);
        pt->fAmountCard = "0";
        break;
    case PAYMENT_CARD:
        pt->fAmountCard = float_str(ui->tblData->item(row, 4)->data(Qt::EditRole).toDouble(), 2);
        pt->fAmountCash = "0";
        break;
    default:
        message_error("Cannot print tax for this order, because payment mode is not card or cash. Bye!");
        delete pt;
        return;
    }
    pt->print();
    delete pt;

}

void DlgOrders::on_btnCancel_clicked()
{
    reject();
}

void DlgOrders::on_btnDown_clicked()
{
    ui->tblData->verticalScrollBar()->setValue(ui->tblData->verticalScrollBar()->value() + 6);
}

void DlgOrders::on_btnUp_clicked()
{
    ui->tblData->verticalScrollBar()->setValue(ui->tblData->verticalScrollBar()->value() - 6);
}

void DlgOrders::on_btnByNumber_clicked()
{
    QString num;
    num = QInputDialog::getText(this, tr("Enter number"), tr("Order id"));
    if (num.isEmpty()) {
        return;
    }
    num = num.toUpper();
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDbBind[":f_id"] = num;
    fDb.select("select oh.f_id, h.f_name, t.f_name, concat(u.f_firstName, ' ', u.f_lastName),"
               "oh.f_total, oh.f_tax, oh.f_paymentMode, '' "
               "from o_header oh "
               "left join r_hall h on h.f_id=oh.f_hall "
               "left join r_table t on t.f_id=oh.f_table "
               "left join users u on u.f_id=oh.f_staff "
               "where oh.f_id=:f_id and oh.f_state=:f_state ", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
}
