#include "fcash.h"
#include "ui_fcash.h"
#include "wreportgrid.h"
#include "dwselectorcashdesk.h"
#include "dwselectorstorepartners.h"
#include "dwselectorinvoiceitem.h"
#include "wcashentry.h"
#include "dlgcashop.h"
#include "cachecashdoc.h"

#define SEL_DEBIT 1
#define SEL_CREDIT 2
#define SEL_PARTNER 3

FCash::FCash(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCash)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Cashdesk of cash"), ":/images/refund.png");
    fReportGrid->setBtnNewVisible(true);
    connect(fReportGrid, &WReportGrid::newBtn, [this](){
        DlgCashOp::cash(0);
    });
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
    fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removeDoc()), this)->setFocusPolicy(Qt::NoFocus);

    DWSelectorCashDesk *d = new DWSelectorCashDesk(this);
    d->configure();
    d->setSelector(ui->leDCode);
    d->setDialog(this, SEL_DEBIT);

    DWSelectorInvoiceItem *i = new DWSelectorInvoiceItem(this);
    i->configure();
    i->setSelector(ui->leCCode);
    connect(i, &DWSelectorInvoiceItem::invoiceItem, [this](CI_InvoiceItem *c) {
        dock<CI_InvoiceItem, CacheInvoiceItem>(ui->leCCode, ui->leCName, c);
    });

    DWSelectorStorePartners *p = new DWSelectorStorePartners(this);
    p->configure();
    p->setSelector(ui->lePartnerCode);
    p->setDialog(this, SEL_PARTNER);
}

FCash::~FCash()
{
    delete ui;
}

QString FCash::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Accounts"))
            .arg(ui->deFrom->text())
            .arg(ui->deTo->text());
}

QWidget *FCash::firstElement()
{
    return ui->deFrom;
}

void FCash::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Code"))
            .setColumn(120, "", tr("Date"))
            .setColumn(200, "", tr("Cash"))
            .setColumn (200, "", tr("Purpose"))
            .setColumn(200, "", tr("Partner"))
            .setColumn(100, "", tr("Amount"))
            .setColumn(300, "", tr("Comment"));
    QString query = "select c.f_id, c.f_date, cnd.f_name, cnc.f_en, p.f_name, c.f_amount, c.f_comment "
                    "from c_cash c "
                    "left join c_cash_names cnd on cnd.f_id=c.f_debit "
                    "left join f_invoice_item cnc on cnc.f_id=c.f_credit "
                    "left join r_partners p on p.f_id=c.f_partner "
                    "where c.f_date between " + ui->deFrom->dateMySql() + " and " + ui->deTo->dateMySql() + " ";
    if (ui->leDCode->asInt() > 0) {
        query += " and c.f_debit=" + ui->leDCode->text();
    }
    if (ui->leCCode->asInt() > 0) {
        query += " and c.f_credit=" + ui->leCCode->text();
    }
    if (ui->lePartnerCode->asInt() > 0) {
        query += " and c.f_partner=" + ui->lePartnerCode->text();
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 5;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FCash::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_DEBIT:
        dock<CI_CashDesk, CacheCashDesk>(ui->leDCode, ui->leDName, value.value<CI_CashDesk*>());
        break;
    case SEL_CREDIT:
        dock<CI_CashDesk, CacheCashDesk>(ui->leCCode, ui->leCName, value.value<CI_CashDesk*>());
        break;
    case SEL_PARTNER:
        dock<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, value.value<CI_StorePartners*>());
        break;
    }
}

void FCash::doubleClickOnRow(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    DlgCashOp::cash(row.at(0).toInt());
}

void FCash::removeDoc()
{
    QList<QVariant> out;
    int row;
    if ((row = fReportGrid->fillRowValuesOut(out)) < 0) {
        message_info(tr("Nothing was selected"));
        return;
    }
    if (message_confirm_tr("Are you sure to remove selected document") != QDialog::Accepted) {
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_docType"] = CASHDOC_MOVE;
    fDbBind[":f_id"] = out.at(0).toInt();
    dr.select(fDb, "select f_id from c_cash where f_docNum=:f_id and f_docType=:f_docType", fDbBind);
    if (dr.rowCount() > 0) {
        fDbBind[":f_id"] = dr.value("f_id");
        fDb.select("delete from c_cash where f_id=:f_id", fDbBind, fDbRows);
    }
    fDbBind[":f_id"] = out.at(0);
    fDb.select("delete from c_cash where f_id=:f_id", fDbBind, fDbRows);

}
