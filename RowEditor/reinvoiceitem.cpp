#include "reinvoiceitem.h"
#include "ui_reinvoiceitem.h"
#include "cacheinvoiceitem.h"
#include "cachetaxmap.h"

REInvoiceItem::REInvoiceItem(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_INVOICE_ITEM, parent),
    ui(new Ui::REInvoiceItem)
{
    ui->setupUi(this);
    ui->lePrice->setValidator(new QDoubleValidator(0, 99999999, 2));
    addWidget(ui->leCode, "Code");
    addWidget(ui->leVaucher, "Voucher");
    addWidget(ui->leGroupCode, "");
    addWidget(ui->leGroupName, "Group");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leNameEn, "Name, en");
    addWidget(ui->leNameRu, "Name, ru");
    addWidget(ui->lePrice, "Price");
    addWidget(ui->leTaxName, "Tax name");
    addWidget(ui->leADGT, "ADGT");
    addWidget(ui->leVATDept, "Tax VAT Dept");
    addWidget(ui->leNoVATDept, "Tax no VAT Dept");
    addWidget(ui->chAuto, "Manual charge");
    addWidget(ui->chRest, "Available in restaurant");
    addWidget(ui->leReceptionTax, "Tax dept, reception");
    fTable = "f_invoice_item";
    fCacheId = cid_invoice_item;
    fDockGroup = new DWSelectorInvoiceItemGroup(this);
    fDockGroup->configure();
    fDockGroup->setSelector(ui->leGroupCode);
    connect(fDockGroup, SIGNAL(invoiceGroup(CI_InvoiceGroup*)), this, SLOT(invoiceGroup(CI_InvoiceGroup*)));
    ui->tblTaxPrint->setColumnWidth(0, 200);
}

REInvoiceItem::~REInvoiceItem()
{
    delete ui;
}

void REInvoiceItem::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    ui->tblTaxPrint->clearContents();

    DatabaseResult dr;
    dr.select(fDb, "select f_comp from serv_tax where f_active=1", fDbBind);
    ui->tblTaxPrint->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->tblTaxPrint->setItem(i, 0, new QTableWidgetItem(dr.value(i, "f_comp").toString()));
        fTaxMap[dr.value(i, "f_comp").toString()] = i;
    }

    fDbBind[":f_item"] = ui->leCode->text();
    dr.select(fDb, "select f_comp, f_dept from f_invoice_item_tax where f_item=:f_item", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        if (!fTaxMap.contains(dr.value(i, "f_comp").toString())) {
            continue;
        }
        int row = fTaxMap[dr.value(i, "f_comp").toString()];
        ui->tblTaxPrint->setItem(row, 1, new QTableWidgetItem(dr.value(i, "f_dept").toString()));
    }
}

void REInvoiceItem::invoiceGroup(CI_InvoiceGroup *c)
{
    dockResponse<CI_InvoiceGroup, CacheInvoiceItemGroup>(ui->leGroupCode, ui->leGroupName, c);
}

void REInvoiceItem::on_tnCancel_clicked()
{
    reject();
}

void REInvoiceItem::on_btnOk_clicked()
{
    save();
    fDbBind[":f_item"] = ui->leCode->text();
    fDb.select("delete from f_invoice_item_tax where f_item=:f_item", fDbBind, fDbRows);
    for (int i = 0; i < ui->tblTaxPrint->rowCount(); i++) {
        if (!ui->tblTaxPrint->item(i, 1)) {
            continue;
        }
        fDbBind[":f_comp"] = ui->tblTaxPrint->toString(i, 0);
        fDbBind[":f_dept"] = ui->tblTaxPrint->toString(i, 1);
        fDbBind[":f_item"] = ui->leCode->text();
        fDb.insert("f_invoice_item_tax", fDbBind);
    }
    BroadcastThread::cmdRefreshCache(cid_tax_map, ui->leCode->text());
}
