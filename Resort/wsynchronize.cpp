#include "wsynchronize.h"
#include "ui_wsynchronize.h"
#include "vauchers.h"
#include "databaseresult.h"
#include "excel.h"
#include "paymentmode.h"
#include "tablemodel.h"
#include <QItemDelegate>
#include <QScrollBar>
#include <QSqlRecord>
#include <QSqlField>
#include <QPainter>

class TableItemDelegate : public QItemDelegate {
private:
    EQTableWidget *fTW;
public:
    TableItemDelegate(EQTableWidget *tw) : QItemDelegate() {fTW = tw;}
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        if (index.column() == 0) {
            QItemDelegate::paint(painter, option, index);
            return;
        }
        painter->save();
        QBrush br(Qt::white, Qt::SolidPattern);
        if (fTW->item(index.row(), 0)->text() == "1") {
            br.setColor(Qt::gray);
        }
        painter->setBrush(br);
        painter->fillRect(option.rect, br);
        painter->drawText(option.rect, index.data(Qt::EditRole).toString());
        painter->restore();
    }
};

WSynchronize::WSynchronize(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WSynchronize)
{
    ui->setupUi(this);
    ui->tblData->setItemDelegate(new TableItemDelegate(ui->tblData));
    ui->rbHotel->setVisible(false);
    ui->rbEvents->setVisible(false);
    ui->rbActiveReserve->setVisible(false);
    ui->rbNoShow->setVisible(r__(cr__super_correction));
    ui->deEnd->setVisible(r__(cr__super_correction));
    connect(ui->deEnd, SIGNAL(dateChanged(QDate)), this, SLOT(dateStartChanged(QDate)));
    connect(ui->tblTotal->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(totalHScroll(int)));
}

WSynchronize::~WSynchronize()
{
    delete ui;
}

void WSynchronize::setupTab()
{
    setupTabTextAndIcon(tr("Synchronize"), ":/images/database.png");
}

void WSynchronize::hotelReport()
{
    ui->chSelectAll->setChecked(false);
    ui->tblData->setColumnCount(10);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 80, 30, 100, 100, 100, 300, 100, 100, 100);
    Utils::tableSetColumnWidths(ui->tblTotal, ui->tblTotal->columnCount(),
                                0, 80, 30, 100, 100, 100, 300, 100, 100, 100);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  QString("Id"), QString("F"), QString("P"), QString("Room"), QString("Entry"),
                                  QString("Departure"), QString("Guest"), QString("Invoice"),
                                  QString("Tax"), QString("No Tax"));
    fQuery = " \
            select r.f_id, r.f, m.p, r.f_room, r.f_startDate, r.f_endDate, "
            "concat(g.f_firstName, ' ', g.f_lastName), i.f_id,\
            coalesce(tp.amount, 0) as printed, coalesce(tn.amount, 0) as noprinted\
            from f_reservation r \
            left join m_v_invoice i on r.f_invoice=i.f_id \
            inner join f_guests g on g.f_id=r.f_guest \
            left join (select f_inv, sum(f_amountAmd) as amount from m_register where f_finance=1 and f_fiscal=1 and f_canceled=0 group by 1) tp on tp.f_inv=i.f_id\
            left join (select f_inv, sum(f_amountAmd) as amount from m_register where f_finance=1 and f_fiscal=0 and f_canceled=0 group by 1) tn on tn.f_inv=i.f_id\
            left join (select f_inv, sum(p) as p from m_register where f_finance=1 and f_canceled=0 ) m on m.f_inv=r.f_invoice \
            where i.f_wdate between :f_wdate1 and :f_wdate2 and r.f_state=3";
    fQuery.replace(":f_wdate1", ui->deDate->dateMySql()).replace(":f_wdate2", ui->deEnd->dateMySql());
    fDb.select(fQuery, fDbBind, fDbRows);
    ui->tblData->clearContents();
    Utils::fillTableWithData(ui->tblData, fDbRows);
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 1);
        item->setCheckState(item->data(Qt::EditRole).toInt() ? Qt::Checked : Qt::Unchecked);
        item->setFlags(item->data(Qt::EditRole).toInt() ? item->flags() ^ Qt::ItemIsUserCheckable : item->flags() | Qt::ItemIsUserCheckable);
        item = ui->tblData->item(i, 2);
        item->setCheckState(Qt::Unchecked);
    }
}

void WSynchronize::restaurantReport()
{
    ui->chSelectAll->setChecked(false);
    ui->tblData->setColumnCount(8);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                100, 100, 50, 100, 150, 80, 100, 200);
    Utils::tableSetColumnWidths(ui->tblTotal, ui->tblData->columnCount(),
                                100, 100, 50, 100, 150, 80, 100, 200);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  QString("F"), QString("Code"), QString("Tax"), QString("Amount"), QString(tr("State")), QString(tr("Table")),
                                  QString(tr("Mode of payment")), QString(tr("Comment")));
    fDbBind[":f_transferToRoom"] = PAYMENT_ROOM;
    fQuery = "\
            select h.f, h.f_id, h.f_tax, h.f_total, hs.f_en, \
            t.f_name, pm.f_en, h.f_paymentModeComment \
            from o_header h \
            left join o_state hs on hs.f_id=h.f_state \
            left join r_table t on t.f_id=h.f_table \
            left join f_payment_type pm on pm.f_id=h.f_paymentMode \
            where h.f_dateCash between :f_dateCash1 and :f_dateCash2  and h.f_paymentMode <> :f_transferToRoom \
        ";
    fQuery.replace(":f_dateCash1", ui->deDate->dateMySql()).replace(":f_dateCash2", ui->deEnd->dateMySql());
    fDb.select(fQuery, fDbBind, fDbRows);
    ui->tblData->clearContents();
    Utils::fillTableWithData(ui->tblData, fDbRows);
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        item->setCheckState(item->data(Qt::EditRole).toInt() ? Qt::Checked : Qt::Unchecked);
        total += ui->tblData->toDouble(i, 3);
    }
    ui->tblTotal->setItem(0, 3, new QTableWidgetItem(float_str(total, 2)));
    QStringList t;
    t << QString::number(ui->tblData->rowCount());
    ui->tblTotal->setVerticalHeaderLabels(t);
}

void WSynchronize::vauchersReport()
{
    ui->chSelectAll->setChecked(false);
    ui->tblData->setColumnCount(11);
    ui->tblTotal->setColumnCount(11);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                50, 80, 100, 300, 100, 60, 200, 150, 100, 100, 50);
    Utils::tableSetColumnWidths(ui->tblTotal, ui->tblData->columnCount(),
                                50, 80, 100, 300, 100, 60, 200, 150, 100, 100, 50);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  QString("F"), QString("#"), QString("Source"), QString("Name"), QString("Date"),
                                  QString("Room"), QString("Guest"), QString("Payment mode"), QString("Comment"),
                                  QString("Amount"), QString("Tax"));
    fQuery = "\
        select m.p, m.f_id, m.f_source, m.f_finalName, m.f_wdate, m.f_room, m.f_guest, p.f_" + def_lang +
        ", m.f_paymentComment, m.f_amountAmd, m.f_fiscal \
        from m_register m \
        left join f_payment_type p on p.f_id=m.f_paymentMode \
        where :rf (f_source='AV' and  f_wdate between :f_wdate1 and :f_wdate2 and f_canceled=0) or ((f_inv='' or f_inv='0') and f_canceled=0 and f_finance=1 and f_source<>'PS' \
        and f_wdate between :f_wdate1 and :f_wdate2 and (f_source in (:f_source) or "
            "((f_inv='' or f_inv='0') and f_itemCode=:breakfast))) \
             ";
    QString source = "'PE'";
    if (r__(cr__export_single_vauchers)) {
        source += ",'CH', 'RV', 'AV', 'RM'";
        fQuery.replace(":rf", "(f_source='RF' and  f_wdate between :f_wdate1 and :f_wdate2 and f_canceled=0) or ");
    } else {
        fQuery.replace(":rf", "");
    }
    fQuery.replace(":f_wdate1", ui->deDate->dateMySql()).replace(":f_wdate2", ui->deEnd->dateMySql());
    fQuery.replace(":breakfast", fPreferences.getDb(def_auto_breakfast_id).toString());
    fQuery.replace(":f_source", source);
    fDb.select(fQuery, fDbBind, fDbRows);
    ui->tblData->clearContents();
    Utils::fillTableWithData(ui->tblData, fDbRows);
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        item->setCheckState(item->data(Qt::EditRole).toInt() ? Qt::Checked : Qt::Unchecked);
        total += ui->tblData->toDouble(i, 9);
        if (item->checkState() == Qt::Checked) {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(COLOR_VERY_DARK_ROW);
                ui->tblData->item(i, j)->setTextColor(Qt::white);
            }
        } else {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(Qt::white);
                ui->tblData->item(i, j)->setTextColor(Qt::black);
            }
        }
    }
    ui->tblTotal->setItem(0, 9, new QTableWidgetItem(float_str(total, 2)));
    QStringList t;
    t << QString::number(ui->tblData->rowCount());
    ui->tblTotal->setVerticalHeaderLabels(t);
}

void WSynchronize::noShowReport()
{
    ui->chSelectAll->setChecked(false);
    ui->tblData->setColumnCount(6);
    ui->tblTotal->setColumnCount(6);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                50, 100, 200, 100, 300, 100);
    Utils::tableSetColumnWidths(ui->tblTotal, ui->tblData->columnCount(),
                                50, 100, 200, 100, 300, 100);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  QString("F"), QString("Invoice"), QString("Name"), QString("Room"), QString("Guest"), QString("Amount"));
    fQuery = "select r.f, m.f_inv, m.f_finalName, r.f_room, g.guest, m.f_amountAmd "
            "from f_reservation r "
            "left join m_register m on m.f_inv=r.f_invoice "
            "left join guests g on g.f_id=r.f_guest "
            "where m.f_itemCode in (" + QString("%1,%2").arg(fPreferences.getDb(def_noshowfee_code).toString()).arg(fPreferences.getDb(def_cancelfee_code).toString()) + ") "
            "and m.f_wdate between " + ui->deDate->dateMySql() + " and " + ui->deEnd->dateMySql() + " "
            "and m.f_canceled=0 ";
    fDb.select(fQuery, fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        item->setCheckState(item->data(Qt::EditRole).toInt() ? Qt::Checked : Qt::Unchecked);
        total += ui->tblData->toDouble(i, 5);
        if (item->checkState() == Qt::Checked) {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(COLOR_VERY_DARK_ROW);
                ui->tblData->item(i, j)->setTextColor(Qt::white);
            }
        } else {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(Qt::white);
                ui->tblData->item(i, j)->setTextColor(Qt::black);
            }
        }
    }
}

void WSynchronize::checkinReservedReport()
{

}

void WSynchronize::eventReport()
{
    ui->chSelectAll->setChecked(false);
    ui->tblData->setColumnCount(11);
    ui->tblTotal->setColumnCount(11);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                50, 80, 100, 300, 100, 60, 200, 150, 100, 100, 50);
    Utils::tableSetColumnWidths(ui->tblTotal, ui->tblData->columnCount(),
                                50, 80, 100, 300, 100, 60, 200, 150, 100, 100, 50);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  QString("F"), QString("#"), QString("Source"), QString("Name"), QString("Date"),
                                  QString("Room"), QString("Guest"), QString("Payment mode"), QString("Comment"),
                                  QString("Amount"), QString("Tax"));
    fQuery = "\
        select m.p, m.f_id, m.f_source, m.f_finalName, m.f_wdate, m.f_room, m.f_guest, p.f_" + def_lang +
        ", m.f_paymentComment, m.f_amountAmd, m.f_fiscal \
        from m_register m \
        left join f_payment_mode p on p.f_id=m.f_paymentMode \
        where ((f_inv='' or f_inv='0') and f_canceled=0 and f_finance=1 and f_source<>'PS' \
        and f_wdate between :f_wdate1 and :f_wdate2 and (f_source in (:f_source) or "
            "((f_inv='' or f_inv='0') and f_itemCode=:breakfast))) \
             ";
    fQuery.replace(":f_wdate1", ui->deDate->dateMySql()).replace(":f_wdate2", ui->deEnd->dateMySql());
    QString source = "'PE'";
    if (r__(cr__export_single_vauchers)) {

    }
    fQuery.replace(":breakfast", fPreferences.getDb(def_auto_breakfast_id).toString());
    fQuery.replace(":f_source", source);
    fDb.select(fQuery, fDbBind, fDbRows);
    ui->tblData->clearContents();
    Utils::fillTableWithData(ui->tblData, fDbRows);
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        item->setCheckState(item->data(Qt::EditRole).toInt() ? Qt::Checked : Qt::Unchecked);
        total += ui->tblData->toDouble(i, 9);
        if (item->checkState() == Qt::Checked) {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(COLOR_VERY_DARK_ROW);
                ui->tblData->item(i, j)->setTextColor(Qt::white);
            }
        } else {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(Qt::white);
                ui->tblData->item(i, j)->setTextColor(Qt::black);
            }
        }
    }
    ui->tblTotal->setItem(0, 9, new QTableWidgetItem(float_str(total, 2)));
    QStringList t;
    t << QString::number(ui->tblData->rowCount());
    ui->tblTotal->setVerticalHeaderLabels(t);
}

bool WSynchronize::goHotel()
{
    // List of reservations id that need to transfer to other database
    QList<int> ids;
    QList<int> invoiceIds;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 1);
        if (item->flags() & Qt::ItemIsUserCheckable) {
            if (item->checkState() == Qt::Checked) {
                ids.append(ui->tblData->item(i, 0)->data(Qt::EditRole).toInt());
                invoiceIds.append(ui->tblData->item(i, 7)->data(Qt::EditRole).toInt());
            }
        }
    }
    //TABLE: f_reservations
    QMap<int, QMap<QString, QVariant> > reservations;
    QSqlQuery *q = fDb.select("select * from f_reservation "
           "where f_state=3 and f_invoice in "
            "(select f_id from m_v_invoice where f_wdate=" + ui->deDate->dateMySql() + ")");
    while (q->next()) {
        if (!ids.contains(q->value(0).toInt())) {
            continue;
        }
        QMap<QString, QVariant> row;
        for (int i = 0; i < q->record().count(); i++) {
            row[":" + q->record().fieldName(i)] = q->value(i);
        }
        reservations[q->value(0).toInt()] = row;
    }
    //TABLE: f_reservation_guest
    QMap<int, QMap<QString, QVariant> > guests;
    q = fDb.select(q, "select * from f_reservation_guests where f_reservation in "
                   "(select f_id from f_reservation where f_state=3 and f_invoice in "
                   "(select f_id from m_v_invoice where f_wdate=" + ui->deDate->dateMySql() + "))", fDbBind);
    while (q->next()) {
        if (!ids.contains(q->value("f_reservation").toInt())) {
            continue;
        }
        QMap<QString, QVariant> row;
        for (int i = 0; i < q->record().count(); i++) {
            row[":" + q->record().fieldName(i)] = q->value(i);
        }
        guests[q->value(0).toInt()] = row;
    }
    //TABLE: m_v_invoice
    QMap<int, QMap<QString, QVariant> > invoices;
    q = fDb.select(q, "select * from m_v_invoice where f_wdate=" + ui->deDate->dateMySql(), fDbBind);
    while (q->next()) {
        if (!invoiceIds.contains(q->value(0).toInt())) {
            continue;
        }
        QMap<QString, QVariant> row;
        for (int i = 0; i < q->record().count(); i++) {
            row[":" + q->record().fieldName(i)] = q->value(i);
        }
        invoices[q->value(0).toInt()] = row;
    }
    //TABLE: m_register
    QList<int> oheaders;
    QMap<int, QMap<QString, QVariant> > registers;
    q = fDb.select(q, "select * from m_register where f_canceled=0 and f_inv in ("
                   "select f_id from m_v_invoice where f_wdate=" + ui->deDate->dateMySql() + ")", fDbBind);
    while (q->next()) {
        if (!invoiceIds.contains(q->value("f_inv").toInt())) {
            continue;
        }
        QMap<QString, QVariant> row;
        for (int i = 0; i < q->record().count(); i++) {
            row[":" + q->record().fieldName(i)] = q->value(i);
        }
        registers[q->value(0).toInt()] = row;
        if (row[":f_source"].toString() == "PS") {
            oheaders.append(row[":f_doc"].toInt());
        }
    }
    //TABLE: o_header
    bool first = true;
    QString headersIds;
    QMap<int, QMap<QString, QVariant> > headers;
    foreach (int i, oheaders) {
        if (first) {
            first = false;
        } else {
            headersIds += ",";
        }
        headersIds += QString::number(i);
    }
    q = fDb.select(q, "select * from o_header where f_id in (" + headersIds + ")", fDbBind);
    while (q->next()) {
        QMap<QString, QVariant> row;
        for (int i = 0; i < q->record().count(); i++) {
            row[":" + q->record().fieldName(i)] = q->value(i);
        }
        headers[q->value(0).toInt()] = row;
    }
    QMap<int, QMap<QString, QVariant> > dishes;
    q = fDb.select(q, "select * from o_dish where f_state=1 and f_header in (" + headersIds + ")", fDbBind);
    while (q->next()) {
        QMap<QString, QVariant> row;
        for (int i = 0; i < q->record().count(); i++) {
            row[":" + q->record().fieldName(i)] = q->value(i);
        }
        dishes[q->value(0).toInt()] = row;
    }
    //START TRANSFER
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return false;
    }
    Database db;
    bool result = true;
    db.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!db.open()) {
        message_error_tr("Cannot connect to second database");
        return false;
    }
    QMap<QString, QVariant> dbBind;
    db.fDb.transaction();
    fDb.fDb.transaction();
    //TRANSFER RESERVATIONS
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = reservations.begin(); it != reservations.end(); it++) {
            it.value().remove(":f_id");
            it.value()[":f_id"] = db.insert("f_reservation", it.value());
            result = result && it.value()[":f_id"].toInt() > 0;
            if (!result) {
                break;
            }
        }
    }
    //TRANSFER GUESTS
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = guests.begin(); it != guests.end(); it++) {
            it.value().remove(":f_id");
            it.value()[":f_reservation"] = reservations[it.value()[":f_reservation"].toInt()][":f_id"];
            it.value()[":f_id"] = db.insert("f_reservation_guests", it.value());
            result = result && it.value()[":f_id"].toInt() > 0;
            if (!result) {
                break;
            }
        }
    }
    //TRANSFER INVOICES
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = invoices.begin(); it != invoices.end(); it++) {
            it.value().remove(":f_id");
            it.value()[":f_id"] = db.insert("m_v_invoice", it.value());
            result = result && it.value()[":f_id"].toInt() > 0;
            if (!result) {
                break;
            }
            dbBind[":f_invoice"] = it.value()[":f_id"];
            db.update("f_reservation", dbBind, where_field("f_invoice", it.key()));
        }
    }
    //TRANSFER REGISTERS
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = registers.begin(); it != registers.end(); it++) {
            it.value().remove(":f_id");
            it.value()[":f_inv"] = invoices[it.value()[":f_inv"].toInt()][":f_id"];
            it.value()[":f_id"] = db.insert("m_register", it.value());
            result = result && it.value()[":f_id"].toInt() > 0;
            if (!result) {
                break;
            }
        }
    }
    //TRANSFER O_HEADER
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = headers.begin(); it != headers.end(); it++) {
            it.value().remove(":f_id");
            it.value()[":f_id"] = db.insert("o_header", it.value());
            result = result && it.value()[":f_id"].toInt() > 0;
            if (!result) {
                break;
            }
        }
    }
    //TRANSFER O_DISH
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = dishes.begin(); it != dishes.end(); it++) {
            it.value().remove(":f_id");
            it.value()[":f_header"] = headers[it.value()[":f_header"].toInt()][":f_id"];
            it.value()[":f_id"] = db.insert("o_dish", it.value());
            result = result && it.value()[":f_id"].toInt() > 0;
        }
    }
    //UPDATE NEW IDS IN CURRENT DATABASE
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = reservations.begin(); it != reservations.end(); it++) {
            fDbBind[":f"] = it.value()[":f_id"];
            result = result && fDb.update("f_reservation", fDbBind, where_id(it.key()));
            if (!result) {
                break;
            }
            dbBind[":f"] = it.key();
            result = result && db.update("f_reservation", dbBind, where_id(it.value()[":f_id"].toInt()));
            if (!result) {
                break;
            }
        }
    }
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = invoices.begin(); it != invoices.end(); it++) {
            fDbBind[":f"] = it.value()[":f_id"];
            result = result && fDb.update("m_v_invoice", fDbBind, where_id(it.key()));
            if (!result) {
                break;
            }
            dbBind[":f"] = it.key();
            result = result && db.update("m_v_invoice", dbBind, where_id(it.value()[":f_id"].toInt()));
            if (!result) {
                break;
            }
        }
    }
    if (result) {
        for (QMap<int, QMap<QString, QVariant> >::iterator it = headers.begin(); it != headers.end(); it++) {

        }
    }
    if (result) {
        db.fDb.commit();
        fDb.fDb.commit();
    } else {
        db.fDb.rollback();
        fDb.fDb.rollback();
    }

    return result;
}

bool WSynchronize::goRestaurant()
{
    Database db;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return false;
    }
    db.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!db.open()) {
        message_error_tr("Cannot connect to second database");
        return false;
    }

    QStringList ids;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        if (item->text().toInt() == 0) {
            if (item->checkState() == Qt::Checked) {
                ids.append(ui->tblData->item(i, 1)->data(Qt::EditRole).toString());
            }
        }
    }

    fDb.fDb.transaction();
    db.fDb.transaction();
    bool result = true;
    QMap<QString, QVariant> v;
    foreach (QString ord, ids) {
        DatabaseResult drHeader;
        fDbBind[":f_id"] = ord;
        v[":f_id"] = ord;
        if (ui->chOverride->isChecked()) {
            db.select("delete from o_header where f_id=:f_id", v, fDbRows);
            v[":f_id"] = ord;
            db.select("delete from m_register where f_id=:f_id", v, fDbRows);
            v[":f_id"] = ord;
            db.select("delete from o_dish where f_header=:f_id", v, fDbRows);
        }
        drHeader.select(fDb, "select * from o_header where f_id=:f_id", fDbBind);

        DatabaseResult drVaucher;
        fDbBind[":f_id"] = ord;
        drVaucher.select(fDb, "select * from m_register where f_id=:f_id", fDbBind);

        DatabaseResult drDishes;
        fDbBind[":f_header"] = ord;
        drDishes.select(fDb, "select * from o_dish where f_header=:f_header", fDbBind);

        QMap<QString, QVariant> sdbBind;
        drHeader.getBindValues(0, sdbBind);
        result =  db.insertWithoutId("o_header", sdbBind);
        if (!result) {
            message_error(db.fLastError);
            break;
        }

        if (drHeader.value("f_state").toInt() == ORDER_STATE_CLOSED) {
            if (drVaucher.rowCount() > 0) {
                drVaucher.getBindValues(0, sdbBind);
                result = db.insertWithoutId("m_register", sdbBind);
            }
            if (!result) {
                message_error(db.fLastError);
                break;
            }
        }

        for (int i = 0, count = drDishes.rowCount(); i < count; i++) {
            drDishes.getBindValues(i, sdbBind);
            result = db.insertWithoutId("o_dish", sdbBind);
            if (!result) {
                message_error(db.fLastError);
                goto MARK;
            }
        }

        fDbBind[":f"] = 1;
        fDb.update("o_header", fDbBind, where_id(ap(ord)));
        fDbBind[":p"] = 1;
        fDb.update("m_register", fDbBind, where_id(ap(ord)));
    }

    if (result) {
        fDb.fDb.commit();
        db.fDb.commit();
    } else {
        fDb.fDb.rollback();
        db.fDb.rollback();
    }
    MARK:

    return result;
}

bool WSynchronize::goVauchers()
{
    Database db;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return false;
    }
    db.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!db.open()) {
        message_error_tr("Cannot connect to second database");
        return false;
    }

    QStringList ids;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        if (item->text().toInt() == 0) {
            if (item->checkState() == Qt::Checked) {
                ids.append(ui->tblData->item(i, 1)->data(Qt::EditRole).toString());
            }
        }
    }

    fDb.fDb.transaction();
    db.fDb.transaction();
    bool result = true;
    foreach (QString v, ids) {
        DatabaseResult drVaucher;
        fDbBind[":f_id"] = v;
        drVaucher.select(fDb, "select * from m_register where f_id=:f_id", fDbBind);
        QMap<QString, QVariant> sdbBind;
        drVaucher.getBindValues(0, sdbBind);
        if (ui->chOverride->isChecked()) {
            QMap<QString, QVariant> tb;
            QList<QList<QVariant> > tr;
            tb[":f_id"] = v;
            db.select("delete from m_register where f_id=:f_id", tb, tr);
        }
        if (sdbBind[":f_cityLedger"].toInt() > 0) {
            QMap<QString, QVariant> be;
            be[":f_id"] = sdbBind[":f_cityLedger"];
            DatabaseResult drbe;
            drbe.select(db, "select * from f_city_ledger where f_id=:f_id", be);
            if (drbe.rowCount() == 0) {
                result = false;
                message_error(tr("City ledger is not exists in destination database"));
            } else {
                if (sdbBind[":f_source"].toString() == VAUCHER_CHECKOUT_N
                        || sdbBind[":f_source"].toString() == VAUCHER_REFUND_N
                        || sdbBind[":f_source"].toString() == VAUCHER_RECEIPT_N
                        || sdbBind[":f_source"].toString() == VAUCHER_ROOM_RATE_N
                        || sdbBind[":f_source"].toString() == VAUCHER_POINT_SALE_N) {
                    sdbBind[":f_paymentComment"] = drbe.value("f_name").toString();

                    if (sdbBind[":f_source"].toString() == VAUCHER_RECEIPT_N) {
                        sdbBind[":f_finalName"] = tr("PAYMENT") + " " + drbe.value("f_name").toString();
                    }
                    if (sdbBind[":f_source"].toString() == VAUCHER_REFUND_N) {
                        sdbBind[":f_finalName"] = tr("REFUND") + " " + drbe.value("f_name").toString();
                    }
                }
            }
        }

        result = db.insertWithoutId("m_register", sdbBind);
        if (!result) {
            message_error(db.fLastError);
            break;
        }
        DatabaseResult tmp;
        if (drVaucher.value("f_source") == VAUCHER_EVENT_N) {
            fDbBind[":f_id"] = v;
            tmp.select(fDb, "select * from o_event where f_id=:f_id", fDbBind);
            tmp.getBindValues(0, sdbBind);
            if (ui->chOverride->isChecked()) {
                QMap<QString, QVariant> tb;
                QList<QList<QVariant> > tr;
                tb[":f_id"] = v;
                db.select("delete from o_event where f_id=:f_id", tb, tr);
            }
            result = db.insertWithoutId("o_event", sdbBind);
            if (!result) {
                message_error(db.fLastError);
                break;
            }
        } else if (drVaucher.value("f_itemCode").toInt() == fPreferences.getDb(def_auto_breakfast_id).toInt()) {
            fDbBind[":f_id"] = v;
            tmp.select(fDb, "select * from o_breakfast where f_id=:f_id", fDbBind);
            tmp.getBindValues(0, sdbBind);
            if (ui->chOverride->isChecked()) {
                QMap<QString, QVariant> tb;
                QList<QList<QVariant> > tr;
                tb[":f_id"] = v;
                db.select("delete from o_breakfast where f_id=:f_id", tb, tr);
            }
            result = db.insertWithoutId("o_breakfast", sdbBind);
            if (!result) {
                message_error(db.fLastError);
                break;
            }
        } else if (drVaucher.value("f_source").toString() == VAUCHER_ADVANCE_N) {
            DatabaseResult dreserv;
            fDbBind[":f_invoice"] = drVaucher.value("f_inv").toString();
            dreserv.select(fDb, "select * from f_reservation where f_invoice=:f_inv", fDbBind);
            if (dreserv.rowCount() > 0) {
                dreserv.getBindValues(0, fDbBind);
                if (ui->chOverride->isChecked()) {
                    QMap<QString, QVariant> tb;
                    QList<QList<QVariant> > tr;
                    tb[":f_id"] = dreserv.value("f_id");
                    db.select("delete from f_reservation where f_id=:f_id", tb, tr);
                }
                result = db.insertWithoutId("f_reservation", fDbBind);
                if (!result) {
                    message_error(db.fLastError);
                    break;
                }
                DatabaseResult dguest;

            }
        }
        fDbBind[":p"] = 1;
        fDb.update("m_register", fDbBind, where_id(ap(v)));
    }

    if (result) {
        fDb.fDb.commit();
        db.fDb.commit();
    } else {
        fDb.fDb.rollback();
        db.fDb.rollback();
    }

    return result;
}

bool WSynchronize::goNoShow()
{
    bool result = true;
    Database db;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return false;
    }
    db.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!db.open()) {
        message_error_tr("Cannot connect to second database");
        return false;
    }
    QStringList ids;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 0);
        if (item->text().toInt() == 0) {
            if (item->checkState() == Qt::Checked) {
                ids.append(ui->tblData->item(i, 1)->data(Qt::EditRole).toString());
            }
        }
    }
    fDb.fDb.transaction();
    db.fDb.transaction();
    foreach (QString id, ids) {
        DatabaseResult dr;
        fDbBind[":f_invoice"] = id;
        dr.select(fDb, "select * from f_reservation where f_invoice=:f_invoice", fDbBind);
        int guest = dr.value("f_guest").toInt();
        dr.getBindValues(0, fDbBind);
        result = result && db.insertWithoutId("f_reservation", fDbBind);
        if (!result) {
            message_error(db.fLastError);
            goto mark;
        }
        fDbBind[":f_id"] = guest;
        dr.select(fDb, "select * from f_guests where f_id=:f_id", fDbBind);
        dr.getBindValues(0, fDbBind);
        db.insertWithoutId("f_guests", fDbBind);
        fDbBind[":f_inv"] = id;
        dr.select(fDb, "select * from m_register where f_inv=:f_inv", fDbBind);
        for (int i = 0; i < dr.rowCount(); i++) {
            dr.getBindValues(i, fDbBind);
            if (ui->chOverride->isChecked()) {
                QMap<QString, QVariant> tb;
                QList<QList<QVariant> > tr;
                tb[":f_id"] = id;
                db.select("delete from m_register where f_id=:f_id", tb, tr);
            }
            result = result && db.insertWithoutId("m_register", fDbBind);if (!result) {
                message_error(db.fLastError);
                goto mark;
            }
        }
        fDbBind[":f"] = 1;
        fDb.update("f_reservation", fDbBind, where_field("f_invoice", ap(id)));
    }
    mark:
    if (result) {
        fDb.fDb.commit();
        db.fDb.commit();
    } else {
        fDb.fDb.rollback();
        db.fDb.rollback();
    }
    on_btnRefresh_clicked();
    return result;
}

bool WSynchronize::goCheckinReserved()
{
    return true;
}

void WSynchronize::clearRestaurant()
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->item(i, 0)->checkState() == Qt::Checked) {
            fDbBind[":f"] = 0;
            fDb.update("o_header", fDbBind, where_id(ap(ui->tblData->toString(i, 1))));
        }
    }
    on_btnRefresh_clicked();
}

void WSynchronize::clearVauchers()
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->item(i, 0)->checkState() == Qt::Checked) {
            fDbBind[":p"] = 0;
            fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->toString(i, 1))));
        }
    }
    on_btnRefresh_clicked();
}

void WSynchronize::clearNoShow()
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->item(i, 0)->checkState() == Qt::Checked) {
            fDbBind[":f"] = 0;
            fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblData->toString(i, 1))));
        }
    }
    on_btnRefresh_clicked();
}

void WSynchronize::dateStartChanged(const QDate &date)
{
    if (!ui->deEnd->isVisible()) {
        ui->deEnd->setDate(date);
    }
}

void WSynchronize::totalHScroll(int value)
{
    ui->tblData->horizontalScrollBar()->setValue(value);
}

void WSynchronize::on_btnRefresh_clicked()
{
    ui->tblTotal->clearContents();
    if (ui->rbHotel->isChecked()) {
        hotelReport();
    } else if (ui->rbRestaurant->isChecked()) {
        restaurantReport();
    } else if (ui->rbNoShow->isChecked()) {
        noShowReport();
    } else if (ui->rbVauchers->isChecked()) {
        vauchersReport();
    } else {
        eventReport();
    }
}

void WSynchronize::on_btnGo_clicked()
{
    bool result;
    if (ui->rbHotel->isChecked()) {
        result = goHotel();
    } else if (ui->rbRestaurant->isChecked()) {
        result = goRestaurant();
    } else if (ui->rbNoShow->isChecked()) {
        result = goNoShow();
    } else {
        result = goVauchers();
    }
    on_btnRefresh_clicked();
    if (result) {
        message_info_tr("Done.");
    } else {
        message_error_tr("Synchronization failed");
    }
}

void WSynchronize::on_chSelectAll_clicked(bool checked)
{
    if (ui->rbHotel->isChecked()) {
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            QTableWidgetItem *item = ui->tblData->item(i, 1);
            if (item->flags() & Qt::ItemIsUserCheckable) {
                item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
            }
        }
    } else {
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            QTableWidgetItem *item = ui->tblData->item(i, 0);
            if (item->flags() & Qt::ItemIsUserCheckable) {
                item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
            }
        }
    }
}

void WSynchronize::on_rbRestaurant_clicked()
{
    on_btnRefresh_clicked();
}

void WSynchronize::on_rbHotel_clicked()
{
    on_btnRefresh_clicked();
}

void WSynchronize::on_rbVauchers_clicked()
{
    on_btnRefresh_clicked();
}

void WSynchronize::on_btnNextDate_clicked()
{
    ui->deDate->setDate(ui->deDate->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    on_btnRefresh_clicked();
}

void WSynchronize::on_btnPrevDate_clicked()
{
    ui->deDate->setDate(ui->deDate->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    on_btnRefresh_clicked();
}

void WSynchronize::on_btnRemoveSync_clicked()
{
    if (message_confirm_tr("Confirm to clear synchronization data for selected entries") != QDialog::Accepted) {
        return;
    }
    if (ui->rbRestaurant->isChecked()) {
        clearRestaurant();
    } else if (ui->rbNoShow->isChecked()) {
        clearNoShow();
    } else {
        clearVauchers();
    }
}

void WSynchronize::on_btnExcel_clicked()
{
    Excel e;
    int colCount = ui->tblData->columnCount();
    int rowCount = ui->tblData->rowCount();
    for (int i = 0; i < colCount; i++) {
        e.setValue(ui->tblData->horizontalHeaderItem(i)->text(), 1, i + 1);
        e.setColumnWidth(i + 1, ui->tblData->columnWidth(i) / 7);
    }
    e.setFontBold(e.address(0, 0), e.address(0, colCount - 1));
    e.setHorizontalAlignment(e.address(0, 0), e.address(0, colCount - 1), Excel::hCenter);

    for (int j = 0; j < rowCount; j++) {
        for (int i = 0; i < colCount; i++) {
            e.setValue(ui->tblData->toString(j, i), j + 2, i + 1);
        }
        QColor c = ui->tblData->item(j, 0)->backgroundColor();
        e.setBackground(e.address(j + 1, 0), e.address(j + 1, ui->tblData->columnCount() - 1), c.red(), c.green(), c.blue());
        QColor fcolor = ui->tblData->item(j, 0)->textColor();
        e.setFontColor(e.address(j + 1, 0), e.address(j + 1, ui->tblData->columnCount() - 1),
                       fcolor.red(), fcolor.green(), fcolor.blue());
    }

    e.setFontSize(e.address(0, 0), e.address(rowCount, colCount - 1), 10);
    e.show();
}
