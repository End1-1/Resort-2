#include "dlgchangeexportvaucher.h"
#include "ui_dlgchangeexportvaucher.h"

DlgChangeExportVaucher::DlgChangeExportVaucher(DatabaseResult &dr, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgChangeExportVaucher),
    fDr(dr)
{
    ui->setupUi(this);
    fDockItem = new DWSelectorInvoiceItem(this);
    fDockItem->configure();
    fDockItem->setSelector(ui->leItemCode);
    connect(fDockItem, &DWSelectorInvoiceItem::invoiceItem, [this](CI_InvoiceItem *c){
       dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leItemCode, ui->leItemName, c);
       if (c) {
           ui->leSource->setText(c->fVaucher);
       }
    });
}

DlgChangeExportVaucher::~DlgChangeExportVaucher()
{
    delete ui;
}

void DlgChangeExportVaucher::setDatabaseResult(int i)
{
    ui->deDate->setDate(fDr.value(i, "f_wdate").toDate());
    ui->leAmount->setText(fDr.value(i, "f_amountAmd").toString());
    ui->leItemCode->setText(fDr.value(i, "f_itemCode").toString());
    ui->leItemName->setText(fDr.value(i, "f_finalName").toString());
    ui->leSource->setText(fDr.value(i, "f_source").toString());
    ui->teTime->setTime(fDr.value(i, "f_time").toTime());
    fIndex = i;
}

void DlgChangeExportVaucher::on_btnCancel_clicked()
{
    reject();
}

void DlgChangeExportVaucher::on_btnOK_clicked()
{
    QString newId = uuid(ui->leSource->text(), fAirDb);
    fDr.setValue(fIndex, "f_id", newId);
    fDr.setValue(fIndex, "f_wdate", ui->deDate->date());
    fDr.setValue(fIndex, "f_rdate", ui->deDate->date());
    fDr.setValue(fIndex, "f_amountAmd", ui->leAmount->text());
    fDr.setValue(fIndex, "f_itemCode", ui->leItemCode->text());
    fDr.setValue(fIndex, "f_finalName", ui->leItemName->text());
    fDr.setValue(fIndex, "f_time", ui->teTime->time());
    fDr.setValue(fIndex, "f_source", ui->leSource->text());
    accept();
}
