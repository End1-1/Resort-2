#include "wcashentry.h"
#include "ui_wcashentry.h"
#include "cachecashdoc.h"
#include "dwselectorstorepartners.h"
#include "dwselectorinvoiceitem.h"
#include "dwselectorcashdoctype.h"

#define SEL_DEBIT 1
#define SEL_CREDIT 2
#define SEL_PARTNER 3
#define SEL_DOC_TYPE 4

WCashEntry::WCashEntry(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WCashEntry)
{
    ui->setupUi(this);

    dockDebit = new DWSelectorCashDesk(this);
    dockDebit->configure();
    dockDebit->setSelector(ui->leDebit);
    dockDebit->setDialog(this, SEL_DEBIT);

    DWSelectorCashDesk *out = new DWSelectorCashDesk(this);
    out->configure();
    out->setSelector(ui->leCashout);
    out->setDialog(this, SEL_CREDIT);

    DWSelectorInvoiceItem *i = new DWSelectorInvoiceItem(this);
    i->configure();
    i->setSelector(ui->leCredit);
    connect(i, &DWSelectorInvoiceItem::invoiceItem, [this](CI_InvoiceItem *c) {
        dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leCredit, ui->leCreditName, c);
    });

    DWSelectorStorePartners *dockPartner = new DWSelectorStorePartners(this);
    dockPartner->configure();
    dockPartner->setSelector(ui->lePartnerCode);
    dockPartner->setDialog(this, SEL_PARTNER);

    DWSelectorCashDocType *dockDocType = new DWSelectorCashDocType(this);
    dockDocType->configure();
    dockDocType->setSelector(ui->leDocTypeNum);
    dockDocType->setDialog(this, SEL_DOC_TYPE);

    connect(ui->leDebit, &EQLineEdit::focusIn, [this]() {

    });

    on_chEnable_stateChanged(0);
    fIn = true;
}

WCashEntry::~WCashEntry()
{
    delete ui;
}

void WCashEntry::save()
{
    if (!ui->chEnable->isChecked()) {
        return;
    }
    QString err;
    if (!isCorrent(err)) {
        message_error(err);
        return;
    }
    if (ui->leCode->asInt() == 0) {
        fDbBind[":f_id"] = 0;
        ui->leCode->setInt(fDb.insert("c_cash", fDbBind));
    }
    if (ui->rbCashIn->isChecked()) {
        fDbBind[":f_date"] = ui->deDate->date();
        fDbBind[":f_docType"] = ui->leDocTypeNum->asInt();
        fDbBind[":f_docNum"] = ui->leDocNum->text();
        fDbBind[":f_debit"] = ui->leDebit->asInt();
        fDbBind[":f_credit"] = ui->leCredit->asInt();
        fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
        fDbBind[":f_amount"] = ui->leAmount->asDouble();
        fDbBind[":f_comment"] = ui->leComment->text();
        fDb.update("c_cash", fDbBind, where_id(ui->leCode->asInt()));
    } else if (ui->rbCashOut->isChecked()) {
        fDbBind[":f_date"] = ui->deDate->date();
        fDbBind[":f_docType"] = ui->leDocTypeNum->asInt();
        fDbBind[":f_docNum"] = ui->leDocNum->text();
        fDbBind[":f_debit"] = ui->leCashout->asInt();
        fDbBind[":f_credit"] = ui->leCredit->asInt();
        fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
        fDbBind[":f_amount"] = ui->leAmount->asDouble() * -1;
        fDbBind[":f_comment"] = ui->leComment->text();
        fDb.update("c_cash", fDbBind, where_id(ui->leCode->asInt()));
    } else if (ui->rbCashMove->isChecked()) {
        if (ui->leDocNum->asInt() == 0) {
            QMap<QString, QVariant> b;
            b[":f_id"] = 0;
            ui->leDocNum->setText(QString::number(fDb.insert("c_cash", b)));
        }
        fDbBind[":f_date"] = ui->deDate->date();
        fDbBind[":f_docType"] = ui->leDocTypeNum->asInt();
        fDbBind[":f_docNum"] = ui->leDocNum->text();
        fDbBind[":f_debit"] = (fIn ? ui->leDebit->asInt() : ui->leCashout->asInt());
        fDbBind[":f_credit"] = ui->leCredit->asInt();
        fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
        fDbBind[":f_amount"] = (fIn ? ui->leAmount->asDouble() : ui->leAmount->asDouble() * -1);
        fDbBind[":f_comment"] = ui->leComment->text();
        fDb.update("c_cash", fDbBind, where_id(ui->leCode->asInt()));

        fDbBind[":f_date"] = ui->deDate->date();
        fDbBind[":f_docType"] = ui->leDocTypeNum->asInt();
        fDbBind[":f_docNum"] = ui->leCode->text();
        fDbBind[":f_debit"] = (fIn ? ui->leCashout->asInt() : ui->leDebit->asInt());
        fDbBind[":f_credit"] = ui->leCredit->asInt();
        fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
        fDbBind[":f_amount"] = (fIn ? ui->leAmount->asDouble() * -1 : ui->leAmount->asDouble());
        fDbBind[":f_comment"] = ui->leComment->text();
        fDb.update("c_cash", fDbBind, where_id(ui->leDocNum->asInt()));
    }
    message_info(tr("Saved"));
}

void WCashEntry::load(int doc)
{
    ui->wOp->setEnabled(false);
    ui->leCode->setInt(doc);
    DatabaseResult dr;
    fDbBind[":f_id"] = doc;
    dr.select(fDb, "select * from c_cash where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Invalid document number"));
        return;
    }
    if (dr.value("f_docType").toInt() == CASHDOC_MOVE) {
        ui->rbCashMove->setChecked(true);
        cashOp(CASHOP_MOVE);
    } else {
        if (dr.value("f_amount").toDouble() > 0.1) {
            ui->rbCashIn->setChecked(true);
            cashOp(CASHOP_IN);
        } else {
            ui->rbCashOut->setChecked(true);
            cashOp(CASHOP_OUT);
        }
    }
    ui->leDocTypeNum->setInt(dr.value("f_docType").toInt());
    dockResponse<CI_CashDoc, CacheCashDoc>(ui->leDocTypeNum, ui->leDocTypeName, 0);
    ui->leDocNum->setInt(dr.value("f_docNum").toInt());
    ui->deDate->setDate(dr.value("f_date").toDate());
    ui->leAmount->setDouble(abs(dr.value("f_amount").toDouble()));
    if (dr.value("f_amount").toDouble() > 0.1) {
        ui->leDebit->setInt(dr.value("f_debit").toInt());
        dockResponse<CI_CashDesk, CacheCashDesk>(ui->leDebit, ui->leDebitName, 0);
    } else {
        ui->leCashout->setInt(dr.value("f_debit").toInt());
        dockResponse<CI_CashDesk, CacheCashDesk>(ui->leCashout, ui->leCashOutName, 0);
    }
    ui->leCredit->setInt(dr.value("f_credit").toInt());
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leCredit, ui->leCreditName, 0);
    ui->lePartnerCode->setInt(dr.value("f_partner").toInt());
    dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, 0);
    ui->leComment->setText(dr.value("f_comment").toString());

    if (ui->rbCashMove->isChecked()) {
        if (ui->leAmount->asDouble() > 0.1) {
            fIn = true;
        } else {
            fIn = false;
        }
        fDbBind[":f_docType"] = CASHDOC_MOVE;
        fDbBind[":f_id"] = ui->leDocNum->asInt();
        dr.select(fDb, "select f_debit from c_cash where f_id=:f_id and f_docType=:f_docType", fDbBind);
        if (dr.rowCount() == 0) {
            message_error(tr("Error in cash movement document"));
            return;
        }
        if (fIn) {
            ui->leCashout->setInt(dr.value("f_debit").toInt());
            dockResponse<CI_CashDesk, CacheCashDesk>(ui->leCashout, ui->leCashOutName, 0);
        } else {
            ui->leDebit->setInt(dr.value("f_debit").toInt());
            dockResponse<CI_CashDesk, CacheCashDesk>(ui->leDebit, ui->leDebitName, 0);
        }
    }
}

void WCashEntry::enable()
{
    ui->chEnable->setChecked(true);
}

void WCashEntry::disable()
{
    ui->chEnable->setChecked(false);
}

int WCashEntry::debit()
{
    return ui->leDebit->asInt();
}

void WCashEntry::debit(int d)
{
    CI_CashDesk *ci = CacheCashDesk::instance()->get(d);
    dockResponse<CI_CashDesk, CacheCashDesk>(ui->leDebit, ui->leDebitName, ci);
}

int WCashEntry::credit()
{
    return ui->leCredit->asInt();
}

void WCashEntry::credit(int c)
{
    CI_CashDesk *ci = CacheCashDesk::instance()->get(c);
    dockResponse<CI_CashDesk, CacheCashDesk>(ui->leCredit, ui->leCreditName, ci);
}

int WCashEntry::cashOp()
{
    if (ui->rbCashIn->isChecked()) {
        return CASHOP_IN;
    } else if (ui->rbCashOut->isChecked()) {
        return CASHOP_OUT;
    } else if (ui->rbCashMove->isChecked()) {
        return CASHOP_MOVE;
    } else {
        return CASHOP_NONE;
    }
}

void WCashEntry::cashOp(int c)
{
    switch (c) {
    case CASHOP_NONE:
        ui->rbCashIn->setChecked(false);
        ui->rbCashOut->setChecked(false);
        ui->rbCashMove->setChecked(false);
        ui->lbCashIn->setVisible(false);
        ui->leDebit->setVisible(false);
        ui->leDebitName->setVisible(false);
        ui->lbCachout->setVisible(false);
        ui->leCashout->setVisible(false);
        ui->leCashOutName->setVisible(false);
        break;
    case CASHOP_IN:
        ui->rbCashIn->setChecked(true);
        break;
    case CASHOP_OUT:
        ui->rbCashOut->setChecked(true);
        break;
    case CASHOP_MOVE:
        ui->rbCashMove->setChecked(true);
        ui->leDocTypeNum->setInt(CASHDOC_MOVE);
        break;
    default:
        break;
    }
    ui->lbCashIn->setVisible(c == CASHOP_IN || c == CASHOP_MOVE);
    ui->leDebit->setVisible(c == CASHOP_IN || c == CASHOP_MOVE);
    ui->leDebitName->setVisible(c == CASHOP_IN || c == CASHOP_MOVE);
    ui->lbCachout->setVisible(c == CASHOP_OUT || c == CASHOP_MOVE);
    ui->leCashout->setVisible(c == CASHOP_OUT || c == CASHOP_MOVE);
    ui->leCashOutName->setVisible(c == CASHOP_OUT || c == CASHOP_MOVE);
}

QString WCashEntry::comment()
{
    return ui->leComment->text();
}

void WCashEntry::comment(const QString &c)
{
    ui->leComment->setText(c);
}

QDate WCashEntry::date()
{
    return ui->deDate->date();
}

void WCashEntry::date(const QDate &date)
{
    ui->deDate->setDate(date);
}

double WCashEntry::amount()
{
    return ui->leAmount->asDouble();
}

void WCashEntry::amount(double a)
{
    ui->leAmount->setDouble(a);
}

void WCashEntry::docType(int t)
{
    CI_CashDoc *ci = CacheCashDoc::instance()->get(t);
    dockResponse<CI_CashDoc, CacheCashDoc>(ui->leDocTypeNum, ui->leDocTypeName, ci);
    switch (t) {
    case CASHDOC_DEBT:
        ui->rbCashOut->setChecked(true);
        cashOp(CASHOP_OUT);
    }
}

void WCashEntry::docNum(QString n)
{
    ui->leDocNum->setText(n);
}

int WCashEntry::partner()
{
    return ui->lePartnerCode->asInt();
}

void WCashEntry::partner(int p)
{
    CI_StorePartners *ci = CacheStorePartners::instance()->get(p);
    dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, ci);
}

bool WCashEntry::isCorrent(QString &errors)
{
    if (!ui->chEnable->isChecked()) {
        return true;
    }
    errors.clear();
    if (!cashOp()) {
        errors += tr("Cash operation is not defined") + "<br>";
    }
    if (ui->rbCashIn->isChecked() || ui->rbCashMove->isChecked()) {
        if (ui->leDebit->asInt() == 0) {
            errors += tr("Cash in is not defined") + "<br>";
        }
    }
    if (ui->rbCashOut->isChecked() || ui->rbCashMove->isChecked()) {
        if (ui->leCredit->asInt() == 0) {
            errors += tr("Purpose is not defined") + "<br>";
        }
    }
    if (ui->leDocTypeNum->asInt() == 0) {
        errors += tr("Document type was not selected") + "<br>";
    }
    if (ui->leAmount->asDouble() < 0.1) {
        errors += tr("Amount must be greater than 0") + "<br>";
    }
    return errors.isEmpty();
}

void WCashEntry::setSaveVisible(bool v)
{
    ui->wOK->setVisible(v);
}

void WCashEntry::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_DEBIT: {
        dockResponse<CI_CashDesk, CacheCashDesk>(ui->leDebit, ui->leDebitName, value.value<CI_CashDesk*>());
        break;
    }
    case SEL_CREDIT: {
        dockResponse<CI_CashDesk, CacheCashDesk>(ui->leCashout, ui->leCashOutName, value.value<CI_CashDesk*>());
        break;
    }
    case SEL_PARTNER:
        dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, value.value<CI_StorePartners*>());
        break;
    case SEL_DOC_TYPE:
        dockResponse<CI_CashDoc, CacheCashDoc>(ui->leDocTypeNum, ui->leDocTypeName, value.value<CI_CashDoc*>());
        break;
    }
}

void WCashEntry::on_btnCancel_clicked()
{
    emit rejected();
}

void WCashEntry::on_btnSave_clicked()
{
    emit accepted();
}

void WCashEntry::on_chEnable_stateChanged(int arg1)
{
//    ui->leCode->setEnabled(ui->leCode->isEnabled() && arg1);
//    ui->leDocTypeNum->setEnabled(ui->leDocTypeNum->isEnabled() && arg1);
//    ui->deDate->setEnabled(ui->deDate->isEnabled() && arg1);
//    ui->leAmount->setEnabled(ui->leAmount->isEnabled() && arg1);
//    ui->leDebit->setEnabled(ui->leDebit->isEnabled() && arg1);
//    ui->leCredit->setEnabled(ui->leCredit->isEnabled() && arg1);
//    ui->lePartnerCode->setEnabled(ui->lePartnerCode->isEnabled() && arg1);
//    ui->leComment->setEnabled(ui->leComment->isEnabled() && arg1);

    ui->leCode->setEnabled(arg1);
    ui->leDocTypeNum->setEnabled(arg1);
    ui->deDate->setEnabled(arg1);
    ui->leAmount->setEnabled(arg1);
    ui->leDebit->setEnabled(arg1);
    ui->leCredit->setEnabled(arg1);
    ui->lePartnerCode->setEnabled(arg1);
    ui->leComment->setEnabled(arg1);
}

void WCashEntry::on_chEnable_clicked(bool checked)
{
    on_chEnable_stateChanged(checked);
}

void WCashEntry::on_rbCashIn_clicked()
{
    cashOp(CASHOP_IN);
}

void WCashEntry::on_rbCashOut_clicked()
{
    cashOp(CASHOP_OUT);
}

void WCashEntry::on_rbCashMove_clicked()
{
    cashOp(CASHOP_MOVE);
}
