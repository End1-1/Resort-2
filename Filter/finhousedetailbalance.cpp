#include "finhousedetailbalance.h"
#include "ui_finhousedetailbalance.h"
#include "wreportgrid.h"
#include "winvoice.h"

FInhouseDetailBalance::FInhouseDetailBalance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FInhouseDetailBalance)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("In House Detailed Balance"), ":/images/invoice.png");
    connect(fReportGrid, &WReportGrid::doubleClickOnRow, [this](const QList<QVariant> &val){
        if (val.count() == 0) {
            return;
        }
        if (val.at(0).toString().isEmpty()) {
            return;
        }
        WInvoice::openInvoiceWindow(val.at(0).toString());
    });
}

FInhouseDetailBalance::~FInhouseDetailBalance()
{
    delete ui;
}

QString FInhouseDetailBalance::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("In House Detailed Balance"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FInhouseDetailBalance::firstElement()
{
    return ui->deStart;
}

void FInhouseDetailBalance::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Invoice"))
            .setColumn(40, "", tr("Room"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(200, "", tr("Description"))
            .setColumn(100, "", tr("Debit"))
            .setColumn(100, "", tr("Credit"))
            .setColumn(150, "", tr("Payment"))
            .setColumn(150, "", tr("Comment"));
    QString query = "select r.f_invoice, r.f_room, g.guest, m.f_finalName, d.f_amountAmd, c.f_amountAmd, pm.f_" + def_lang + ", "
            "c.f_paymentComment "
            "from f_reservation r "
            "left join m_register m on m.f_inv=r.f_invoice "
            "left join f_payment_type pm on pm.f_id=m.f_paymentMode "
            "left join guests g on r.f_Guest=g.f_id "
            "left join (select f_id as did, f_inv, f_amountAmd from m_register where f_finance=1 and f_canceled=0 and f_sign=1 and f_wdate between :date1 and :date2) d on d.f_inv=r.f_invoice and d.did=m.f_id "
            "left join (select f_id as did, f_inv, f_amountAmd, f_paymentComment from m_register where f_finance=1 and f_canceled=0 and f_sign=-1 and f_wdate between :date1 and :date2) c on c.f_inv=r.f_invoice and c.did=m.f_id "
            "where (r.f_state=1 or (r.f_state=3 and :date1 between r.f_startDate and r.f_endDate)) and (d.f_amountAmd is not null or c.f_amountAmd is not null)"
            "order by 1 ";
    query.replace(":date1", ui->deStart->dateMySql());
    query.replace(":date2", ui->deEnd->dateMySql());
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> col;
    col << 4 << 5;
    rg->fModel->insertSubTotals(0, col);
    QString curr;
    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        if (i == 0) {
            curr == rg->fModel->data(i, 0).toString();
            continue;
        }
        if (rg->fModel->data(i, 0).toString() == curr) {
            rg->fModel->setData(i, 0, "");
            rg->fModel->setData(i, 1, "");
            rg->fModel->setData(i, 2, "");
        } else {
            curr = rg->fModel->data(i, 0).toString();
        }
    }
}
