#include "winvoice.h"
#include "ui_winvoice.h"
#include "dlgpostingcharges.h"
#include "dlgpaymentsdetails.h"
#include "cachereservation.h"
#include "epushbutton.h"
#include "cacheroom.h"
#include "dlginvoicecancelation.h"
#include "dlggposorderinfo.h"
#include "dlgtransfer.h"
#include "dlgtracking.h"
#include "dlgdiscount.h"
#include "pprintinvoice.h"
#include "dlgtransferinvoiceamount.h"
#include "dlginvoiceprintoption.h"
#include "wreservation.h"
#include "dlgtaxback2.h"
#include "cachetaxmap.h"
#include "dlgpretax.h"
#include "vauchers.h"
#include "dlgcityadvance.h"
#include "dlgtaxback.h"
#include "dlgadvance.h"
#include "dlgprinttaxsideoption.h"
#include "dlgpostbreakfast.h"
#include "dlgprintrandomtax.h"
#include "cacheredreservation.h"
#include "dlgtaxreference.h"
#include <QProcess>
#include <QDir>

WInvoice::WInvoice(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WInvoice)
{
    ui->setupUi(this);
    ui->btnTaxPrint->setEnabled(false);
    QListIterator<QObject*> it(ui->wBtn->children());
    while (it.hasNext()) {
        QWidget *w = dynamic_cast<QWidget*>(it.next());
        if (w) {
            w->setEnabled(false);
        }
    }
    ui->teRemark->setReadOnly(true);
    QList<int> tblInvWidths;
    tblInvWidths << 0 << 0 << 100 << 200 << 80 << 80 << 30 << 200 << 0 << 0 << 27 << 0 << 0 << 0;
    for (int i = 0; i < tblInvWidths.count(); i++) {
        ui->tblInvLeft->setColumnWidth(i, tblInvWidths.at(i));
        ui->tblInvRight->setColumnWidth(i, tblInvWidths.at(i));
    }
    fDockActiveRoom = new DWSelectorActiveRoom(this);
    fDockActiveRoom->configure();
    fDockActiveRoom->setSelector(ui->leRoomCode);
    connect(fDockActiveRoom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomChanged(CI_ActiveRoom*)));
    ui->btnPostingCharges->setEnabled(r__(cr__postchage_vaucher));
    ui->btnPaymentsDetails->setEnabled(r__(cr__receipt_vaucher));
    ui->btnDiscount->setEnabled(r__(cr__discount_vaucher));
    ui->btnTransfer->setEnabled(r__(cr__transfer_vaucher));
    ui->btnTransferAmount->setEnabled(r__(cr__transfer_vaucher));
}

WInvoice::~WInvoice()
{
    delete ui;
}

void WInvoice::loadInvoice(const QString &id)
{
    ui->btnTaxPrint->setEnabled(false);
    if (fTrackControl) {
        if (fTrackControl->hasChanges()) {
            fTrackControl->saveChanges();
            if (ui->leInvoice->notEmpty()) {
                fDbBind[":f_remarks"] = ui->teRemark->toPlainText();
                fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserveID->text())));
            }
        }
    }
    QString oldInvoice = ui->leInvoice->text();
    ui->leInvoice->setText(id);
    QString query = "select rs.f_invoice, rs.f_id, 'invoice date', rs.f_room, "
            "r.f_short as f_room_short, concat(g.f_firstName, ' ', g.f_lastName) as f_guest_name, "
            "g.f_passport, rs.f_cardex, c.f_name, rs.f_pricePerNight, rs.f_remarks, rs.f_startDate, rs.f_endDate,"
            "rs.f_man+rs.f_woman+rs.f_child, gs.total, rs.f_vatMode, v.f_" + def_lang +", "
            "rs.f_checkInTime, 0 as i_f_prepaid, ucheckin.f_username, rs.f_cityLedger, cl.f_name, "
            "ra.f_" + def_lang + ", nights.ntotal, rs.f_booking "
            "from  f_reservation rs "
            "left join f_room r on rs.f_room=r.f_id "
            "left join f_guests g on rs.f_guest=g.f_id "
            "left join f_cardex c on c.f_cardex=rs.f_cardex "
            "left join f_vat_mode v on v.f_id=rs.f_vatMode "
            "left join users ucheckin on ucheckin.f_id=rs.f_checkInUser "
            "left join (select rg.f_reservation, count(f_id) as total from f_reservation_guests rg group by 1) gs on gs.f_reservation=rs.f_id "
            "left join f_city_ledger cl on cl.f_id=rs.f_cityLedger "
            "left join f_room_arrangement ra on ra.f_id=rs.f_arrangement "
            "left join (select f_inv, count(f_id) as ntotal from m_register where f_canceled=0 and f_source='RM' and f_inv=" + ap(ui->leInvoice->text()) + ") nights on nights.f_inv=rs.f_invoice "
            "where rs.f_invoice=" + ap(id);
    fDb.select(query, fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Invoice number wrong");
        return;
    }
    QList<QVariant> &row = fDbRows[0];
    int c = 0;
    c++; //Skip invoice id
    ui->leReserveID->setText(row.at(c++).toString());
    c++; //Skip invoice date
    ui->leRoomCode->setText(row.at(c++).toString());
    ui->leRoom->setText(row.at(c++).toString());
    ui->leGuest->setText(row.at(c++).toString());
    ui->lePassport->setText(row.at(c++).toString());
    ui->leCardex->setText(row.at(c++).toString());
    ui->leCardexName->setText(row.at(c++).toString());
    ui->leRoomRate->setText(row.at(c++).toString());
    ui->teRemark->setPlainText(row.at(c++).toString());
    ui->deCheckin->setDate(row.at(c++).toDate());
    ui->deDeparture->setDate(row.at(c++).toDate());
//    ui->leNights->setText(QString::number(ui->deCheckin->date().daysTo(ui->deDeparture->date())));
    ui->leGuestCount->setText(row.at(c++).toString());
    ui->leRegisteredGuest->setText(row.at(c++).toString());
    ui->leVatMode->fHiddenText = row.at(c++).toString();
    ui->leVatMode->setText(row.at(c++).toString());
    ui->leCheckInTime->setText(row.at(c++).toString());
    ui->lePrepaid->setDouble(row.at(c++).toDouble());
    ui->leCheckInOperator->setText(row.at(c++).toString());
    ui->leCityLedgerCode->setText(row.at(c++).toString());
    ui->leCityLedgerName->setText(row.at(c++).toString());
    ui->leArrangement->setText(row.at(c++).toString());
    ui->leNights->setText(QString("%1 / %2").arg(row.at(c++).toString()).arg(ui->deCheckin->date().daysTo(ui->deDeparture->date())));
    ui->leBookingNo->setText(row.at(c++).toString());
    /* -------------------------- BEGIN CONTENT --------------------------*/
    ui->tblInvLeft->clearContents();
    ui->tblInvRight->clearContents();
    //////////////////////////////////////////////////////////////////////////////  content
    query = "select m.f_id, m.f_sign, m.f_wdate, m.f_finalName, "
            "m.f_amountAmd, m.f_amountVat, m.f_fiscal, m.f_remarks, m.f_source, m.f_doc, "
            "m.f_side, m.f_itemCode, m.f_source, m.f_paymentMode "
            "from m_register m "
            "where m.f_inv=" + ap(ui->leInvoice->text()) + " " +
            "and m.f_canceled=0 and m.f_finance=1 "
            "order by m.f_wdate, m.f_time ";
    fDb.select(query, fDbBind, fDbRows);
    ui->tblInvLeft->clearContents();
    ui->tblInvRight->clearContents();
    ui->tblInvLeft->setRowCount(0);
    ui->tblInvRight->setRowCount(0);
    QFont f(qApp->font());
    f.setBold(true);
    foreach_rows {
        QTableWidget *side = 0;
        if (it->at(10).toInt() == 0) {
            side = ui->tblInvLeft;
        } else {
            side = ui->tblInvRight;
        }
        side->setRowCount(side->rowCount() + 1);
        int row = side->rowCount() - 1;
        side->setItem(row, 0, new QTableWidgetItem(it->at(0).toString())); // rec id
        side->setItem(row, 1, new QTableWidgetItem(it->at(1).toString())); // sign
        side->setItem(row, 2, new QTableWidgetItem(it->at(2).toString())); // date
        side->setItem(row, 3, new QTableWidgetItem(it->at(3).toString())); // item name
        side->setItem(row, 4, new QTableWidgetItem(float_str(it->at(4).toDouble(), 2))); // amount
        side->setItem(row, 5, new QTableWidgetItem(it->at(5).toString())); // vat
        side->setItem(row, 6, new QTableWidgetItem(it->at(6).toString())); // tax
        side->setItem(row, 7, new QTableWidgetItem(it->at(7).toString())); // remarks
        side->setItem(row, 8, new QTableWidgetItem(it->at(8).toString())); // source
        side->setItem(row, 9, new QTableWidgetItem(it->at(9).toString())); //source id
        side->setItem(row, 10, new QTableWidgetItem()); // btn
        side->setItem(row, 11, new QTableWidgetItem(it->at(11).toString())); // item id
        side->setItem(row, 12, new QTableWidgetItem(it->at(12).toString())); // item group
        side->setItem(row, 13, new QTableWidgetItem(it->at(13).toString())); // Payment mode
        /// tax chebox
        side->item(row, 6)->setCheckState(it->at(6).toInt() > 0 ? Qt::Checked : Qt::Unchecked);
        side->item(row, 6)->setFlags(side->item(row, 6)->flags() ^ Qt::ItemIsUserCheckable);
        side->setItem(row, 7, new QTableWidgetItem(it->at(7).toString()));
        /// button for view of restaurant invoice details
        if (side->item(row, 8)->text().toInt() == 2) {
            EPushButton *b = new EPushButton(this);
            b->setIcon(QIcon(":/images/search.png"));
            QSize size(25, 25);
            b->setMaximumSize(size);
            b->setMinimumSize(size);
            b->setTag(it->at(9).toInt());
            connect(b, SIGNAL(clickedWithTag(int)), this, SLOT(viewGPOSOrder(int)));
            side->setCellWidget(row, 10, b);
        }
        if (it->at(1).toInt() < 0) {
            for (int i = 0; i < side->columnCount(); i++) {
                side->item(row, i)->setFont(f);
            }
        }
    }
    /* -------------------------- END CONTENT --------------------------*/

    /* advance */
    DatabaseResult dr;
    fDbBind[":f_inv"] = ui->leInvoice->text();
    dr.select(fDb, "select sum(f_amountAmd) as f_advance from m_register where f_Canceled=0 and f_inv=:f_inv and f_source='AV' and f_fiscal=1", fDbBind);
    if (dr.rowCount() > 0) {
        ui->lePrepaid->setDouble(dr.value("f_advance").toDouble());
    }
    /* end advance */

    countTotals();
    if (fTrackControl) {
        fTrackControl->resetChanges();
    }
    startTracking();
    if (oldInvoice != ui->leInvoice->text()) {
        fTrackControl->insert("Open invoice", "", "");
    }
    fTrackControl->saveChanges();
    ui->teRemark->setReadOnly(false);
    setupTabTextAndIcon(tr("Invoice for room") + " #" + ui->leRoomCode->text(), ":/images/invoice.png");
    QListIterator<QObject*> it(ui->wBtn->children());
    while (it.hasNext()) {
        QWidget *w = dynamic_cast<QWidget*>(it.next());
        if (w) {
            w->setEnabled(true);
        }
    }
    ui->btnTaxPrint->setEnabled(true);
}

void WInvoice::loadReservation(const QString &id)
{
    fDbBind[":f_id"] = id;
    fDb.select("select f_invoice from f_reservation where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        loadInvoice(fDbRows.at(0).at(0).toString());
    }
}

void WInvoice::startTracking()
{
    if (fTrackControl == 0) {
        fTrackControl = new TrackControl(TRACK_RESERVATION);
        fTrackControl->fRecord = "-";
        fTrackControl->fInvoice = ui->leInvoice->text();
        fTrackControl->fReservation = ui->leReserveID->text();
        fTrackControl->addWidget(ui->leTotalLeft, "Guest total")
            .addWidget(ui->leTotalRight, "Company total")
            .addWidget(ui->leGranTotal, "Grand total")
            .addWidget(ui->teRemark, "Remarks");
    }
}

bool WInvoice::activeDoc(const QString &invoice)
{
    if (invoice == ui->leInvoice->text()) {
        return true;
    }
    return false;
}

void WInvoice::openInvoiceWindow(const QString &invoice)
{
    WInvoice *w = 0;
    for (int i = 0; i < fMainWindow->fTab->count(); i++) {
        w = dynamic_cast<WInvoice*>(fMainWindow->fTab->widget(i));
        if (w) {
            if (w->activeDoc(invoice)) {
                fMainWindow->fTab->setCurrentIndex(i);
                return;
            }
        }
    }
    w = addTab<WInvoice>();
    w->loadInvoice(invoice);
}

void WInvoice::setupTab()
{
    setupTabTextAndIcon(tr("Invoice"), ":/images/invoice.png");
}

bool WInvoice::canClose()
{
    bool canClose = true;
    if (fTrackControl) {
        canClose = canClose && (ui->leInvoice->notEmpty() && !fTrackControl->hasChanges());
    }
    if (!canClose) {
        int result = message_yesnocancel("Invoice was modifed, save changes?");
        switch (result) {
        case RESULT_YES:
            save();
            canClose = true;
            break;
        case RESULT_NO:
            canClose = true;
            break;
        case RESULT_CANCEL:
            canClose = false;
            break;
        }
    }
    return canClose;
}

void WInvoice::roomChanged(CI_ActiveRoom *c)
{
    if (ui->leInvoice->notEmpty()) {
        if (fTrackControl) {
            if (fTrackControl->hasChanges()) {
                fTrackControl->saveChanges();
                save();
            }
        }
    }
    if (c) {
        if (c->fValid) {
            if (ui->leInvoice->notEmpty()) {
                if (ui->leInvoice->text() != c->fInvoice) {
                    loadInvoice(c->fInvoice);
                }
            } else {
                loadInvoice(c->fInvoice);
            }
        } else {
            clearInvoice();
            if (ui->leRoomCode->asInt() > 0) {
                message_error_tr("No active invoice on this room or wrong room number");
            }
            ui->leRoomCode->setFocus();
            ui->leRoomCode->selectAll();
        }
    } else {
        QListIterator<QObject*> it(ui->wBtn->children());
        while (it.hasNext()) {
            QWidget *w = dynamic_cast<QWidget*>(it.next());
            if (w) {
                w->setEnabled(false);
            }
        }
        clearInvoice();
        if (ui->leRoomCode->asInt() > 0) {
            message_error_tr("No active invoice on this room or wrong room number");
        }
    }
}

void WInvoice::viewGPOSOrder(const QString &id)
{
    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(id);
    d->exec();
    delete d;
}

void WInvoice::on_btnShowLeft_clicked()
{
    ui->wdtLeft->setVisible(!ui->wdtLeft->isVisible());
}

void WInvoice::on_btnShowRight_clicked()
{
    ui->wdtRight->setVisible(!ui->wdtRight->isVisible());
}

void WInvoice::on_leRoomCode_returnPressed()
{
    roomChanged(CacheActiveRoom::instance()->get(ui->leRoomCode->text()));
}

void WInvoice::on_btnLeft_clicked()
{
    moveTableRow(ui->tblInvRight, ui->tblInvLeft);
    countTotals();
}

void WInvoice::moveTableRow(QTableWidget *from, QTableWidget *to)
{
    QModelIndexList sel = from->selectionModel()->selectedRows();
    QString fromTo = QString("%1->%2")
            .arg((from == ui->tblInvLeft ? tr("Guest") : tr("Company")))
            .arg((from == ui->tblInvLeft ? tr("Company") : tr("Guest")));
    for (int i = sel.count() - 1; i > -1; i--) {
        int rowTo = to->rowCount();
        to->setRowCount(rowTo + 1);
        for (int j = 0; j < from->columnCount(); j++) {
            to->setItem(rowTo, j, new QTableWidgetItem(*(from->item(sel.at(i).row(), j))));
        }
        fTrackControl->insert("Entry moved", fromTo,
                                  from->item(sel.at(i).row(), 2)->text() + " / " +
                                  from->item(sel.at(i).row(), 4)->text() + " / " +
                                  from->item(sel.at(i).row(), 5)->text());
        from->removeRow(sel.at(i).row());
    }
    for (int i = 0; i < ui->tblInvLeft->rowCount(); i++) {
        fDbBind[":f_side"] = 0;
        fDb.update("m_register", fDbBind, where_id(ap(ui->tblInvLeft->item(i, 0)->text())));
    }
    for (int i = 0; i < ui->tblInvRight->rowCount(); i++) {
        fDbBind[":f_side"] = 1;
        fDb.update("m_register", fDbBind, where_id(ap(ui->tblInvRight->item(i, 0)->text())));
    }
}

void WInvoice::on_btnDoubleLeft_clicked()
{
    ui->tblInvRight->selectAll();
    moveTableRow(ui->tblInvRight, ui->tblInvLeft);
    countTotals();
}

void WInvoice::on_btnRight_clicked()
{
    moveTableRow(ui->tblInvLeft, ui->tblInvRight);
    countTotals();
}

void WInvoice::countTotals()
{
    double left = countTotal(ui->tblInvLeft);
    double right = countTotal(ui->tblInvRight);
    ui->leTotalLeft->setDouble(left);
    ui->leTotalRight->setDouble(right);
    ui->leGranTotal->setDouble(left + right);
}

double WInvoice::countTotal(QTableWidget *t)
{
    double result = 0.00;
    for (int i = 0; i < t->rowCount(); i++) {
        double amount = t->item(i, 4)->data(Qt::DisplayRole).toDouble();
        int sign = t->item(i, 1)->data(Qt::DisplayRole).toDouble();
        result += amount * sign;
    }
    return result;
}

void WInvoice::on_btnDoubleRight_clicked()
{
    ui->tblInvLeft->selectAll();
    moveTableRow(ui->tblInvLeft, ui->tblInvRight);
    countTotals();
}

void WInvoice::on_btnRefresh_clicked()
{
    QString fileName = "tax_" + ui->leInvoice->text() + ".json" ;
    QDir d;
    QStringList args;
    args << fPreferences.getDb(def_tax_address).toString()
         << fPreferences.getDb(def_tax_port).toString()
         << fPreferences.getDb(def_tax_password).toString()
         << fileName;
    QProcess *pr = new QProcess();
    pr->start(d.currentPath() + "/printtax.exe", args);
    if (ui->leInvoice->notEmpty()) {
        loadInvoice(ui->leInvoice->text());
    }
}

void WInvoice::on_tblInvRight_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    ui->tblInvLeft->selectionModel()->clear();
}

void WInvoice::on_tblInvLeft_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    ui->tblInvRight->selectionModel()->clear();
}

void WInvoice::on_btnPostingCharges_clicked()
{
    DlgPostingCharges *p = new DlgPostingCharges(this);
    p->setRoom(ui->leRoomCode->text());
    p->exec();
    delete p;
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnCheckout_clicked()
{
    save();
    if (QTime::currentTime() > fPreferences.getDb(def_late_checkout).toTime()) {
        /// check, if not already exists late checkout item
        bool lateOutExists = false;
        for_row_count(ui->tblInvLeft) {
            if (ui->tblInvLeft->toInt(i, 3) == fPreferences.getDb(def_late_checkout_id).toInt() && ui->tblInvLeft->toInt(i, 1) == 1) {
                lateOutExists = true;
                break;
            }
        }
        if (!lateOutExists) {
            for_row_count(ui->tblInvRight) {
                if (ui->tblInvRight->toInt(i, 3) == fPreferences.getDb(def_late_checkout_id).toInt() && ui->tblInvRight->toInt(i, 1) == 1) {
                    lateOutExists = true;
                    break;
                }
            }
        }
        if (!lateOutExists) {
            switch (message_yesnocancel("Charge late checkout?")) {
            case RESULT_YES: {
                if (fPreferences.getDb(def_late_checkout_id).toInt() == 0) {
                    message_error_tr("Late checkout id not defined. Contact to administrator");
                    return;
                }
                DlgPostingCharges *p = new DlgPostingCharges(this);
                p->setRoom(ui->leRoomCode->text());
                p->setSaleItem(fPreferences.getDb(def_late_checkout_id).toString());
                p->exec();
                delete p;
                loadInvoice(ui->leInvoice->text());
                break;
            }
            case RESULT_NO:
                break;
            case RESULT_CANCEL:
                return;
            }
        }
    }
    ////////////////////////////////////// check balance
    if (ui->leGranTotal->asDouble() > 0.1 || ui->leGranTotal->asDouble() < -0.1) {
        DlgPaymentsDetails *d = new DlgPaymentsDetails(this);
        d->setInvoice(ui->leInvoice->text());
        d->setCheckoutFlag();
        d->exec();
        delete d;
        loadInvoice(ui->leInvoice->text());
         if (ui->leGranTotal->asDouble() > 0.1 || ui->leGranTotal->asDouble() < -0.1) {
            message_error_tr("Balance not equal to zero");
            return;
         }
    }
    if (message_confirm_tr("Confirm to checkout") != QDialog::Accepted) {
        return;
    }
    bool result = true;
    if (result) {
        // RESERVATION
        fDb.fDb.transaction();
        fDbBind[":f_state"] = RESERVE_CHECKOUT;
        fDbBind[":f_checkOutTime"] = QTime::currentTime();
        fDbBind[":f_checkOutUser"] = WORKING_USERID;
        result = result && fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserveID->text())));
        fTrackControl->insert("Checkout", "", "");
    }
    if (result) {
        //ROOM
        fDbBind[":f_state"] = ROOM_STATE_DIRTY;
        result = result && fDb.update("f_room", fDbBind, where_id(ui->leRoomCode->asInt()));

        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_oldState"] = ROOM_STATE_CHECKIN;
        fDbBind[":f_newState"] = ROOM_STATE_DIRTY;
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comment"] = "CHECKOUT";
        fDbBind[":f_reservation"] = ui->leReserveID->text();
        fDb.insert("f_room_state_change", fDbBind);
    }
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDb.select("select f_cityLedger, sum(f_amountAmd*f_sign) from m_register where f_cityLedger>0 and f_inv=:f_inv and f_canceled=0", fDbBind, fDbRows);
    double cityAmount =0;
    int cityCode = 0;
    if (fDbRows.count() > 0) {
        cityCode = fDbRows.at(0).at(0).toInt();
        cityAmount = fDbRows.at(0).at(1).toDouble();
    }
    QString cid = uuuid(VAUCHER_CHECKOUT_N, fAirDb);
    fDb.insertId("m_register", cid);
    fDbBind[":f_source"] = VAUCHER_CHECKOUT_N;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_room"] = ui->leRoomCode->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_itemCode"] = 38;
    fDbBind[":f_finalName"] = tr("CHECKOUT ") + cid;
    fDbBind[":f_amountAmd"] = 0;
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = 0;
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = 0;
    fDbBind[":f_paymentComment"] = "";
    fDbBind[":f_dc"] = "DEBIT";
    fDbBind[":f_sign"] = 1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 0;
    fDbBind[":f_remarks"] = "";
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(cid)));
    if (cityAmount > 0.1 || cityAmount < -0.1) {
        cid = uuuid(VAUCHER_CHECKOUT_N, fAirDb);
        fDb.insertId("m_register", cid);
        fDbBind[":f_source"] = VAUCHER_CHECKOUT_N;
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_room"] = ui->leRoomCode->text();
        fDbBind[":f_guest"] = ui->leGuest->text();
        fDbBind[":f_itemCode"] = 38;
        fDbBind[":f_finalName"] = tr("CHECKOUT C/L ") + cid;
        fDbBind[":f_amountAmd"] = cityAmount;
        fDbBind[":f_amountVat"] = 0;
        fDbBind[":f_amountUsd"] = def_usd;
        fDbBind[":f_fiscal"] = 0;
        fDbBind[":f_paymentMode"] = 0;
        fDbBind[":f_creditCard"] = 0;
        fDbBind[":f_cityLedger"] = cityCode;
        fDbBind[":f_paymentComment"] = CacheCityLedger::instance()->get(cityCode)->fName;
        fDbBind[":f_dc"] = "DEBIT";
        fDbBind[":f_sign"] = 1;
        fDbBind[":f_doc"] = "";
        fDbBind[":f_rec"] = "";
        fDbBind[":f_inv"] = ui->leInvoice->text();
        fDbBind[":f_finance"] = 0;
        fDbBind[":f_remarks"] = "";
        fDbBind[":f_canceled"] = 0;
        fDbBind[":f_cancelReason"] = "";
        fDbBind[":f_side"] = 0;
        fDb.update("m_register", fDbBind, where_id(ap(cid)));
    }

    // CHECK FOR DAY USE AND REGISTER GUEST COUNT FOR HISTORY
    if (ui->deCheckin->date() == ui->deDeparture->date()) {
        fDbBind[":f_date"] = WORKING_DATE;
        fDbBind[":f_reservation"] = ui->leReserveID->text();
        fDb.select("insert into f_reservation_guests_history (f_date, f_reservation, f_guest) "
                   "select :f_date, :f_reservation, f_guest from f_reservation_guests "
                   "where f_reservation=:f_reservation", fDbBind, fDbRows);
    }
    if (result) {
        //TRACK
        /////////////////////////////// check checkout date == departure date
        if (ui->deDeparture->date() > WORKING_DATE) {
            fTrackControl->insert("Departure date changed", ui->deDeparture->date().toString(def_date_format),
                                      WORKING_DATE.toString(def_date_format));
            fTrackControl->insert("Checkout", "", "");
            fDbBind[":f_endDate"] = WORKING_DATE;
            result = result && fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserveID->text())));
        }
    }
    // FINAL AMOUNT
    if (result) {
        DatabaseResult dr;
        fDbBind[":f_inv"] = ui->leInvoice->text();
        QVariant finalAmount = dr.singleResult("select sum(f_amountAmd) from m_register where f_inv=:f_inv "
                                               "and f_finance=1 and f_canceled=0 and f_sign=1", fDbBind);
        fDbBind[":f_grandTotal"] = finalAmount;
        fDbBind[":f_id"] = ui->leReserveID->text();
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserveID->text())));
        fDbBind[":f_grandTotal"] = finalAmount;
        fDbBind[":f_id"] = ui->leReserveID->text();
        fDb.update("m_register", fDbBind, where_id(ap(ui->leReserveID->text())));
    }
    if (result) {
        fTrackControl->insert("Checkout", "", "");
        fTrackControl->saveChanges();
        fDb.fDb.commit();

        if (ui->tblInvLeft->rowCount() > 0) {
            PPrintInvoice(ui->leInvoice->text(), 0, this);
        }
        if (ui->tblInvRight->rowCount() > 0) {
            PPrintInvoice(ui->leInvoice->text(), 1, this);
        }
        BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReserveID->text());
        BroadcastThread::cmdRefreshCache(cid_room, ui->leRoomCode->text());
        BroadcastThread::cmdRefreshCache(cid_active_room, ui->leRoomCode->text());
        CI_Reservation *r = CacheReservation::instance()->get(ui->leReserveID->text());
        CI_Reservation *n = 0;
        if (CacheReservation::instance()->hasNext(r, n)) {
            BroadcastThread::cmdRefreshCache(cid_reservation, n->fId);
            BroadcastThread::cmdRefreshCache(cid_red_reservation, n->fId);
        }
        clearInvoice();
        fTabWidget->removeTab(fTabIndex);
    } else {
        fDb.fDb.rollback();
    }
}

void WInvoice::on_btnPaymentsDetails_clicked()
{
    DlgPaymentsDetails *d = new DlgPaymentsDetails(this);
    d->setInvoice(ui->leInvoice->text());
    d->exec();
    loadInvoice(ui->leInvoice->text());
    delete d;
}

void WInvoice::on_btnTaxPrint_clicked()
{
    EQTableWidget *t = 0;
    int result = DlgPrintTaxSideOption::printTaxSide();
    switch (result) {
    case pts_none:
        return;
    case pts_guest:
        t = ui->tblInvLeft;
        break;
    case pts_company:
        t = ui->tblInvRight;
        break;
    }

    if (fPreferences.getDb(def_tax_port).toInt() == 0) {
        message_error_tr("Setup tax printer first");
    }

    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnCancel_clicked()
{
    QModelIndexList t1 = ui->tblInvLeft->selectionModel()->selectedRows();
    QModelIndexList t2 = ui->tblInvRight->selectionModel()->selectedRows();
    if (t1.count() + t2.count() == 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    DlgInvoiceCancelation *d = new DlgInvoiceCancelation(this);
    d->setTrackWindowId(ui->leInvoice->text());
    bool noall = false;
    bool haveEntries = false;
    for (int i = 0; i < t1.count(); i++) {
        QList<QVariant> row;
        row << ui->tblInvLeft->item(t1.at(i).row(), 0)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 8)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 2)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 3)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 4)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 5)->data(Qt::DisplayRole);
        if (ui->tblInvLeft->item(t1.at(i).row(), 2)->data(Qt::EditRole).toDate() < WORKING_DATE) {
            if (!r__(cr__invoice_cancelation_previouse_date)) {
                noall = true;
                continue;
            }
        }
        haveEntries = true;
        d->addRow(row);
    }
    for (int i = 0; i < t2.count(); i++) {
        QList<QVariant> row;
        row << ui->tblInvRight->item(t2.at(i).row(), 0)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 8)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 2)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 3)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 4)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 5)->data(Qt::DisplayRole);
        if (ui->tblInvRight->item(t2.at(i).row(), 11)->data(Qt::DisplayRole).toInt() == fPreferences.getDb(def_room_charge_id).toInt()) {
            if (ui->tblInvRight->item(t2.at(i).row(), 2)->data(Qt::EditRole).toDate() < WORKING_DATE) {
                if (!r__(cr__invoice_cancelation_previouse_date)) {
                    noall = true;
                    continue;
                }
            }
        }
        haveEntries = true;
        d->addRow(row);
    }
    if (noall) {
        message_info_tr("Some ent╚ries was excluded from selection, because insufficiently of privileges");
    }
    if (haveEntries) {
        if (d->exec() == QDialog::Accepted) {
            loadInvoice(ui->leInvoice->text());
        }
    }
    delete d;
}

void WInvoice::clearInvoice()
{
    ui->leRoomCode->clear();
    ui->leRoom->clear();
    ui->teRemark->clear();
    ui->leInvoice->clear();
    ui->leReserveID->clear();
    ui->leGuest->clear();
    ui->lePassport->clear();
    ui->deCheckin->setDate(QDate::currentDate());
    ui->deDeparture->setDate(QDate::currentDate());
    ui->leCardex->clear();
    ui->leCardexName->clear();
    ui->leNights->setText("0");
    ui->leRoomRate->setText("0");
    ui->tblInvLeft->clearContents();
    ui->tblInvRight->clearContents();
    ui->tblInvLeft->setRowCount(0);
    ui->tblInvRight->setRowCount(0);
    countTotal(ui->tblInvLeft);
    countTotal(ui->tblInvRight);
    setupTabTextAndIcon(tr("Invoice"), ":/images/invoice.png");
}

void WInvoice::on_btnTransfer_clicked()
{
    QModelIndexList t1 = ui->tblInvLeft->selectionModel()->selectedRows();
    QModelIndexList t2 = ui->tblInvRight->selectionModel()->selectedRows();
    if (t1.count() + t2.count() == 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    DlgTransfer *d = new DlgTransfer(this);
    for (int i = 0; i < t1.count(); i++) {
        if (ui->tblInvLeft->item(t1.at(i).row(), 1)->data(Qt::DisplayRole).toInt() < 0) {
            continue;
        }
        QList<QVariant> row;
        row << ui->tblInvLeft->item(t1.at(i).row(), 0)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 2)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 4)->data(Qt::DisplayRole);
        row << ui->tblInvLeft->item(t1.at(i).row(), 5)->data(Qt::DisplayRole);
        d->addRow(row);
    }
    for (int i = 0; i < t2.count(); i++) {
        if (ui->tblInvRight->item(t2.at(i).row(), 1)->data(Qt::DisplayRole).toInt() < 0) {
            continue;
        }
        QList<QVariant> row;
        row << ui->tblInvRight->item(t2.at(i).row(), 0)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 2)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 4)->data(Qt::DisplayRole);
        row << ui->tblInvRight->item(t2.at(i).row(), 5)->data(Qt::DisplayRole);
        d->addRow(row);
    }
    d->setRoomByCode(ui->leRoomCode->text());
    if (d->exec() == QDialog::Accepted) {
        loadInvoice(ui->leInvoice->text());
    }
    delete d;
}

void WInvoice::on_btnTrack_clicked()
{
    DlgTracking::showTracking(TRACK_RESERVATION, ui->leInvoice->text());
}

void WInvoice::save()
{
    fDb.fDb.transaction();
    fDbBind[":f_remarks"] = ui->teRemark->toPlainText();
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserveID->text())));
    fTrackControl->saveChanges();
    fDb.fDb.commit();
    ui->teRemark->setReadOnly(true);
}

void WInvoice::on_btnDiscount_clicked()
{
    DlgDiscount *d = new DlgDiscount(this);
    double guestAmount = countTotal(ui->tblInvLeft);
    double companyAmount = countTotal(ui->tblInvRight);
    d->setParams(ui->leRoomCode->text(), guestAmount, companyAmount);
    if (d->exec() == QDialog::Accepted) {
        loadInvoice(ui->leInvoice->text());
    }
    delete d;
}

void WInvoice::on_btnTransferAmount_clicked()
{
    DlgTransferInvoiceAmount *d = new DlgTransferInvoiceAmount(this);
    d->setRoomFrom(CacheActiveRoom::instance()->get(ui->leRoomCode->text()));
    d->exec();
    delete d;
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnPrintInvoice_clicked()
{
    switch (DlgInvoicePrintOption::getOption()) {
    case pio_none:
        break;
    case pio_guest:
        PPrintInvoice(ui->leInvoice->text(), 0, this);
        break;
    case pio_comp:
        PPrintInvoice(ui->leInvoice->text(), 1, this);
        break;
    case pio_guestcomp_ser:
        PPrintInvoice(ui->leInvoice->text(), 0, this);
        PPrintInvoice(ui->leInvoice->text(), 1, this);
        break;
    case pio_guestcomp_tog:
        PPrintInvoice(ui->leInvoice->text(), -1, this);
        break;
    }
}

void WInvoice::on_btnModifyRoom_clicked()
{
    WReservation::openReserveWindows(ui->leReserveID->text());
}

void WInvoice::on_tblInvLeft_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    openVaucher(ui->tblInvLeft->toString(index.row(), 8), ui->tblInvLeft->toString(index.row(), 0));
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_tblInvRight_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    openVaucher(ui->tblInvRight->toString(index.row(), 8), ui->tblInvRight->toString(index.row(), 0));
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnPrintAdvance_clicked()
{
    if (fPreferences.getDb(def_tax_port).toInt() == 0) {
        message_error_tr("Setup tax printer first");
    }
    QList<QList<QVariant> > data;
    for (int i = 0; i < ui->tblInvLeft->rowCount(); i++) {
        if (ui->tblInvLeft->toInt(i, 1) != -1) {
            continue;
        }
        if (ui->tblInvLeft->itemChecked(i, 6)) {
            continue;
        }
        if (ui->tblInvLeft->item(i, 8)->text() != VAUCHER_ADVANCE_N) {
            continue;
        }
        QList<QVariant> row;
        row << ui->tblInvLeft->toInt(i, 0)
            << ui->tblInvLeft->toString(i, 3)
            << ui->tblInvLeft->toString(i, 4)
            << ""
            << ui->tblInvLeft->toString(i, 13);
        data << row;
    }
    for (int i = 0; i < ui->tblInvRight->rowCount(); i++) {
        if (ui->tblInvRight->toInt(i, 1) != -1) {
            continue;
        }
        if (ui->tblInvRight->itemChecked(i, 6)) {
            continue;
        }
        if (ui->tblInvRight->item(i, 8)->text() != VAUCHER_ADVANCE_N) {
            continue;
        }
        QList<QVariant> row;
        row << ui->tblInvRight->toInt(i, 0)
            << ui->tblInvRight->toString(i, 3)
            << ui->tblInvRight->toString(i, 4)
            << ""
            << ui->tblInvRight->toString(i, 13);
        data << row;
    }
    if (data.count() == 0) {
        message_info_tr("Nothing to print");
        return;
    }
    DlgPreTax *d = new DlgPreTax(this);
    d->setInvoice(ui->leInvoice->text());
    foreach (QList<QVariant> v, data) {
        d->addRow(v);
    }
    d->exec();
    delete d;
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnPrevInvoice_clicked()
{
    int i = ui->leRoomCode->asInt();
    if (i == 0) {
        i = 1000;
    }
    if (i > 0) {
        while (i > 1) {
            i--;
            CI_ActiveRoom *ar = CacheActiveRoom::instance()->get(i);
            if (ar && ar->fValid) {
                ui->leRoomCode->setInt(i);
                loadInvoice(ar->fInvoice);
                return;
            }
        }
        message_info_tr("The start is reached");
    }
}

void WInvoice::on_btnNextInvoice_clicked()
{
    int i = ui->leRoomCode->asInt();
    if (i == 0) {
        i = 1;
    }
    if (i > 0) {
        while (i < 700) {
            i++;
            CI_ActiveRoom *ar = CacheActiveRoom::instance()->get(i);
            if (ar && ar->fValid) {
                ui->leRoomCode->setInt(i);
                loadInvoice(ar->fInvoice);
                return;
            }
        }
        message_info_tr("The end is reached");
    }
}

void WInvoice::on_btnTaxBack_clicked()
{
    QList<QList<QVariant> > rows;
    for (int i = 0; i < ui->tblInvLeft->rowCount(); i++) {
        if (ui->tblInvLeft->item(i, 6)->checkState() == Qt::Checked && ui->tblInvLeft->toInt(i, 6) > 0) {
            QList<QVariant> row;
            row << ui->tblInvLeft->toString(i, 0)
                << ui->tblInvLeft->toString(i, 3)
                << ui->tblInvLeft->toString(i, 4)
                << ui->tblInvLeft->toString(i, 8)
                << ui->tblInvLeft->toInt(i, 6);
            rows << row;
        }
    }
    for (int i = 0; i < ui->tblInvRight->rowCount(); i++) {
        if (ui->tblInvRight->item(i, 6)->checkState() == Qt::Checked && ui->tblInvRight->toInt(i, 6) > 0) {
            QList<QVariant> row;
            row << ui->tblInvRight->toString(i, 0)
                << ui->tblInvRight->toString(i, 3)
                << ui->tblInvRight->toString(i, 4)
                << ui->tblInvRight->toString(i, 8)
                << ui->tblInvRight->toInt(i, 6);
            rows << row;
        }
    }
    DlgTaxBack::taxBack(ui->leInvoice->text(), rows);
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnAdvance_clicked()
{
    DlgAdvance *d = new DlgAdvance(ui->leReserveID->text(), this);
    d->exec();
    delete d;
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnManualTax_clicked()
{
    DlgPrintRandomTax *d = new DlgPrintRandomTax(this);
    d->setHeader(ui->leRoomCode->text(), ui->leGuest->text(), ui->leInvoice->text());
    d->exec();
    delete d;
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDb.select("select f_id, f_source, f_item, f_amount, f_taxCode from m_free_tax where f_invoice=:f_invoice and f_used=0", fDbBind, fDbRows);
    for (int i = 0; i < ui->tblInvLeft->rowCount(); i++) {
        if (ui->tblInvLeft->item(i, 6)->checkState() == Qt::Checked) {
            continue;
        }
        if (ui->tblInvLeft->toString(i, 8) == "RM" || ui->tblInvLeft->toString(i, 8) == "CH") {
            QMutableListIterator<QList<QVariant> >it(fDbRows);
            while (it.hasNext()) {
                it.next();
                if (ui->tblInvLeft->toString(i, 8) == it.value().at(1).toString()) {
                    if (ui->tblInvLeft->toString(i, 8) == "RM") {
                        if (ui->tblInvLeft->toDouble(i, 4) == it.value().at(3).toDouble()) {
                            fDbBind[":f_used"] = 1;
                            fDb.update("m_free_tax", fDbBind, where_id(it.value().at(0).toInt()));
                            fDbBind[":f_fiscal"] = it.value().at(4);
                            fDbBind[":f_usedTaxId"] = it.value().at(0);
                            fDb.update("m_register", fDbBind, where_id(ap(ui->tblInvLeft->toString(i, 0))));
                            it.remove();
                        }
                    } else {
                        if (ui->tblInvLeft->toInt(i, 11) == it.value().at(2).toInt() && ui->tblInvLeft->toDouble(i, 4) == it.value().at(3).toDouble()) {
                            fDbBind[":f_used"] = 1;
                            fDb.update("m_free_tax", fDbBind, where_id(it.value().at(0).toInt()));
                            fDbBind[":f_fiscal"] = it.value().at(4);
                            fDbBind[":f_usedTaxId"] = it.value().at(0);
                            fDb.update("m_register", fDbBind, where_id(ap(ui->tblInvLeft->toString(i, 0))));
                            it.remove();
                        }
                    }
                    goto next;
                }
            }
        }
        next:
        continue;
    }
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnRefTax_clicked()
{
    /*
    EQTableWidget *t = ui->tblInvLeft;
    QModelIndexList l = ui->tblInvLeft->selectionModel()->selectedIndexes();
    if (l.count() == 0) {
        t = ui->tblInvRight;
        l = ui->tblInvRight->selectionModel()->selectedIndexes();
    }
    if (l.count() == 0) {
        message_info_tr("Nothing is selected");
        return;
    }
    */
    DlgTaxReference *d = new DlgTaxReference(0, this);
    d->setHeader(ui->leRoomCode->text(), ui->leGuest->text(), ui->leInvoice->text());
    //d->setItem(t->toString(l.at(0).row(), 3), t->toString(l.at(0).row(), 4), t->toInt(l.at(0).row(), 0));
    d->exec();
    delete d;
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnTaxBack_2_clicked()
{
    QList<QVariant> rows;
    for (int i = 0; i < ui->tblInvLeft->rowCount(); i++) {
        if (ui->tblInvLeft->item(i, 6)->checkState() == Qt::Checked && ui->tblInvLeft->toInt(i, 6) > 1) {
            QList<QVariant> row;
            row << ui->tblInvLeft->toInt(i, 6);
            rows << row;
        }
    }
    for (int i = 0; i < ui->tblInvRight->rowCount(); i++) {
        if (ui->tblInvRight->item(i, 6)->checkState() == Qt::Checked && ui->tblInvRight->toInt(i, 6) > 1) {
            QList<QVariant> row;
            row << ui->tblInvLeft->toInt(i, 6);
            rows << row;
        }
    }
    DlgTaxBack2::taxBack(ui->leInvoice->text(), rows);
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnPostBreakfast_clicked()
{
    DlgPostBreakfast::postBreakfast(ui->leInvoice->text(), DlgPostBreakfast::ptBreakfast);
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnTransferAmountCL_clicked()
{
    DlgCityAdvance::cityAdvance("", ui->leInvoice->text(), 1);
    loadInvoice(ui->leInvoice->text());
}

void WInvoice::on_btnPostMinibar_clicked()
{
    DlgPostBreakfast::postBreakfast(ui->leInvoice->text(), DlgPostBreakfast::ptMinibar);
    loadInvoice(ui->leInvoice->text());
}
