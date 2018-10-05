#include "waccinvoice.h"
#include "ui_waccinvoice.h"
#include "dlgtracking.h"
#include "wreportgrid.h"
#include "pprintinvoice.h"
#include "dlginvoiceprintoption.h"
#include "cacheinvoiceitem.h"
#include "vauchers.h"
#include "cachevatmode.h"
#include "dlgprinttaxsideoption.h"
#include "dlgsearchinvoice.h"
#include "printtax.h"
#include "cachetaxmap.h"
#include "dlgtaxback.h"
#include "dlgviewinvoicecorrections.h"
#include "databaseresult.h"
#include "pexportinvoicetoexcel.h"
#include "dlgchartdaterange.h"
#include "dlgreservationguests.h"
#include "dlgpostingcharges.h"
#include "dlgcl.h"
#include "wreservation.h"
#include "dlgremotinvoices.h"
#include "dlgreserveshortinfo.h"
#include "wvauchereditor.h"
#include <QSqlRecord>
#include <QInputDialog>

#define sel_invoice 1

WAccInvoice::WAccInvoice(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WAccInvoice)
{
    ui->setupUi(this);
    ui->leReservationId->setVisible(false);
    ui->deCheckIn->setReadOnly(!r__(cr__super_correction));
    ui->deDeparture->setReadOnly(!r__(cr__super_correction));
    ui->btnEliminate->setVisible(r__(cr__super_correction));
    ui->btnPostingCharges->setVisible(r__(cr__super_correction) && false);
    ui->btnMoveItem->setVisible(r__(cr__super_correction));
    ui->btnNewVaucher->setVisible(r__(cr__super_correction));
    ui->btnEditReserv->setVisible(r__(cr__super_correction));
    ui->btnEditRowVaucher->setVisible(r__(cr__super_correction));
    ui->btnExcel->setVisible(r__(cr__invoice_export_to_excel));
    fCurrentInvoicePosition = -1;
    fReport = dynamic_cast<WReportGrid*>(parent);
    if (fReport) {
        connect(fReport, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(handleValues(QList<QVariant>)));
    }
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 30, 90, 0, 250, 80, 80, 80, 30, 30, 0, 0);
    Utils::tableSetColumnWidths(ui->tblTotal, ui->tblTotal->columnCount(),
                                0, 30, 90, 0, 250, 80, 80, 80, 30, 30, 0, 0);
    fDockInvoice = new DWSelectorCheckoutInvoice(this);
    fDockInvoice->configure();
    fDockInvoice->setSelector(ui->leInvoice);
    fDockInvoice->setDialog(this, sel_invoice);
    fTrackControl = new TrackControl(TRACK_RESERVATION);
    fTrackControl->addWidget(ui->deCheckIn, "Check in date");
    fTrackControl->addWidget(ui->deDeparture, "Departure date");

    if (r__(cr__super_correction)) {
        fDockCardex = new DWSelectorCardex(this);
        fDockCardex->configure();
        fDockCardex->setSelector(ui->leCardex);
        connect(fDockCardex, SIGNAL(cardex(CI_Cardex*)), this, SLOT(cardex(CI_Cardex*)));

        fDockVat = new DWSelectorVATMode(this);
        fDockVat->configure();
        fDockVat->setSelector(ui->leVATMode);
        connect(fDockVat, SIGNAL(vatMode(CI_VATMode*)), this, SLOT(vatMode(CI_VATMode *)));
    }
}

WAccInvoice::~WAccInvoice()
{
    delete ui;
}

void WAccInvoice::setupTab()
{
    setupTabTextAndIcon(tr("Invoice"), ":/images/invoice.png");
}

void WAccInvoice::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case sel_invoice:
        CI_CheckoutInvoice *ci = value.value<CI_CheckoutInvoice*>();
        if (ci) {
            ui->leInvoice->setText(ci->fCode);
            on_leInvoice_returnPressed();
            fCurrentInvoicePosition = fDockInvoice->fCurrentRow;
        }
        break;
    }
}

bool WAccInvoice::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        if (ui->leInvoice->hasFocus()) {
            load(ui->leInvoice->text());
            return QWidget::event(event);
        }
    }
    return BaseWidget::event(event);
}

QString WAccInvoice::invoiceId()
{
    return ui->leInvoice->text();
}

void WAccInvoice::load(const QString &id)
{
    disconnect(ui->deEntry, SIGNAL(userDateChanged(QDate)), this, SLOT(on_deEntry_userDateChanged(QDate)));
    disconnect(ui->deDeparture, SIGNAL(userDateChanged(QDate)), this, SLOT(on_deDeparture_userDateChanged(QDate)));
    setupTabTextAndIcon(tr("Invoice") + " #?", ":/images/invoice.png");
    ui->tblData->setRowCount(0);
    ui->leInvoice->setText(id);
    fDbBind[":f_id"] = id;
    QString query = "select r.f_startDate, r.f_endDate, r.f_checkInDate, r.f_checkInTime, "
            "r.f_checkOutTime, rc.f_short, r.f_room, ra.f_" + def_lang + ", "
            "concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName), n.f_name, "
            "concat(u1.f_firstName, ' ', u1.f_lastName), concat(u2.f_firstName, ' ', u2.f_lastName), "
            "concat(r.f_cardex, '/', c.f_name), "
            "r.f_man + r.f_woman + r.f_child, r.f_id, r.f_vatMode, r.f_remarks, r.f_state "
            "from f_reservation r "
            "left join f_room rm on rm.f_id=r.f_room "
            "left join f_room_classes rc on rc.f_id=rm.f_class "
            "left join f_room_arrangement ra on ra.f_id=r.f_arrangement "
            "left join f_guests g on g.f_id=r.f_guest "
            "left join f_nationality n on n.f_short=g.f_nation "
            "left join users u1 on u1.f_id=r.f_checkInUser "
            "left join users u2 on u2.f_id=r.f_checkOutUser "
            "left join f_cardex c on r.f_cardex=c.f_cardex "
            "where r.f_invoice=:f_id";
    fDb.select(query, fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Incorrect invoice number, or end reached");
        return;
    }
    /* NEXT PREV
    if (fDbRows.at(0).at(17).toInt() != RESERVE_CHECKOUT) {
        if (next == 0) {
            message_error_tr("This invoice available only from room chart");
            return;
        }
        id += next;
        load(id);
        return;
    }
    */
    int c = 0;
    ui->deEntry->setDate(fDbRows.at(0).at(c++).toDate());
    ui->deDeparture->setDate(fDbRows.at(0).at(c++).toDate());
    ui->deCheckIn->setDate(fDbRows.at(0).at(c++).toDate());
    ui->teCheckInTime->setTime(fDbRows.at(0).at(c++).toTime());
    ui->teCheckOutTime->setTime(fDbRows.at(0).at(c++).toTime());
    ui->leCategory->setText(fDbRows.at(0).at(c++).toString());
    ui->leRoom->setText(fDbRows.at(0).at(c++).toString());
    ui->leArrangment->setText(fDbRows.at(0).at(c++).toString());
    ui->leGuest->setText(fDbRows.at(0).at(c++).toString());
    ui->leNationality->setText(fDbRows.at(0).at(c++).toString());
    ui->leOperatorIn->setText(fDbRows.at(0).at(c++).toString());
    ui->leOperatorOut->setText(fDbRows.at(0).at(c++).toString());
    ui->leCardex->setText(fDbRows.at(0).at(c++).toString());
    ui->leGuestCount->setText(fDbRows.at(0).at(c++).toString());
    ui->leReservationId->setText(fDbRows.at(0).at(c++).toString());
    ui->leVATMode->fHiddenText = fDbRows.at(0).at(c++).toString();
    ui->teRemarks->setPlainText(fDbRows.at(0).at(c++).toString());

    CI_VATMode *v = CacheVatMode::instance()->get(ui->leVATMode->fHiddenText);
    if (v) {
        ui->leVATMode->setText(v->fName);
    }

    query = "select m.f_id, m.f_sign, m.f_wdate, m.f_itemcode, m.f_finalName, "
            "m.f_amountAmd, m.f_amountVat, m.f_fiscal, if(m.f_side=0,'G', 'C'), m.f_source, m.f_doc "
            "from m_register m "
            "where m.f_inv=" + ap(ui->leInvoice->text()) + " " +
            "and m.f_canceled=0 and m.f_finance in (:fin) "
            "order by m.f_wdate, m.f_time ";
    if (ui->chViewAllVauchers->isChecked()) {
        query.replace(":fin", "0,1");
    } else {
        query.replace(":fin", "1");
    }
    fDb.select(query, fDbBind, fDbRows);
    viewEntries();
    setupTabTextAndIcon(tr("Invoice") + " S/N " + id, ":/images/invoice.png");
    fTrackControl->fInvoice = ui->leInvoice->text();
    fTrackControl->fReservation = ui->leReservationId->text();
    fTrackControl->resetChanges();
    connect(ui->deEntry, SIGNAL(userDateChanged(QDate)), this, SLOT(on_deEntry_userDateChanged(QDate)));
    connect(ui->deDeparture, SIGNAL(userDateChanged(QDate)), this, SLOT(on_deDeparture_userDateChanged(QDate)));
}

void WAccInvoice::viewEntries()
{
    QFont f(qApp->font());
    f.setBold(true);
    QList<QList<QVariant> > proxyData;
    foreach_rows {
        if (!ui->chPaymentByGuest->isChecked()) {
            if (it->at(8).toString() == "G") {
                continue;
            }
        }
        if (!ui->chPaymentByCompany->isChecked()) {
            if (it->at(8).toString() == "C") {
                continue;
            }
        }
        if (ui->rbEntryName->isChecked()) {
            bool found = false;
            for (QList<QList<QVariant> >::iterator row = proxyData.begin(); row != proxyData.end(); row++) {
                if (row->at(3).toInt() == it->at(3).toInt() && row->at(1).toInt() == it->at(1).toInt()) {
                    if (row->at(1).toInt() == 1) {
                        (*row)[5] = row->at(5).toDouble() + it->at(5).toDouble();
                    } else if (row->at(1).toInt() == -1) {
                        (*row)[5] = row->at(5).toDouble() + it->at(5).toDouble();
                    }
                    (*row)[6] = row->at(6).toDouble() + it->at(6).toDouble();
                    found = true;
                }
            }
            if (!found) {
                proxyData.append(*it);
                proxyData[proxyData.count() - 1][2] = ui->deDeparture->date();
            }
        } else {
            proxyData.append(*it);
        }
    }
    ui->tblData->setRowCount(proxyData.count());
    int row = 0;
    foreach (QList<QVariant> rowData, proxyData) {
        //rec id - 0
        ui->tblData->setItem(row, 0, Utils::tableItem(rowData.at(0)));
        //selection - 1
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, rowData.at(1));
        item->setCheckState(Qt::Checked);
        ui->tblData->setItem(row, 1, item);
        //date - 2
        ui->tblData->setItem(row, 2, Utils::tableItem(rowData.at(2), rowData.at(2)));
        //item code - 3
        ui->tblData->setItem(row, 3, Utils::tableItem(rowData.at(3)));
        //item - 4
        ui->tblData->setItem(row, 4, Utils::tableItem(rowData.at(4)));
        //debit, depended on second field - 5
        if (rowData.at(1).toInt() == 1) {
            ui->tblData->setItem(row, 5, Utils::tableItem(rowData.at(5)));
        } else {
            ui->tblData->setItem(row, 5, Utils::tableItem(0));
        }
        //credit, depended on second field - 6
        if (rowData.at(1).toInt() == -1) {
            ui->tblData->setItem(row, 6, Utils::tableItem(rowData.at(5)));
        } else {
            ui->tblData->setItem(row, 6, Utils::tableItem(0));
        }
        //vat - 7
        ui->tblData->setItem(row, 7, Utils::tableItem(rowData.at(6)));
        //tax - 8
        QTableWidgetItem *itemTax = new QTableWidgetItem();
        itemTax->setFlags(itemTax->flags() ^ Qt::ItemIsUserCheckable);
        itemTax->setCheckState(rowData.at(7).toInt() == 0  ? Qt::Unchecked : Qt::Checked);
        ui->tblData->setItem(row, 8, itemTax);
        //side - 9
        ui->tblData->setItem(row, 9, Utils::tableItem(rowData.at(8).toString()));
        // source - 10
        ui->tblData->setItem(row, 10, Utils::tableItem(rowData.at(9).toString()));
        // source id - 11
        ui->tblData->setItem(row, 11, Utils::tableItem(rowData.at(10).toString()));
        if (rowData.at(1).toInt() < 0) {
            for (int i = 0; i < ui->tblData->columnCount(); i++) {
                ui->tblData->item(row, i)->setFont(f);
            }
        }
        row++;
    }
    countAmounts();
}

void WAccInvoice::countAmounts()
{
    double debet = 0, credit = 0, vat = 0;
    for (int i = 0, count = ui->tblData->rowCount(); i < count; i++) {
        debet += ui->tblData->item(i, 5)->text().toDouble();
        credit += ui->tblData->item(i, 6)->text().toDouble();
        vat += ui->tblData->item(i, 7)->text().toDouble();
    }
    ui->tblTotal->setItem(0, 5, Utils::tableItem(debet));
    ui->tblTotal->setItem(0, 6, Utils::tableItem(credit));
    ui->tblTotal->setItem(0, 7, Utils::tableItem(vat));
    ui->tblTotal->setItem(1, 4, Utils::tableItem(tr("Balance")));
    ui->tblTotal->setItem(1, 5, Utils::tableItem(credit - debet));
}

int WAccInvoice::countNights()
{
    int result = 0;
    foreach_rows {
        if (it->at(1).toInt() != 1) {
            continue;
        }
        if (it->at(3).toInt() == fPreferences.getDb(def_room_charge_id).toInt()) {
            result++;
        }
    }
    return result;
}

void WAccInvoice::cardex(CI_Cardex *c)
{
    if (c) {
        if (message_confirm_tr("Change the cardex?")) {
            QString oldCardex = ui->leCardex->text();
            ui->leCardex->setText(c->fCode + "/" + c->fName);
            fDbBind[":f_cardex"] = c->fCode;
            fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservationId->text())));
            fTrackControl->insert(TRACK_RESERVATION, "Cardex", oldCardex, ui->leCardex->text());
            //fTrackControl->saveChanges(ui->leReservationId->text());
        }
    }
}

void WAccInvoice::vatMode(CI_VATMode *c)
{
    if (c) {
        if (message_confirm_tr("Change the VAT mode?")) {
            QString oldMode = ui->leVATMode->text();
            ui->leVATMode->setText(c->fName);
            fDbBind[":f_vatMode"] = c->fCode.toInt();
            fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservationId->text())));
            fTrackControl->insert("VAT mode",  oldMode, ui->leVATMode->text());
        }
    }
}

void WAccInvoice::handleValues(const QList<QVariant> &values)
{
    if (values.count() == 0) {
        return;
    }
    WAccInvoice *ai = 0;
    for (int i = 0, count = fMainWindow->fTab->count(); i < count; i++) {
        ai = dynamic_cast<WAccInvoice*>(fMainWindow->fTab->widget(i));
        if (ai) {
            if (ai->invoiceId() == values.at(0).toString()) {
                fMainWindow->fTab->setCurrentWidget(ai);
                return;
            }
        }
    }
    ai = addTab<WAccInvoice>();
    ai->load(values.at(0).toString());
}

void WAccInvoice::on_btnNext_clicked()
{
    CI_CheckoutInvoice *ci = fDockInvoice->nextRow(fCurrentInvoicePosition);
    if (ci) {
        load(ci->fCode);
    }
}

void WAccInvoice::on_btnPreviouse_clicked()
{
    CI_CheckoutInvoice *ci = fDockInvoice->prevRow(fCurrentInvoicePosition);
    if (ci) {
        load(ci->fCode);
    }
}

void WAccInvoice::on_btnTracking_clicked()
{
    DlgTracking::showTracking(TRACK_RESERVATION, ui->leInvoice->text());
}

void WAccInvoice::on_rbEntryName_clicked()
{
    viewEntries();
}

void WAccInvoice::on_rbNoGroup_clicked()
{
    viewEntries();
}

void WAccInvoice::on_chPaymentByGuest_clicked()
{
    viewEntries();
}

void WAccInvoice::on_chPaymentByCompany_clicked()
{
    viewEntries();
}

void WAccInvoice::on_btnPrint_clicked()
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

void WAccInvoice::on_btnTaxPrint_clicked()
{
    QString side;
    int result = DlgPrintTaxSideOption::printTaxSide();
    switch (result) {
    case pts_none:
        return;
    case pts_guest:
        side = "G";
        break;
    case pts_company:
        side = "C";
        break;
    }

    if (fPreferences.getDb(def_tax_port).toInt() == 0) {
        message_error_tr("Setup tax printer first");
    }
    PrintTax *pt =  new PrintTax(this);
    pt->fInvoice = ui->leInvoice->text();
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (!isTaxPay(ui->tblData->toString(i, 10))) {
            continue;
        }

        if (ui->tblData->itemChecked(i, 8)) {
            continue;
        }
        if (ui->tblData->toString(i, 9) != side) {
            continue;
        }
        //HOTEL SOURCE
        if (ui->tblData->toString(i, 10) != VAUCHER_POINT_SALE_N ) {
            CI_InvoiceItem *c = CacheInvoiceItem::instance()->get(ui->tblData->toString(i, 3));
            if (!c) {
                message_error_tr("Error in tax print. c == 0, case 1.");
                continue;
            }
            CI_TaxMap *ci = CacheTaxMap::instance()->get(c->fCode);
            if (!ci) {
                message_error(tr("Tax department undefined for ") + c->fName);
                return;
            }
            QString qty = "1";
            QString price = QString::number(ui->tblData->toDouble(i, 5), 'f', 2);
            if (c->fCode == fPreferences.getDb(def_auto_breakfast_id).toString()) {
                DatabaseResult drb;
                fDbBind[":f_id"] = ui->tblData->toString(i, 0);
                drb.select(fDb, "select * from o_breakfast where f_id=:f_id", fDbBind);
                if (drb.rowCount() == 0) {
                    message_error_tr("Cannot find breakfast record. Contact to application developer.");
                    continue;
                }
                qty = drb.value("f_pax").toString();
                price = drb.value("f_price").toString();
            }
            pt->fDept.append(ui->leVATMode->fHiddenText.toInt() == VAT_INCLUDED ? ci->fName : c->fNoVatDept);
            pt->fRecList.append(ui->tblData->toString(i, 0));
            pt->fCodeList.append(c->fCode);
            pt->fNameList.append(c->fTaxName);
            pt->fPriceList.append(price);
            pt->fQtyList.append(qty);
            pt->fAdgCode.append(c->fAdgt);
            pt->fTaxNameList.append(c->fTaxName);
        } else if (ui->tblData->toString(i, 10) == VAUCHER_POINT_SALE_N ) {
            //RESTAURANT SOURCE
            QString orderId = ui->tblData->toString(i, 11);
            CI_InvoiceItem *c = CacheInvoiceItem::instance()->get(ui->tblData->toString(i, 3));
            if (!c) {
                message_error_tr("Error in tax print. c == 0, case 2.");
                continue;
            }
            CI_TaxMap *ci = CacheTaxMap::instance()->get(c->fCode);
            if (!ci) {
                message_error(tr("Tax department undefined for ") + c->fName);
                return;
            }
            fDbBind[":f_header"] = orderId;
            fDbBind[":f_state"] = DISH_STATE_READY;
            fDb.select("select d.f_id, d.f_am, d.f_adgt, od.f_qty, od.f_price "
                       "from o_dish od "
                       "inner join r_dish d on d.f_id=od.f_dish "
                       "where od.f_header=:f_header and od.f_state=:f_state "
                       "and (od.f_complex=0 or (od.f_complex>0 and od.f_complexId>0)) ", fDbBind, fDbRows);
            foreach_rows {
                QString vatReception = c->fVatDept;
                if (!c->fVatReception.isEmpty()) {
                    vatReception = c->fVatReception;
                }
                pt->fDept.append(ui->leVATMode->fHiddenText.toInt() == VAT_INCLUDED ? ci->fName : c->fNoVatDept);
                pt->fRecList.append(ui->tblData->toString(i, 0));
                pt->fCodeList.append(it->at(0).toString());
                pt->fNameList.append(it->at(1).toString());
                pt->fPriceList.append(it->at(4).toString());
                pt->fQtyList.append(it->at(3).toString());
                pt->fAdgCode.append(it->at(2).toString());
                pt->fTaxNameList.append(it->at(1).toString());
            }
        }
    }

    pt->build();
    if (pt->fRecList.count() > 0) {
        pt->exec();
    } else {
        message_info_tr("Nothing to print");
    }
    delete pt;
    load(ui->leInvoice->text());
}

void WAccInvoice::on_leInvoice_returnPressed()
{
    load(ui->leInvoice->text());
}

void WAccInvoice::on_btnTaxBack_clicked()
{
    QList<QList<QVariant> > rows;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->item(i, 8)->checkState() == Qt::Checked) {
            QList<QVariant> row;
            row << ui->tblData->toString(i, 0)
                << ui->tblData->toString(i, 4)
                << (ui->tblData->toInt(i, 9) == 0 ? ui->tblData->toDouble(i, 5) : ui->tblData->toDouble(i, 6))
                << ui->tblData->toString(i, 10)
                << "";
            rows << row;
        }
    }
    DlgTaxBack::taxBack(ui->leInvoice->text(), rows);
    load(ui->leInvoice->text());
}

void WAccInvoice::on_btnViewCorrections_clicked()
{
    DlgViewInvoiceCorrections *d = new DlgViewInvoiceCorrections(ui->leInvoice->text(), this);
    d->exec();
    delete d;
}

void WAccInvoice::on_btnEliminate_clicked()
{
    QModelIndexList rows = ui->tblData->selectionModel()->selectedRows();
    if (rows.count() == 0 && ui->tblData->rowCount() == 0) {
        if (message_confirm_tr("<b>This operation cannot be undon, continue?<b>") != QDialog::Accepted) {
            return;
        }
        fDbBind[":f_inv"] = ui->leInvoice->text();
        fDb.select("delete from m_register where f_inv=:f_inv", fDbBind, fDbRows);
        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDb.select("delete from f_reservation where f_invoice=:f_invoice", fDbBind, fDbRows);
        fDbBind[":f_reservation"] = ui->leReservationId->text();
        fDb.select("delete from f_reservation_guests where f_reservation=:f_reservation", fDbBind, fDbRows);
        fTrackControl->insert("Invoice eliminated", "", "");
        fTrackControl->saveChanges();
        message_info_tr("The invoice was eliminated");
        close();
        fTabWidget->removeTab(fTabIndex);
        return;
    }
    if (rows.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    if (message_confirm("<b>" + tr("This operation cannot be undon, continue?") + "<b>") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] = ui->tblData->item(rows.at(0).row(), 0)->data(Qt::EditRole);
    fDb.select("delete from m_register where f_id=:f_id", fDbBind, fDbRows);
    if (ui->tblData->toString(rows.at(0).row(), 10) == VAUCHER_POINT_SALE_N) {
        fDbBind[":f_id"] = ui->tblData->item(rows.at(0).row(), 0)->data(Qt::EditRole);
        fDb.select("delete from o_header where f_id=:f_id", fDbBind, fDbRows);
        fDbBind[":f_header"] = ui->tblData->item(rows.at(0).row(), 0)->data(Qt::EditRole);
        fDb.select("delete from o_dish where f_header=:f_header", fDbBind, fDbRows);
    } else if (ui->tblData->toString(rows.at(0).row(), 10) == VAUCHER_EVENT_N) {
        fDbBind[":f_id"] = ui->tblData->item(rows.at(0).row(), 0)->data(Qt::EditRole);
        fDb.select("delete from o_event where f_id=:f_id", fDbBind, fDbRows);
    }
    QString oldValue = QString("%1 %2(%3) %4 %5")
            .arg(ui->tblData->item(rows.at(0).row(), 0)->data(Qt::EditRole).toString())
            .arg(ui->tblData->item(rows.at(0).row(), 5)->data(Qt::EditRole).toString())
            .arg(ui->tblData->item(rows.at(0).row(), 6)->data(Qt::EditRole).toString())
            .arg(ui->tblData->item(rows.at(0).row(), 2)->data(Qt::EditRole).toDate().toString(def_date_format))
            .arg(ui->tblData->item(rows.at(0).row(), 4)->data(Qt::EditRole).toString());
    fTrackControl->insert("Entry eliminated", oldValue, "");
    fTrackControl->saveChanges();
    load(ui->leInvoice->text());
    message_info_tr("The entry was eliminated");
}

void WAccInvoice::on_tblData_doubleClicked(const QModelIndex &index)
{
    if (!r__(cr__super_correction)) {
        return;
    }
    if (!index.isValid()) {
        return;
    }
    if (index.column() == 2) {
        QString oldDate = ui->tblData->item(index.row(), 2)->data(Qt::EditRole).toDate().toString(def_date_format);
        QDate date = WORKING_DATE;
        if (DlgChartDateRange::getDate(date)) {
            fDbBind[":f_wdate"] = date;
            fDbBind[":f_rdate"] = date;
            fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->item(index.row(), 0)->data(Qt::EditRole).toString())));
            fTrackControl->insert("Manual change date for " + ui->tblData->item(index.row(), 0)->data(Qt::EditRole).toString(),
                                      oldDate, date.toString(def_date_format));
            if (ui->tblData->toString(index.row(), 10) == VAUCHER_POINT_SALE_N) {
                fDbBind[":f_dateCash"] = date;
                fDbBind[":f_id"] = ui->tblData->item(index.row(), 0)->data(Qt::EditRole).toString();
                fDb.select("update o_header set f_dateCash=:f_dateCash where f_id=:f_id", fDbBind, fDbRows);
            }
            fTrackControl->saveChanges();
            load(ui->leInvoice->text());
        }
    } else if (index.column() == 4) {
        openVaucher(ui->tblData->toString(index.row(), 10), ui->tblData->toString(index.row(), 0));
    } else if (index.column() == 5 || index.column() == 6) {
        double oldAmount = ui->tblData->toDouble(index.row(), index.column());
        double newAmount = QInputDialog::getDouble(this, tr("New amount"), tr("New amount"), oldAmount);
        if (newAmount < 1) {
            return;
        }
        fDbBind[":f_amountAmd"] = newAmount;
        fDbBind[":f_amountVat"] = (index.column() == 5 ? Utils::countVATAmount(newAmount, ui->leVATMode->fHiddenText.toInt()) : 0);
        fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->item(index.row(), 0)->data(Qt::EditRole).toString())));
        fTrackControl->insert("Manual change amount for " + ui->tblData->item(index.row(), 0)->data(Qt::EditRole).toString(),
                                  float_str(oldAmount, 1), float_str(newAmount, 1));
        fTrackControl->saveChanges();
        load(ui->leInvoice->text());
    }
}

void WAccInvoice::on_deDeparture_userDateChanged(const QDate &date)
{
    if (message_confirm_tr("Are you sure to change the date of entry?") != QDialog::Accepted) {
        load(ui->leInvoice->text());
        return;
    }
    fDbBind[":f_startDate"] = date;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leReservationId->text())));
    fTrackControl->saveChanges();
}

void WAccInvoice::on_deEntry_userDateChanged(const QDate &date)
{
    if (message_confirm_tr("Are you sure to change the date of entry?") != QDialog::Accepted) {
        load(ui->leInvoice->text());
        return;
    }
    fDbBind[":f_startDate"] = date;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leReservationId->text())));
    fTrackControl->saveChanges();
}

void WAccInvoice::on_btnPostingCharges_clicked()
{
    DlgPostingCharges *p = new DlgPostingCharges(this);
    p->setInvoice(ui->leInvoice->text());
    p->exec();
    delete p;
}

void WAccInvoice::on_btnMoveItem_clicked()
{
    QModelIndexList rows = ui->tblData->selectionModel()->selectedRows();
    if (rows.count() == 0) {
        message_error_tr("Nothing was selected");
        return;
    }
    int row = rows.at(0).row();
    DlgRemotInvoices *d = new DlgRemotInvoices(true, this);
    if (d->exec() == QDialog::Accepted) {
        DatabaseResult dr;
        fDbBind[":f_invoice"] = d->fResult;
        dr.select(fDb, "select r.f_room, g.guest "
                  "from f_reservation r "
                  "inner join guests g on g.f_id=r.f_guest "
                  "where r.f_invoice=:f_invoice", fDbBind);
        fDbBind[":f_inv"] = d->fResult;
        fDbBind[":f_room"] = dr.value("f_room");
        fDbBind[":f_guest"] = dr.value("guest");
        fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->toString(row, 0))));
        fDbBind[":f_id"] = ui->tblData->toString(row, 0);
        DatabaseResult di;
        di.select(fDb, "select f_source from m_register where f_id=:f_id", fDbBind);
        if (di.value("f_source").toString() == VAUCHER_POINT_SALE_N) {
            fDbBind[":f_paymentModeComment"] = dr.value("f_room").toString() + ", " + dr.value("guest").toString();
            fDbBind[":f_roomComment"] = dr.value("f_room").toString() + ", " + dr.value("guest").toString();
            fDbBind[":f_reservation"] = ui->leReservationId->text();
            fDb.update("o_header", fDbBind, where_id(ap(ui->tblData->toString(row, 0))));
        }
        fTrackControl->insert("Entry moved to other invoice " + ui->tblData->item(row, 0)->data(Qt::EditRole).toString(),
                                  ui->tblData->toString(row, 4) + ": " + ui->tblData->toString(row, 5) + "/" + ui->tblData->toString(row, 6),
                                  d->fResult);
        fTrackControl->saveChanges();
        fTrackControl->insert("Entry moved from other invoice " + ui->tblData->item(row, 0)->data(Qt::EditRole).toString(),
                                  ui->tblData->toString(row, 4) + ": " + ui->tblData->toString(row, 5) + "/" + ui->tblData->toString(row, 6),
                                  ui->leInvoice->text());
        fTrackControl->saveChanges();
        load(ui->leInvoice->text());
    }
    delete d;
}

void WAccInvoice::on_btnChangeCL_clicked()
{
    QModelIndexList rows = ui->tblData->selectionModel()->selectedRows();
    if (rows.count() == 0) {
        message_error_tr("Nothing was selected");
        return;
    }
    int row = rows.at(0).row();
    DatabaseResult dr;
    fDbBind[":f_id"] = ui->tblData->toString(row, 0);
    dr.select(fDb, "select f_source, f_itemCode, f_cityLedger from m_register  where f_id=:f_id", fDbBind);
    if (dr.value("f_source").toString() != "RV") {
        return;
    }
    if (dr.value("f_itemCode").toInt() != 30) {
        return;
    }
    QString oldCL = dr.value("f_cityLedger").toString();
    CI_CityLedger *cl = CacheCityLedger::instance()->get(oldCL);
    if (!cl) {
        message_error_tr("Wrong cityledger code. Contact with application developer.");
        return;
    }
    QString code, name;
    if (DlgCL::getCL(code, name)) {
        fDbBind[":f_finalName"] = "C/L " + name;
        fDbBind[":f_cityLedger"] = code.toInt();
        fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->toString(row, 0))));
        fTrackControl->insert("CL Changed for  " + ui->tblData->toString(row, 0),
                                  oldCL + "/" + cl->fName, code + "/" + name);
        load(ui->leInvoice->text());
    }
}

void WAccInvoice::on_btnNewVaucher_clicked()
{
    WVaucherEditor::newVaucher(ui->leInvoice->text());
    load(ui->leInvoice->text());
}

void WAccInvoice::on_deEntry_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
}

void WAccInvoice::on_deCheckIn_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
}

void WAccInvoice::on_btnEditReserv_clicked()
{
    DlgReserveShortInfo::loadShortInfo(ui->leReservationId->text());
    load(ui->leInvoice->text());
}

void WAccInvoice::on_btnExcel_clicked()
{
    switch (DlgInvoicePrintOption::getOption()) {
    case pio_none:
        break;
    case pio_guest:
        PExportInvoiceToExcel::exportInvoice(ui->leInvoice->text(), 0);
        break;
    case pio_comp:
        PExportInvoiceToExcel::exportInvoice(ui->leInvoice->text(), 1);
        break;
    case pio_guestcomp_ser:
        PExportInvoiceToExcel::exportInvoice(ui->leInvoice->text(), 0);
        PExportInvoiceToExcel::exportInvoice(ui->leInvoice->text(), 1);
        break;
    case pio_guestcomp_tog:
        PExportInvoiceToExcel::exportInvoice(ui->leInvoice->text(), -1);
        break;
    }

}

void WAccInvoice::on_btnEditRowVaucher_clicked()
{
    QModelIndexList rows = ui->tblData->selectionModel()->selectedRows();
    if (rows.count() == 0) {
        message_error_tr("Nothing was selected");
        return;
    }
    int row = rows.at(0).row();
    WVaucherEditor::load(ui->tblData->toString(row, 0));
}

void WAccInvoice::on_chViewAllVauchers_clicked()
{
    load(ui->leInvoice->text());
}

void WAccInvoice::on_btnViewReserv_clicked()
{
    WReservation::openReserveWindows(ui->leReservationId->text());
}

void WAccInvoice::on_btnGuestInfo_clicked()
{
    DlgReservationGuests::viewGuests(ui->leReservationId->text());
}
