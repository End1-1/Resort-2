#include "dlgorders.h"
#include "ui_dlgorders.h"
#include "rmessage.h"
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
