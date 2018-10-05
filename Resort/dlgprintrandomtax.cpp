#include "dlgprintrandomtax.h"
#include "ui_dlgprintrandomtax.h"
#include "dlgprinttaxsm.h"
#include <QDir>
#include <QFile>
#include <QProcess>

DlgPrintRandomTax::DlgPrintRandomTax(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPrintRandomTax)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 300, 100, 100, 100, 30, 0, 0, 0);
    fDockItem = new DWSelectorInvoiceItem(this);
    fDockItem->setFilter(fPreferences.getDb(def_filter_manual_tax).toString());
    fDockItem->configure();
    fDockItem->setSelector(ui->leItemCode);
    connect(fDockItem, SIGNAL(invoiceItem(CI_InvoiceItem*)), this, SLOT(invoiceItem(CI_InvoiceItem*)));
}

DlgPrintRandomTax::~DlgPrintRandomTax()
{
    delete ui;
}

void DlgPrintRandomTax::setHeader(const QString &room, const QString &guest, const QString &invoice)
{
    ui->leRoom->setText(room);
    ui->leGuest->setText(guest);
    ui->leInvoice->setText(invoice);
}

void DlgPrintRandomTax::countTotal()
{
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        total += ui->tblData->toDouble(i, 3);
    }
    ui->leTotal->setDouble(total);
    ui->leCash->setDouble(total);
    ui->leCard->setDouble(0);
    ui->lePrepaiment->setDouble(0);
}

void DlgPrintRandomTax::correctAmounts(EQLineEdit *l1, EQLineEdit *l2, EQLineEdit *l3)
{
    double total = ui->leTotal->asDouble();
    if (l1->asDouble() > total) {
        l1->setDouble(total);
        l2->setDouble(0);
        l3->setDouble(0);
    }
    l2->setDouble(total - l3->asDouble() - l1->asDouble());
    if (l2->asDouble() < 0) {
        l3->setDouble(l3->asDouble() + l2->asDouble());
        l2->setDouble(0);
    }
}

void DlgPrintRandomTax::invoiceItem(CI_InvoiceItem *c)
{
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leItemCode, ui->leItemName, c);
}

void DlgPrintRandomTax::on_btnAdd_clicked()
{
    if (ui->leItemCode->asInt() == 0) {
        return;
    }
    CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(ui->leItemCode->text());
    if (!ii) {
        message_error_tr("Invalid item code");
        return;
    }
    int qty = ui->leItemQty->asInt();
    double price = ui->leItemAmount->asDouble() / qty;
    for (int i = 0; i < qty; i++) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        int col = 0;
        ui->tblData->setItem(row, col++, Utils::tableItem(ui->leItemCode->text()));
        ui->tblData->setItem(row, col++, Utils::tableItem(ui->leItemName->text()));
        ui->tblData->setItem(row, col++, Utils::tableItem("1"));
        ui->tblData->setItem(row, col++, Utils::tableItem(float_str(price, 2)));
        ui->tblData->setItem(row, col++, Utils::tableItem(float_str(price, 2)));
        ui->tblData->addButton(row, col++, SLOT(btnClickWithTag(int)), this, QIcon(":/images/garbage.png"));
        ui->tblData->setItem(row, col++, Utils::tableItem(ii->fAdgt));
        ui->tblData->setItem(row, col++, Utils::tableItem(0));
        ui->tblData->setItem(row, col++, Utils::tableItem(ii->fVaucher));
    }
    ui->leItemCode->clear();
    ui->leItemName->clear();
    ui->leItemQty->setText("1");
    ui->leItemAmount->clear();
    countTotal();
    ui->leItemCode->setFocus();
}

void DlgPrintRandomTax::btnClickWithTag(int tag)
{
    tag = -1;
    EPushButton *src = static_cast<EPushButton*>(sender());
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        EPushButton *btn = static_cast<EPushButton*>(ui->tblData->cellWidget(i, 5));
        if (btn == src) {
            tag = i;
            break;
        }
    }
    if (tag < 0) {
        return;
    }
    if (ui->tblData->toInt(tag, 7) > 0) {
        fDbBind[":f_id"] = ui->tblData->toInt(tag, 7);
        fDb.select("delete from m_free_tax where f_id=:f_id", fDbBind, fDbRows);
    }
    ui->tblData->removeRow(tag);
    countTotal();
}

void DlgPrintRandomTax::on_leCash_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    correctAmounts(ui->leCash, ui->leCard, ui->lePrepaiment);
}

void DlgPrintRandomTax::on_btnPrint_clicked()
{
    if (ui->tblData->rowCount() == 0) {
        message_error_tr("Nothing to print");
        return;
    }
    DlgPrintTaxSM dpt(this);
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(ui->tblData->toString(i, 0));
        dpt.addGoods(ii->fVatDept,
                     ii->fAdgt,
                     ii->fCode,
                     ii->fTaxName,
                     ui->tblData->toDouble(i, 3),
                     ui->tblData->toDouble(i, 2));

    }
    dpt.fOrder = ui->leInvoice->text();
    dpt.fCardAmount = ui->leCard->asDouble();
    dpt.fPrepaid = ui->lePrepaiment->asDouble();

    int result = dpt.exec();
    if (result == TAX_OK) {
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
//            if (ui->tblData->item(i, 4)->checkState() != Qt::Checked) {
//                continue;
//            }
//            fDbBind[":f_fiscal"] = dpt.fTaxCode;
//            fDb.update("m_register", fDbBind, ap(ui->tblData->toString(i, 8)));

            fDbBind[":f_invoice"] = ui->leInvoice->text();
            fDbBind[":f_item"] = ui->tblData->toInt(i, 0);
            fDbBind[":f_amount"] = ui->tblData->toDouble(i, 4);
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_date"] = QDateTime::currentDateTime();
            fDbBind[":f_source"] = ui->tblData->toString(i, 8);
            fDbBind[":f_taxCode"] = dpt.fTaxCode;
            ui->tblData->setItem(i, 7, Utils::tableItem(fDb.insert("m_free_tax", fDbBind)));
        }
        accept();
    }
    /*
    QDir d;
    QString fileName = QString("tax_%1.json").arg(ui->leInvoice->text());
    QFile f(d.currentPath() + "/" + fileName);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QString("{\"seq\":1,\"paidAmount\":%1, \"paidAmountCard\":%2, \"partialAmount\":0, "
                        "\"prePaymentAmount\":%3, \"useExtPOS\":true, \"mode\":2,\"items\":[")
                 .arg(float_str(ui->leCash->asDouble(), 2))
                 .arg(float_str(ui->leCard->asDouble(), 2))
                 .arg(float_str(ui->lePrepaiment->asDouble(), 2)).toUtf8());
        bool first = true;
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            if (!first)
                f.write(",");
            else
                first = false;
            CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(ui->tblData->toString(i, 0));
            f.write(QString("{\"dep\":%1,\"qty\":%2,\"price\":%3, \"totalPrice\":%4, "
                            "\"productCode\":\"%5\",\"productName\":\"%6\",\"adgCode\":\"%7\", \"unit\":\"%8\"}")
                    .arg(ii->fVatDept)
                    .arg(ui->tblData->toString(i, 2))
                    .arg(ui->tblData->toString(i, 3))
                    .arg(ui->tblData->toString(i, 4))
                    .arg(ii->fCode)
                    .arg(ii->fTaxName)
                    .arg(ii->fAdgt)
                    .arg(QString::fromUtf8("հատ"))
                    .toUtf8());
        }
        f.write("]}");
        f.close();
        QStringList args;
        args << fPreferences.getDb(def_tax_address).toString()
             << fPreferences.getDb(def_tax_port).toString()
             << fPreferences.getDb(def_tax_password).toString()
             << fileName;
        QProcess *p = new QProcess();
        p->start(d.currentPath() + "/printtax.exe", args);
    }
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->toInt(i, 7) > 0) {
            continue;
        }
        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDbBind[":f_item"] = ui->tblData->toInt(i, 0);
        fDbBind[":f_amount"] = ui->tblData->toDouble(i, 4);
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_date"] = QDateTime::currentDateTime();
        fDbBind[":f_source"] = ui->tblData->toString(i, 8);
        ui->tblData->setItem(i, 7, Utils::tableItem(fDb.insert("m_free_tax", fDbBind)));

        fDbBind[":f_vaucher"] = 0;
        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_name"] = ui->tblData->toString(i, 1);
        fDbBind[":f_amountCash"] = ui->leCash->asDouble();
        fDbBind[":f_amountCard"] = ui->leCard->asDouble();
        fDbBind[":f_amountPrepaid"] = ui->lePrepaiment->asDouble();
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comp"] = HOSTNAME;
        fDb.insert("m_tax_history", fDbBind);
    }
    */
}

void DlgPrintRandomTax::on_leCard_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    correctAmounts(ui->leCard, ui->leCash, ui->lePrepaiment);
}

void DlgPrintRandomTax::on_btnRetryPrint_clicked()
{
    QDir d;
    QString fileName = QString("tax_%1.json").arg(ui->leInvoice->text());
    QFile f(d.currentPath() + "/" + fileName);
    if (f.exists()) {
        QStringList args;
        args << fPreferences.getDb(def_tax_address).toString()
             << fPreferences.getDb(def_tax_port).toString()
             << fPreferences.getDb(def_tax_password).toString()
             << fileName;
        QProcess *p = new QProcess();
        p->start(d.currentPath() + "/printtax.exe", args);
    } else {
        on_btnPrint_clicked();
    }
}

void DlgPrintRandomTax::on_lePrepaiment_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    correctAmounts(ui->lePrepaiment, ui->leCash, ui->leCard);
}

void DlgPrintRandomTax::on_leItemQty_textEdited(const QString &arg1)
{
    if (arg1.toInt() > 100) {
        ui->leItemQty->setText("100");
    }
}
