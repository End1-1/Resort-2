#include "dlgpaymentsdetails.h"
#include "ui_dlgpaymentsdetails.h"
#include "cachepaymentmode.h"
#include "pimage.h"
#include "ptextrect.h"
#include "pprintpreview.h"
#include "cacheinvoiceitem.h"
#include "pprintheader.h"
#include "pprintvaucher.h"
#include "printtax.h"
#include "paymentmode.h"
#include "vauchers.h"
#include <QInputDialog>

DlgPaymentsDetails::DlgPaymentsDetails(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPaymentsDetails)
{
    ui->setupUi(this);

    Utils::tableSetColumnWidths(ui->tblGuest, ui->tblGuest->columnCount(),
                                0, 120, 0, 200, 90, 90, 90, 200, 25, 30, 0);
    Utils::tableSetColumnWidths(ui->tblCompany, ui->tblCompany->columnCount(),
                                0, 120, 0, 200, 90, 90, 90, 200, 25, 30, 0);
    Utils::tableSetColumnWidths(ui->tblRefund, ui->tblRefund->columnCount(),
                                0, 120, 100, 100, 80, 200, 100, 30, 0);
    fDockCityLedger = new DWSelectorCityLedger(this);
    connect(fDockCityLedger, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));
    fDockCard = new DWSelectorCreditCard(this);
    connect(fDockCard, SIGNAL(creditCard(CI_CreditCard*)), this, SLOT(creditCard(CI_CreditCard*)));
    fDockRoom = new DWSelectorActiveRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->leRoomCode);
    connect(fDockRoom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(room(CI_ActiveRoom*)));
    fDockCityRefund = new DWSelectorCityLedger(this);
    fDockCityRefund->configure();
    connect(fDockCityRefund, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityRefund(CI_CityLedger*)));
    fTrackControl = new TrackControl(TRACK_RESERVATION);
    fTrackControl->fInvoice = ui->leInvoice->text();
    fTrackControl->fReservation = ui->leReservation->text();
    ui->lbCityLedger->setVisible(false);

    fCheckoutFlag = 0;
}

DlgPaymentsDetails::~DlgPaymentsDetails()
{
    delete ui;
}

void DlgPaymentsDetails::setInvoice(const QString &id)
{
    ui->leInvoice->setText(id);
    fDbBind[":invoice"] = id;
    fDb.select("select r.f_room, rm.f_short, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName), "
               "r.f_cityLedger, cl.f_name, r.f_startDate, r.f_endDate, r.f_id "
               "from f_reservation r "
               "inner join f_guests g on g.f_id=r.f_guest "
               "inner join f_room rm on rm.f_id=r.f_room "
               "left join f_city_ledger cl on cl.f_id=r.f_cityLedger "
               "where r.f_invoice=:invoice", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leRoomCode->setText(fDbRows.at(0).at(0).toString());
        ui->leRoomName->setText(fDbRows.at(0).at(1).toString());
        ui->leGuest->setText(fDbRows.at(0).at(2).toString());
        ui->leCityLedgerCode->setText(fDbRows.at(0).at(3).toString());
        ui->leCityLedgerName->setText(fDbRows.at(0).at(4).toString());
        fArrival = fDbRows.at(0).at(5).toDate();
        fDeparture = fDbRows.at(0).at(6).toDate();
        ui->leReservation->setText(fDbRows.at(0).at(7).toString());
    }
    /*-------------------- BEGIN INVOICE------------------
    fDbBind[":f_group1"] = IG_POSTING_CHARGES;
    fDbBind[":f_group2"] = IG_DISCOUNT;
    fDbBind[":f_group3"] = IG_NEGATIVE_BALANCE;
    fDbBind[":f_group4"] = IG_POSITIVE_BALANCE;*/
    fDb.select("select sum(f_amountAmd * f_sign),sum(f_amountAmd / (f_amountUSD * f_sign)), 1 "
               "from m_register m "
               "where f_side=0 and f_finance=1 and f_canceled=0 and f_inv=" + ap(ui->leInvoice->text()) + " "
               "and f_sign=1  "
               " union "
               "select sum(f_amountAmd * f_sign),sum(f_amountUSD * f_sign), 2 "
               "from m_register "
               "where f_side=1 and f_finance=1 and f_canceled=0 and f_inv=" + ap(ui->leInvoice->text()) + " "
               "and f_sign=1  ",
               fDbBind, fDbRows);
    foreach_rows {
        if (it->at(2).toInt() == 1) {
            ui->leTotalAmdGuest->setDouble(it->at(0).toDouble());
            ui->leTotalUSDGuest->setDouble(it->at(1).toDouble());
        } else {
            ui->leTotalCompAMD->setDouble(it->at(0).toDouble());
            ui->leTotalCompUSD->setDouble(it->at(1).toDouble());
        }
    }
    /*-------------------- END INVOICE ------------------*/

    /*------------------- BEGIN PAID --------------------*/
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDb.select("select m.f_id, m.f_wdate, m.f_itemCode, m.f_finalName, "
               "m.f_paymentComment, m.f_amountAmd, "
               "m.f_amountAmd / m.f_amountUSD, m.f_remarks, m.f_side "
               "from m_register m "
               "where m.f_inv=:f_invoice and m.f_canceled=0 and f_finance=1 "
               "and m.f_sign=-1", fDbBind, fDbRows);
    QTableWidget *tg = ui->tblGuest;
    QTableWidget *tc = ui->tblCompany;
    tg->setRowCount(0);
    tc->setRowCount(0);
    fDockCard->setSelector(0);
    fDockCityLedger->setSelector(0);
    fDockCityRefund->setSelector(0);
    foreach_rows {
        QTableWidget *t = it->at(8).toInt() == 0 ? tg : tc;
        int row = t->rowCount();
        t->setRowCount(row + 1);
        for (int i = 0, colCount = t->columnCount() - 1; i < colCount; i++) {
            createLineEdit(t, row, i);
        }
        QList<QVariant> values;
        values << it->at(0).toInt() // rec
               << it->at(1).toDate().toString(def_date_format) // date
               << it->at(2).toInt() // itemid
               << it->at(3).toString() // item name
               << it->at(4).toString() //payment detail
               << it->at(5).toDouble() //amount
               << it->at(6).toDouble() //amount ids
               << it->at(7).toString() // remarks
               << ""
               << ""
               << ""
                  ;
        setRowValues(t, row, values);
    }
    /*------------------- END PAID --------------------*/
    /*-------------- BEGIN REFUND ---------------------*/
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDb.select("select m.f_id, m.f_wdate, m.f_amountAmd, "
               "m.f_amountAmd/m.f_amountUsd, m.f_cityLedger, cl.f_name,"
               "m.f_remarks, '',  m.f_side "
               "from m_register m "
               "left join f_city_ledger cl on cl.f_id=m.f_cityLedger "
               "where m.f_inv=:f_invoice and m.f_canceled=0 "
               "and m.f_source='RF'", fDbBind, fDbRows);
    foreach_rows {
        Utils::tableAppendRowData(ui->tblRefund, *it, Qt::DisplayRole);
    }
    /*-------------- END REFUND ---------------------*/
    countRefund();
    countTotal();
    fTrackControl->resetChanges();
}

bool DlgPaymentsDetails::isZeroBalance()
{
    return float_equal(ui->leBalanceAMD->asDouble(), 0.0);
}

void DlgPaymentsDetails::setCheckoutFlag()
{
    fCheckoutFlag = 1;
}

void DlgPaymentsDetails::refundLineFocusIn()
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    fDockCityRefund->setSelector(l);
}

void DlgPaymentsDetails::refundRemove(int tag)
{
    tag = -1;
    EPushButton *b = static_cast<EPushButton*>(sender());
    for (int i = 0; i < ui->tblRefund->rowCount(); i++) {
        if (ui->tblRefund->cellWidget(i, 7) == b) {
            tag = i;
            break;
        }
    }
    if (tag < 0) {
        message_error("Application error. Contact to developer. Message: refundRemove, tag=-1");
        return;
    }
    ui->tblRefund->removeRow(tag);
    double totalAmd = ui->tblRefund->sumOfColumn(2);
    double totalUsd = ui->tblRefund->sumOfColumn(3);
    ui->leRefund->setDouble(totalAmd);
    ui->leRefundUSD->setDouble(totalUsd);
    countTotal();
}

void DlgPaymentsDetails::refundTextChanged(const QString &text)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    ui->tblRefund->setValue(l->fRow, l->fColumn + 1, text.toDouble() / def_usd);
    double totalAmd = ui->tblRefund->sumOfColumn(l->fColumn);
    double totalUsd = ui->tblRefund->sumOfColumn(l->fColumn + 1);
    ui->leRefund->setDouble(totalAmd);
    ui->leRefundUSD->setDouble(totalUsd);
    countTotal();
}

void DlgPaymentsDetails::room(CI_ActiveRoom *c)
{
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leRoomCode, ui->leRoomName, c);
    if (c) {
        ui->leInvoice->setText(c->fInvoice);
        setInvoice(c->fInvoice);
        ui->leRoomCode->setText(c->fRoomCode);
    }
}

void DlgPaymentsDetails::cityLedgerFocusIn()
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    fDockCityLedger->setSelector(l);
}

void DlgPaymentsDetails::cardFocusIn()
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    fDockCard->setSelector(l);
}

void DlgPaymentsDetails::cityLedger(CI_CityLedger *c)
{
    if (c) {
        QTableWidget *t  = ui->tabWidget->currentIndex() == 0 ? ui->tblGuest : ui->tblCompany;
        EQLineEdit *l = fDockCityLedger->selector();
        l->setText(c->fCode);
        l = lineEdit(t, l->fRow, 7);
        l->setText(c->fName);
        if (lineEdit(t, l->fRow, 10)->text() == "RV") {
            lineEdit(t, l->fRow, 3)->setText("PAYMENT " + c->fName);
        } else{
            lineEdit(t, l->fRow, 3)->setText("ADVANCE " + c->fName);
        }
    }
}

void DlgPaymentsDetails::cityRefund(CI_CityLedger *c)
{
    EQLineEdit *l = fDockCityRefund->selector();
    if (c) {
        l->setText(c->fCode);
        ui->tblRefund->item(l->fRow, 5)->setText(c->fName);
    }
}

void DlgPaymentsDetails::creditCard(CI_CreditCard *c)
{
    if (c) {
        QTableWidget *t  = ui->tabWidget->currentIndex() == 0 ? ui->tblGuest : ui->tblCompany;
        EQLineEdit *l = fDockCard->selector();
        if (!l) {
            return;
        }
        l->setText(c->fCode);
        l = lineEdit(t, l->fRow, 7);
        l->setText(c->fName);
        if (lineEdit(t, l->fRow, 10)->text() == "RV") {
             lineEdit(t, l->fRow, 3)->setText("PAYMENT " + c->fName);
        } else{
            lineEdit(t, l->fRow, 3)->setText("ADVANCE " + c->fName);
        }
    }
}

void DlgPaymentsDetails::removeRowClicked(int tag)
{
    Q_UNUSED(tag)
    EPushButton *btn = static_cast<EPushButton*>(sender());
    QTableWidget *t = ui->tabWidget->currentIndex() == 0 ? ui->tblGuest : ui->tblCompany;
    for (int i = 0; i < t->rowCount(); i++) {
        if (t->cellWidget(i, btn->fColumn) == btn) {
            for (int j = 0; j < t->columnCount(); j++) {
                EQLineEdit *l = lineEdit(t, i, j);
                if (fDockCard->selector() == l) {
                    fDockCard->setSelector(0);
                }
                if (fDockCityLedger->selector() == l) {
                    fDockCityLedger->setSelector(0);
                }
                if (l) {
                    delete l;
                }
            }
            t->removeRow(i);
        }
    }
    countTotal();
}

void DlgPaymentsDetails::amountLineEditEdited(const QString &text)
{
    EQLineEdit *leAmd = static_cast<EQLineEdit*>(sender());
    QTableWidget *t = leAmd->fTag == 0 ? ui->tblGuest : ui->tblCompany;
    EQLineEdit *leUsd = lineEdit(t, leAmd->fRow, leAmd->fColumn + 1);
    leUsd->setDouble(text.toDouble() / def_usd);
    countTotal();
}

void DlgPaymentsDetails::countTotal()
{
    ui->lePaidAMDGuest->setDouble(countTotal(ui->tblGuest, 0));
    ui->lePaidUSDGuest->setDouble(countTotal(ui->tblGuest, 1));
    ui->leBalanceAMDGuest->setDouble(ui->leTotalAmdGuest->asDouble() + ui->leRefund->asDouble() - ui->lePaidAMDGuest->asDouble());
    ui->leBalanceUSDGuest->setDouble(ui->leTotalUSDGuest->asDouble() + ui->leRefundUSD->asDouble() - ui->lePaidUSDGuest->asDouble());
    ui->lePaidCompAMD->setDouble(countTotal(ui->tblCompany, 0));
    ui->lePaidCompUSD->setDouble(countTotal(ui->tblCompany, 1));
    ui->leBalanceCompAMD->setDouble(ui->leTotalCompAMD->asDouble() - ui->lePaidCompAMD->asDouble());
    ui->leBalanceCompUSD->setDouble(ui->leTotalCompUSD->asDouble() - ui->lePaidCompUSD->asDouble());
    ui->leTotalAMD->setDouble(ui->leTotalAmdGuest->asDouble() + ui->leTotalCompAMD->asDouble());
    ui->leTotalUSD->setDouble(ui->leTotalUSDGuest->asDouble() + ui->leTotalCompUSD->asDouble());
    ui->lePaidAMD->setDouble(ui->lePaidAMDGuest->asDouble() + ui->lePaidCompAMD->asDouble());
    ui->lePaidUSD->setDouble(ui->lePaidUSDGuest->asDouble() + ui->lePaidCompUSD->asDouble());
    ui->leBalanceAMD->setDouble(ui->leBalanceAMDGuest->asDouble() + ui->leBalanceCompAMD->asDouble());
    ui->leBalanceUSD->setDouble(ui->leBalanceUSDGuest->asDouble() + ui->leBalanceCompUSD->asDouble());
}

void DlgPaymentsDetails::printPayment(EQTableWidget *t, QList<int> &rows)
{
    foreach (int r, rows) {
        PPrintVaucher::printVaucher(t->toString(r, 0));
    }
}

double DlgPaymentsDetails::countTotal(QTableWidget *t, int curr)
{
    int col = 5 + curr;
    double result = 0;
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        EQLineEdit *l = lineEdit(t, i, col);
        result += l->asDouble();
    }
    return result;
}

void DlgPaymentsDetails::countRefund()
{
    ui->leRefund->setDouble(ui->tblRefund->sumOfColumn(2));
    ui->leRefundUSD->setDouble(ui->tblRefund->sumOfColumn(3));
}

void DlgPaymentsDetails::newPaidRow(int mode)
{
    if (mode == PAYMENT_CL) {
        if (!fCheckoutFlag) {
            message_error_tr("This payment mode available after checkout");
            return;
        }
    }
    int side = ui->tabWidget->currentIndex();
    if (side == 2) {
        newRefundRow(mode);
        return;
    }
    QString rv = "RV";
    QString finalName = "PAYMENT";
    double amount = side == 0 ? ui->leBalanceAMDGuest->asDouble() : ui->leBalanceCompAMD->asDouble();
    if (amount < 0.01) {
        amount = 0;
        rv = "RV";
        finalName = "PAYMENT";
    }

//    CI_InvoiceItem *ci = CacheInvoiceItem::instance()->get(QString::number(mode));
//    if (!ci) {
//        return;
//    }
//    if (ci->fGroup != IG_PAYMENT) {
//        message_error_tr("This method is not allowed for refund");
//        return;
//    }
    CI_PaymentMode *ci = CachePaymentMode::instance()->get(mode);
    QTableWidget *t = side == 0 ? ui->tblGuest : ui->tblCompany;
    int row = t->rowCount();
    t->setRowCount(row + 1);
    EQLineEdit *cardCLSelect = 0;
    for (int i = 0, colCount = t->columnCount(); i < colCount; i++) {
        EQLineEdit *l = createLineEdit(t, row, i);
        l->fTag = side;
        switch (i) {
        case 4:
            if (mode == PAYMENT_CL) {
                connect(l, SIGNAL(focusIn()), this, SLOT(cityLedgerFocusIn()));
                cardCLSelect = l;
            } else if (mode == PAYMENT_CARD) {
                connect(l, SIGNAL(focusIn()), this, SLOT(cardFocusIn()));
                cardCLSelect = l;
            }
            break;
        }
    }
    EPushButton *b = new EPushButton(this);
    b->setMaximumSize(25, 25);
    b->setMinimumSize(25, 25);
    b->setIcon(QIcon(":/images/garbage.png"));
    b->setTag(row);
    b->fColumn = 8;
    connect(b, SIGNAL(clickedWithTag(int)), this, SLOT(removeRowClicked(int)));
    t->setCellWidget(row, 8, b);
    QList<QVariant> values;
    values << ""
           << WORKING_DATE.toString(def_date_format)
           << ci->fCode
           << finalName + " " + ci->fName
           << ""
           << amount
           << float_str(amount / def_usd, 2)
           << ""
           << ""
           << ""
           << rv;
    setRowValues(t, row, values);
    EQLineEdit *l = lineEdit(t, row, 5);
    l->setValidator(new QDoubleValidator(0, 999999999, 2));
    l->setReadOnly(false);
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(amountLineEditEdited(QString)));
    countTotal();
    if (mode == PAYMENT_CL) {
        lineEdit(t, row, 4)->setText(ui->leCityLedgerCode->text());
        lineEdit(t, row, 7)->setText(ui->leCityLedgerName->text());
    }
    if (mode == PAYMENT_CL || mode == PAYMENT_CARD) {
        if (cardCLSelect) {
            cardCLSelect->setFocus();
            if (mode == PAYMENT_CARD) {
                emit cardCLSelect->customButtonClicked(true);
            }
        }
    }
}

void DlgPaymentsDetails::newRefundRow(int mode)
{
    int side = 0;
    switch (mode) {
    case PAYMENT_CASH:
    break;
    case PAYMENT_CARD:
        message_error_tr("This mode is not available for refund.");
        break;
    case PAYMENT_BANK:
        break;
    case PAYMENT_CL:
        side = 1;
        break;
    }

    int row = ui->tblRefund->rowCount();
    ui->tblRefund->setRowCount(row + 1);
    ui->tblRefund->setItemWithValue(row, 0, "");
    ui->tblRefund->setItemWithValue(row, 1, WORKING_DATE);
    ui->tblRefund->setItemWithValue(row, 2, 0);
    ui->tblRefund->setItemWithValue(row, 3, 0);
    ui->tblRefund->setItemWithValue(row, 4, 0);
    ui->tblRefund->setItemWithValue(row, 5, "");
    ui->tblRefund->setItemWithValue(row, 6, "");
    ui->tblRefund->setItemWithValue(row, 7, "");
    ui->tblRefund->setItemWithValue(row, 8, side);
    EQLineEdit *line = ui->tblRefund->addLineEdit(row, 2, true);
    line->setFont(ui->tblRefund->font());
    connect(line, SIGNAL(textChanged(QString)), this, SLOT(refundTextChanged(QString)));
    if (ui->leBalanceAMD->asDouble() < 0) {
        line->setDouble(ui->leBalanceAMD->asDouble() * -1);
    }
    if (mode == PAYMENT_CL) {
        EQLineEdit *lineCity = ui->tblRefund->addLineEdit(row, 4, true);
        connect(lineCity, SIGNAL(focusIn()), this, SLOT(refundLineFocusIn()));
        if (ui->leCityLedgerCode->asInt() == 0) {
            ui->tblRefund->lineEdit(row, 4)->setFocus();
        } else {
            ui->tblRefund->lineEdit(row, 4)->setText(ui->leCityLedgerCode->text());
            ui->tblRefund->setItemWithValue(row, 5, ui->leCityLedgerName->text());
        }
    }
    ui->tblRefund->addButton(row, 7, SLOT(refundRemove(int)), this, QIcon(":/images/garbage.png"));
    countRefund();
    countTotal();
}

void DlgPaymentsDetails::setRowValues(QTableWidget *t, int row, const QList<QVariant> &values)
{
    for (int i = 0, count = values.count(); i < count; i++) {
        t->setItem(row, i, new QTableWidgetItem(values.at(i).toString()));
        EQLineEdit *l = lineEdit(t, row, i);
        if (!l) {
            continue;
        }
        l->fRow = row;
        l->fColumn = i;
        l->fTable = t;
        connect(l, &EQLineEdit::textChanged, [this](const QString &text) {
            EQLineEdit *e = dynamic_cast<EQLineEdit*>(sender());
            if (!e) {
                return;
            }
            e->fTable->item(e->fRow, e->fColumn)->setText(text);
        });
        l->setText(values.at(i).toString());
    }
}

EQLineEdit *DlgPaymentsDetails::createLineEdit(QTableWidget *t, int row, int column)
{
    EQLineEdit *le = new EQLineEdit(this);
    le->setFont(t->font());
    le->setReadOnly(true);
    le->fRow = row;
    le->fColumn = column;
    le->setFrame(false);
    t->setCellWidget(row, column, le);
    return le;
}

EQLineEdit *DlgPaymentsDetails::lineEdit(QTableWidget *t, int row, int column)
{
    EQLineEdit *l = dynamic_cast<EQLineEdit*>(t->cellWidget(row, column));
    return l;
}

bool DlgPaymentsDetails::savePayment(QTableWidget *t, int side, QList<int> &printRows)
{
    if (fCheckoutFlag) {

    }
    //QString sideRemarks = (side == 0 ? "Payment, guest" : "Payment, company");
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        if (lineEdit(t, i, 5)->asDouble() < 0.1) {
            message_error_tr("Amount cannot be zero");
            return false;
        }
        int pmMode = lineEdit(t, i, 2)->text().toInt();
        EQLineEdit *l = lineEdit(t, i, 0);
        if (l->isEmpty()) {
            printRows << i;
            int creditCard = 0;
            int clCode = 0;
            QString modeName = "";
            switch (pmMode) {
            case PAYMENT_CASH:
                modeName = "CASH";
                break;
            case PAYMENT_CARD:
                creditCard = lineEdit(t, i, 4)->text().toInt();
                modeName = lineEdit(t, i, 7)->text();
                break;
            case PAYMENT_BANK:
                modeName = "BANK";
                break;
            case PAYMENT_CL:
                clCode = lineEdit(t, i, 4)->text().toInt();
                modeName = lineEdit(t, i, 7)->text();
                break;
            }

            QString finalName = QString("%1 #%2 %3")
                    .arg("PAYMENT")
                    .arg(ui->leInvoice->text())
                    .arg(modeName);
            if (fCheckoutFlag) {
                finalName = "STL. #" + ui->leInvoice->text() + " " + modeName;
            }

            fDb.fDb.transaction();
            QString rid = uuid(t->item(i, 10)->text(), fAirDb);
            fDb.insertId("m_register", rid);
            fDbBind[":f_source"] = t->item(i, 10)->text();
            fDbBind[":f_res"] = ui->leReservation->text();
            fDbBind[":f_wdate"] = WORKING_DATE;
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_room"] = ui->leRoomCode->text();
            fDbBind[":f_guest"] = ui->leGuest->text();
            fDbBind[":f_itemCode"] = fPreferences.getDb(def_receip_vaucher_id);
            fDbBind[":f_finalName"] = lineEdit(t, i, 3)->text();
            fDbBind[":f_amountAmd"] = lineEdit(t, i, 5)->text();
            fDbBind[":f_amountVat"] = 0;
            fDbBind[":f_amountUsd"] = def_usd;
            fDbBind[":f_fiscal"] = 0;
            fDbBind[":f_paymentMode"] = lineEdit(t, i, 2)->text();
            fDbBind[":f_creditCard"] = creditCard;
            fDbBind[":f_cityLedger"] = clCode;
            fDbBind[":f_paymentComment"] = modeName;
            fDbBind[":f_dc"] = "CREDIT";
            fDbBind[":f_sign"] = -1;
            fDbBind[":f_doc"] = "";
            fDbBind[":f_rec"] = "";
            fDbBind[":f_inv"] = ui->leInvoice->text();
            fDbBind[":f_finance"] = 1;
            fDbBind[":f_remarks"] = "";
            fDbBind[":f_canceled"] = 0;
            fDbBind[":f_cancelReason"] = "";
            fDbBind[":f_side"] = side;
            fDbBind[":f_cash"] = pmMode == PAYMENT_CL ? 0 : 1;
            fDb.update("m_register", fDbBind, where_id(ap(rid)));
            fDb.fDb.commit();
            t->item(i, 0)->setText(rid);
            lineEdit(t, i, 0)->setText(rid);

            fTrackControl->insert("Payment", t->item(i, 0)->text(), lineEdit(t, i, 3)->text() + " " + lineEdit(t, i, 5)->text());

            if (t->item(i, 10)->text() == "AV") {
                if (message_confirm(QString::fromUtf8("Տպել կանխավճարի ՀԴՄ՞ ") + lineEdit(t, i, 5)->text() + " AMD") == QDialog::Accepted) {
                    PrintTax::printAdvance(lineEdit(t, i, 5)->text().toDouble(), lineEdit(t, i, 2)->asInt(), rid);
                    fDbBind[":f_prepaid"] = lineEdit(t, i, 5)->text().toDouble();
                    fDbBind[":f_id"] = ui->leInvoice->text();
                    fDb.select("update m_v_invoice set f_prepaid=f_prepaid+:f_prepaid where f_id=:f_id", fDbBind, fDbRows);
                    fDbBind[":f_fiscal"] = 1;
                    fDb.update("m_register", fDbBind, where_id(ap(rid)));
                }
            }
            t->removeCellWidget(i, 8);
        }
    }
    return true;
}

void DlgPaymentsDetails::on_btnCash_clicked()
{
    newPaidRow(PAYMENT_CASH);
}

void DlgPaymentsDetails::on_btnCard_clicked()
{
    newPaidRow(PAYMENT_CARD);
}

void DlgPaymentsDetails::on_btnCityLedger_clicked()
{
    newPaidRow(PAYMENT_CL);
}

void DlgPaymentsDetails::on_btnBank_clicked()
{
    newPaidRow(PAYMENT_BANK);
}

void DlgPaymentsDetails::on_btnSave_clicked()
{
    if (fPreferences.getDb(def_invoice_default_refund_id).toInt() == 0) {
        message_error_tr("Refund id is not defined. Contact to administrator.");
        return;
    }
    QTableWidget *t = ui->tblGuest;
    for (int i = 0, rowCount = ui->tblGuest->rowCount(); i < rowCount; i++) {
        if (lineEdit(t, i, 2)->asInt() == PAYMENT_CARD ||
                lineEdit(t, i, 2)->asInt() == PAYMENT_CL) {
            if (lineEdit(t, i, 4)->asInt() == 0) {
                message_error_tr("Payment mode is credit card or city ledger, but account field is empty");
                return;
            }
        }
    }
    t = ui->tblCompany;
    for (int i = 0, rowCount = ui->tblCompany->rowCount(); i < rowCount; i++) {
        if (lineEdit(t, i, 2)->asInt() == PAYMENT_CARD ||
                lineEdit(t, i, 2)->asInt() == PAYMENT_CL) {
            if (lineEdit(t, i, 4)->asInt() == 0) {
                message_error_tr("Payment mode is credit card or city ledger, but account field is empty");
                return;
            }
        }
    }
    QList<int> printRefundRow;
    fDb.fDb.transaction();
    QList<int> printRows1;
    QList<int> printRows2;
    if (!savePayment(ui->tblGuest, 0, printRows1)) {
        if (printRows1.count() > 0) {
            printPayment(ui->tblGuest, printRows1);
        }
        return;
    }
    if (!savePayment(ui->tblCompany, 1, printRows2)) {
        if (printRows2.count() > 0) {
            printPayment(ui->tblCompany, printRows2);
        }
        return;
    }

    for (int i = 0, rowCount = ui->tblRefund->rowCount(); i < rowCount; i++) {
        if (ui->tblRefund->toString(i, 0).isEmpty()) {
            printRefundRow << i;
            fDb.fDb.transaction();
            QString rid = uuid(VAUCHER_REFUND_N, fAirDb);
            fDb.insertId("m_register", rid);
            fDbBind[":f_source"] = VAUCHER_REFUND_N;
            fDbBind[":f_wdate"] = WORKING_DATE;
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_room"] = ui->leRoomName->text();
            fDbBind[":f_guest"] = ui->leGuest->text();
            fDbBind[":f_itemCode"] = fPreferences.getDb(def_invoice_default_refund_id);
            fDbBind[":f_finalName"] = tr("REFUND ") + rid;
            fDbBind[":f_amountAmd"] = ui->tblRefund->toString(i, 2);
            fDbBind[":f_amountVat"] = 0;
            fDbBind[":f_amountUsd"] = def_usd;
            fDbBind[":f_fiscal"] = 0;
            fDbBind[":f_paymentMode"] = PAYMENT_CREDIT;
            fDbBind[":f_creditCard"] = 0;
            fDbBind[":f_cityLedger"] = ui->tblRefund->toInt(i, 4);
            fDbBind[":f_paymentComment"] = (ui->tblRefund->toInt(i, 4) ? ui->tblRefund->toString(i, 5) : "");
            fDbBind[":f_dc"] = "DEBIT";
            fDbBind[":f_sign"] = 1;
            fDbBind[":f_doc"] = "";
            fDbBind[":f_rec"] = "";
            fDbBind[":f_inv"] = ui->leInvoice->text();
            fDbBind[":f_finance"] = 1;
            fDbBind[":f_remarks"] = ui->tblRefund->toString(i, 6);
            fDbBind[":f_canceled"] = 0;
            fDbBind[":f_cancelReason"] = "";
            fDbBind[":f_side"] = ui->tblRefund->toInt(i, 8);
            fDbBind[":f_cash"] = 0;
            fDb.update("m_register", fDbBind, where_id(ap(rid)));
            ui->tblRefund->setValue(i, 0, rid);
            fTrackControl->insert("Refund", ui->tblRefund->toString(i, 2), "");
        }
    }
    fTrackControl->saveChanges();
    fDb.fDb.commit();
    if (printRows1.count() > 0) {
        printPayment(ui->tblGuest, printRows1);
    }
    if (printRows2.count() > 0) {
        printPayment(ui->tblCompany, printRows2);
    }
    if (printRefundRow.count() > 0) {
        foreach (int i, printRefundRow) {
            PPrintVaucher::printVaucher(ui->tblRefund->toString(i, 0));
        }
    }
}

void DlgPaymentsDetails::on_leRoomCode_returnPressed()
{
    room(CacheActiveRoom::instance()->get(ui->leRoomCode->text()));
}
