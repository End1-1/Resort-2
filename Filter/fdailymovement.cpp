#include "fdailymovement.h"
#include "ui_fdailymovement.h"
#include "cacheinvoiceitem.h"
#include "wreportgrid.h"
#include "cachepaymentmode.h"
#include "paymentmode.h"
#include "vauchers.h"

FDailyMovement::FDailyMovement(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FDailyMovement)
{
    ui->setupUi(this);
    fReportGrid->addToolBarButton(":/images/invoice.png", tr("Open invoice"), SLOT(openInvoice()), this)
            ->setFocusPolicy(Qt::ClickFocus);

    QString query = "select u.f_username, m.f_rdate, m.f_time, m.f_room, m.f_itemCode, "
            "m.f_id, left(m.f_dc, 1), m.f_finalName, p.f_en, m.f_paymentComment, "
            "m.f_amountAmd, m.f_amountVat, m.f_amountAmd / m.f_amountUsd as f_amountUsd, m.f_guest, m.f_source, "
            "uc.f_username as f_username2, m.f_id as f_id2, m.f_inv, m.f_paymentMode "
            "from m_register m "
            "left join users u on u.f_id=m.f_user "
            "left join f_payment_type p on p.f_id=m.f_paymentMode "
            "left join users uc on uc.f_id=m.f_cancelUser "
            "where m.f_wdate=:f_wdate and m.f_canceled=:f_canceled :where "
            "order by :order "
           ;

    fReportGrid->fTableView->verticalHeader()->setDefaultSectionSize(20);
    fReportGrid->fStaticQuery = query;
    fReportGrid->addFilterWidget(this);
    fReportGrid->setupTabTextAndIcon(tr("Daily movement"), ":/images/graph.png");
    fDockItem = new DWSelectorInvoiceItem(this);
    fDockItem->fGroup = 0;
    fDockItem->configure();
    fDockItem->setSelector(ui->leSale);
    connect(fDockItem, SIGNAL(invoiceItem(CI_InvoiceItem*)), this, SLOT(invoiceItem(CI_InvoiceItem*)));

    GODaily g("Daily movement");
    Q_UNUSED(g)
}

FDailyMovement::~FDailyMovement()
{
    delete ui;
}

void FDailyMovement::apply(WReportGrid *rg)
{
    rg->fDbRows.clear();
    rg->fModel->clearColumns();
    rg->fModel->
            setColumn(30,"", tr("Op")) //1
            .setColumn(80, "", tr("Date")) //2
            .setColumn(60, "", tr("Time")) //3
            .setColumn(80, "", tr("Room")) //4
            .setColumn(0, "", tr("Sal")) //5
            .setColumn(100, "", tr("Code")) //6
            .setColumn(40, "", tr("*")) //7
            .setColumn(300, "", tr("Remark")) //8
            .setColumn(100, "", tr("Payment")) //9
            .setColumn(100, "", tr("Type")) //10
            .setColumn(100, "", tr("Amount")) //11
            .setColumn(80, "", tr("VAT")) //12
            .setColumn(80, "", tr("USD")) //13
            .setColumn(300, "", tr("Guest")) //14
            .setColumn(0, "", tr("Voucher")) //15
            .setColumn((ui->chCanceled->isChecked() ? 100 : 0), "", tr("Cancel by")) //16
            .setColumn(0, "", tr("RecId")) //17
            .setColumn(0, "", tr("Invoice")) //18
            .setColumn(0, "", tr("Payment mode")) // 19
            ;
    QStringList items = fPreferences.getDb(def_daily_movement_items).toString().split(";", QString::SkipEmptyParts);
    if (ui->leSale->fHiddenText.length() > 0) {
        processItems(ui->leSale->fHiddenText);
    } else {
        foreach (QString s, items) {
            processItems(s);
        }
    }
    rg->fModel->fDD.fDbRows = rg->fDbRows;
    rg->fModel->apply(0);
    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        if (rg->fModel->data(i, 0).toString().contains(tr("SUBTOTAL FOR")) ||
                rg->fModel->data(i, 1).toString().isEmpty()) {
            rg->fTableView->setSpan(i, 0, 1, 10);
        } else if (rg->fModel->data(i, 0).toString().isEmpty()) {
            rg->fTableView->setSpan(i, 0, 1, 18);
        }
    }
    rg->fModel->removeRow(rg->fModel->rowCount() - 1);
    /*
    QString query = fReportGrid->fStaticQuery;
    query = query.replace(":f_wdate", ui->deStart->dateMySql())
            .replace(":f_canceled", (ui->chCanceled->isChecked() ? "1" : "0"))
            .replace(":where", where);

    fReportGrid->fModel->setSqlQuery(query);
    fReportGrid->fModel->apply(fReportGrid);0
    int i = 0;
    while (i < fReportGrid->fModel->rowCount() - 1) {
        if (fReportGrid->fModel->data(i, 4).toInt() == 30) {
            if (fReportGrid->fModel->data(i, 8).toString() == "CITY LEDGER"
                    && (fReportGrid->fModel->data(i, 14).toString() == "RV"
                        || fReportGrid->fModel->data(i, 14).toString() == "AV")) {
                fReportGrid->fModel->removeRow(i);
                continue;
            }
        }
        i++;
    }

    QList<int> cols;
    cols << 10 << 11 << 12;
    rg->fModel->insertSubTotals(17, cols);
    double cash = 0, card = 0, bank = 0;
    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        if (rg->fModel->data(i, 4).toInt() == fPreferences.getDb(def_advance_voucher_id).toInt()
                || rg->fModel->data(i, 4).toInt() == fPreferences.getDb(def_receip_vaucher_id).toInt()) {
            if (rg->fModel->data(i, 8).toString() == "CASH") {
                cash += rg->fModel->data(i, 10, Qt::EditRole).toDouble();
            } else if (rg->fModel->data(i, 8).toString().contains("CARD", Qt::CaseInsensitive)) {
                card += rg->fModel->data(i, 10, Qt::EditRole).toDouble();
            } else if (rg->fModel->data(i, 8).toString().contains("BANK", Qt::CaseInsensitive)) {
                bank += rg->fModel->data(i, 10, Qt::EditRole).toDouble();
            }
        }

        if (rg->fModel->data(i, 0).toString().contains("Subtotal", Qt::CaseInsensitive)) {
            int itemCode = rg->fModel->data(i - 1, 4).toInt();
            if (itemCode == fPreferences.getDb(def_advance_voucher_id).toInt()) {
                itemCode = fPreferences.getDb(def_receip_vaucher_id).toInt();
            }
            CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(itemCode);
            if (!ii) {
                message_error(tr("Invoice item missing for") + "\r\n" + QString::number(itemCode));
                return;
            }
            rg->fModel->setData(i, 0, QString("%1 %2").arg(tr("Subtotal")).arg(ii->fName));
            if (itemCode == fPreferences.getDb(def_receip_vaucher_id).toInt()) {
                QList<QVariant> emptyRow;
                         //  1      2     3     4     5    6    7     8      9    10    11    12    13    14    15    16    17    18
                emptyRow << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
                emptyRow[0] = QString("%1").arg(tr("TOTAL CASH"));
                emptyRow[10] = cash;
                rg->fModel->insertRow(i, emptyRow);
                rg->fTableView->setSpan(i, 0, 1, 10);
                i++;
                emptyRow[0] = QString("%1").arg(tr("TOTAL CARD"));
                emptyRow[10] = card;
                rg->fModel->insertRow(i, emptyRow);
                rg->fTableView->setSpan(i, 0, 1, 10);
                i++;
                emptyRow[0] = QString("%1").arg(tr("TOTAL BANK"));
                emptyRow[10] = bank;
                rg->fModel->insertRow(i, emptyRow);
                rg->fTableView->setSpan(i, 0, 1, 10);
                i++;
            }
        }
    }
    */
}

QWidget *FDailyMovement::firstElement()
{
    return ui->deStart;
}

QString FDailyMovement::reportTitle()
{
    QString result = QString("%1 %2")
            .arg(tr("Daily movement"))
            .arg(ui->deStart->text());
    if (ui->chCanceled->isChecked()) {
        result += " [" + tr("Canceled") + "]";
    }
    return result;
}

GOWidget *FDailyMovement::gridOptionWidget()
{
    return new GODaily("Daily movement");
}

void FDailyMovement::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FDailyMovement *dm = new FDailyMovement(rg);
    dm->apply(rg);
}

void FDailyMovement::setupTab()
{
    setupTabTextAndIcon(QString("%1 %2")
                        .arg(tr("Daily movement"))
                        .arg(ui->deStart->date().toString(def_date_format)),
                        ":/images/graph.png");
}

void FDailyMovement::processPayments(QList<QVariant> &emptyRow)
{
    QString mode;
    double totalAMD = 0;
    double totalVAT = 0;
    double totalUSD = 0;
    int cnt = 0;
    for (int i = 0, count = fReportGrid->fModel->rowCount(); i < count; i++) {
        if (fReportGrid->fModel->data(i, 4).toInt() != 30) {
            continue;
        }
        if (mode.isEmpty()) {
            mode = fReportGrid->fModel->data(i, 9).toString();
        } else {
            if (mode != fReportGrid->fModel->data(i, 9).toString()) {
                QList<QVariant> row = emptyRow;
                row[10] = totalAMD;
                row[11] = totalVAT;
                row[12] = totalUSD;
                fReportGrid->fModel->insertRow(i, row);
                fReportGrid->fTableView->setSpan(i, 0, 1, 10);
                i++;
                row = emptyRow;
                fReportGrid->fModel->insertRow(i, row);
                fReportGrid->fTableView->setSpan(i, 0, 1, 14);
                i++;
                mode = fReportGrid->fModel->data(i, 9).toString();
                totalAMD = 0;
                totalUSD = 0;
                totalVAT = 0;
            }
        }
        cnt = i;
        totalAMD += fReportGrid->fModel->data(i, 10, Qt::EditRole).toDouble();
        totalVAT += fReportGrid->fModel->data(i, 11, Qt::EditRole).toDouble();
        totalUSD += fReportGrid->fModel->data(i, 12, Qt::EditRole).toDouble();
    }
    if (cnt > 0) {
        QList<QVariant> row = emptyRow;
        row[10] = totalAMD;
        row[11] = totalVAT;
        row[12] = totalUSD;
        cnt++;
        fReportGrid->fModel->insertRow(cnt, row);
        fReportGrid->fTableView->setSpan(cnt, 0, 1, 10);
        cnt++;
        row = emptyRow;
        fReportGrid->fModel->insertRow(cnt, row);
        fReportGrid->fTableView->setSpan(cnt, 0, 1, 14);
    }
}

void FDailyMovement::processItems(QString items)
{
    bool totalRowSeparate = items.contains("+");
    if (totalRowSeparate) {
        items.remove("+");
    }
    DatabaseResult dr;
    QString query = fReportGrid->fStaticQuery;
    query.replace(":f_wdate", ui->deStart->dateMySql());
    query.replace(":f_canceled", ui->chCanceled->isChecked() ? "1" : "0");
    query.replace(":where", QString(" and f_itemCode in (%1)").arg(items));
    query.replace(":order", GODaily::value("sort order", "Daily movement").toString());
    dr.select(fDb, query, fDbBind);
    if (dr.rowCount() == 0) {
        return;
    }
    QList<QVariant> emptyRow;
           //.  1      2     3     4     5    6    7     8      9    10    11    12    13    14    15    16    17    18    19
    emptyRow << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    QList<QList<QVariant> > rows;
    QMap<int, double> payment;
    for (int i = 0; i < dr.rowCount(); i++) {
        if (dr.value(i, "f_source").toString() == VAUCHER_RECEIPT_N) {
            if (dr.value(i, "f_paymentMode").toInt() == PAYMENT_CL) {
                continue;
            }
        }
        QList<QVariant> oneRow;
        for (int j = 0, count = dr.columnCount(); j < count; j++) {
            oneRow << dr.value(i, j);
        }
        payment[dr.value(i, "f_paymentMode").toInt()] = payment[dr.value(i, "f_paymentMode").toInt()] + dr.value(i, "f_amountAmd").toDouble();
        payment[-1] = payment[-1] + dr.value(i, "f_amountAmd").toDouble();
        payment[-2] = payment[-2] + dr.value(i, "f_amountVat").toDouble();
        payment[-3] = payment[-2] + dr.value(i, "f_amountUsd").toDouble();
        rows << oneRow;
    }
    QList<QVariant> trr = emptyRow;
    trr[10] = payment[-1];
    trr[11] = payment[-2];
    trr[12] = payment[-3];
    QStringList itemCode = items.split(",", QString::SkipEmptyParts);
    QString names = tr("SUBTOTAL FOR ");
    foreach (QString s, itemCode) {
        CI_InvoiceItem *ci = CacheInvoiceItem::instance()->get(s);
        if (ci) {
            names += ci->fName + ",";
        }
    }
    names.remove(names.length() - 1, 1);
    if (totalRowSeparate) {
        names += " " + float_printout(payment[-1]) ;
        trr[0] = names;
        if (fPreferences.getDb(def_daily_movement_total_side).toInt() == 1) {
            rows << trr;
        } else {
            rows.insert(0, trr);
        }
        trr[10] = "";
        trr[11] = "";
        trr[12] = "";
        for (QMap<int, double>::const_iterator it = payment.begin(); it != payment.end(); it++) {
            if (it.key() < 0) {
                continue;
            }
            CI_PaymentMode *ci = CachePaymentMode::instance()->get(it.key());
            if (ci) {
                names = QString("%1: %2").arg(ci->fName).arg(float_printout(it.value()));
            } else {
                names = QString("%1: %2").arg("UNKNOWN").arg(float_printout(it.value()));
            }
            trr[0] = names;
            if (fPreferences.getDb(def_daily_movement_total_side).toInt() == 1) {
                rows << trr;
            } else {
                rows.insert(0, trr);
            }
        }
    } else {
        names += " " + float_printout(payment[-1]) + " [";
        for (QMap<int, double>::const_iterator it = payment.begin(); it != payment.end(); it++) {
            if (it.key() < 0) {
                continue;
            }
            CI_PaymentMode *ci = CachePaymentMode::instance()->get(it.key());
            if (ci) {
                names += QString("%1: %2,").arg(ci->fName).arg(float_printout(it.value()));
            } else {
                names += QString("%1: %2,").arg("UNKNOWN").arg(float_printout(it.value()));
            }
        }
        names.remove(names.length() - 1, 1);
        names += "]";
        trr[0] = names;
        if (fPreferences.getDb(def_daily_movement_total_side).toInt() == 1) {
            rows << trr;
        } else {
            rows.insert(0, trr);
        }
    }
    rows << emptyRow;
    fReportGrid->fDbRows << rows;
}

void FDailyMovement::invoiceItem(CI_InvoiceItem *c)
{
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leSale, c);
}

void FDailyMovement::openInvoice()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    QString inv = out.at(16).toString();
    if (inv.isEmpty()) {
        return;
    }
    openVaucherInvoice(inv);
}

void FDailyMovement::on_btnPrevDate_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    apply(fReportGrid);
}

void FDailyMovement::on_btnNextDate_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    apply(fReportGrid);
}
