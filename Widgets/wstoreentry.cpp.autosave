#include "wstoreentry.h"
#include "excel.h"
#include "ui_wstoreentry.h"
#include "pprintstoreentry.h"

#define SEL_DISH 1

WStoreEntry::WStoreEntry(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WStoreEntry)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                     0, 0, 300, 80, 80, 80, 80);

    fDockDish = new DWSelectorDish(this);
    fDockDish->configure();
    fDockDish->setDialog(this, SEL_DISH);

    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, SIGNAL(store(CI_RestStore*)), this, SLOT(store(CI_RestStore*)));
}

WStoreEntry::~WStoreEntry()
{
    delete ui;
}

void WStoreEntry::setup()
{
    setupTabTextAndIcon(tr("Store entry"), ":/images/storeentry.png");
}

void WStoreEntry::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_DISH: {
        CI_Dish *c = value.value<CI_Dish*>();
        if (c) {
            newGoods(c);
            ui->tblData->setVisible(false);
            ui->tblData->setVisible(true);
            ui->tblData->scrollToBottom();
        }
    }
    }
}

void WStoreEntry::openDoc(int doc)
{
    WStoreEntry *s = addTab<WStoreEntry>();
    if (doc > 0) {
        s->load(doc);
    }
}

void WStoreEntry::load(int doc)
{
    ui->leDocNum->setInt(doc);
    DatabaseResult dr;
    fDbBind[":f_id"] = doc;
    dr.select(fDb, "select * from st_header where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Invalid document number"));
        return;
    }
    ui->leStore->setInt(dr.value("f_store").toInt());
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, ui->leStoreName, 0);
    ui->leAmount->setDouble(dr.value("f_amount").toDouble());
    ui->deDate->setDate(dr.value("f_date").toDate());
    fDbBind[":f_doc"] = doc;
    dr.select(fDb, "select st.f_id, st.f_goods, g.f_en, st.f_qty, st.f_amount from st_body st "
              "left join r_dish g on g.f_id=st.f_goods where f_doc=:f_doc", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        CI_Dish *d = CacheDish::instance()->get(dr.value(i, "f_goods").toString());
        if (!d) {
            message_error(tr("Program error. Please, contact with developer. Message: CI_Dish==0, loadDoc"));
            return;
        }
        newGoods(d);
        ui->tblData->setItemWithValue(i, 0, dr.value(i, "f_id"));
        ui->tblData->lineEdit(i, 3)->setDouble(dr.value(i, "f_qty").toDouble());
        if (dr.value(i, "f_qty").toDouble() > 0.1) {
            ui->tblData->lineEdit(i, 5)->setDouble(dr.value(i, "f_amount").toDouble() / dr.value(i, "f_qty").toDouble());
        } else {
            ui->tblData->lineEdit(i, 5)->setDouble(0);
        }
        ui->tblData->lineEdit(i, 6)->setDouble(dr.value(i, "f_amount").toDouble());
    }
}

void WStoreEntry::newGoods(CI_Dish *c)
{
    if (!c) {
        return;
    }
    int row = ui->tblData->rowCount();
    ui->tblData->setRowCount(row + 1);
    ui->tblData->setItemWithValue(row, 0, "");
    ui->tblData->setItemWithValue(row, 1, c->fCode);
    ui->tblData->setItemWithValue(row, 2, c->fName);
    EQLineEdit *l = ui->tblData->addLineEdit(row, 3, false);
    l->setValidator(new QDoubleValidator(0, 10000, 4));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(qtyChange(QString)));
    ui->tblData->setItemWithValue(row, 4, c->fUnitName);
    l = ui->tblData->addLineEdit(row, 5, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(priceChange(QString)));
    l = ui->tblData->addLineEdit(row, 6, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(totalChange(QString)));
    ui->tblData->lineEdit(row, 3)->setFocus();

}

void WStoreEntry::countTotal()
{
    float total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        total += ui->tblData->lineEdit(i, 6)->asDouble();
    }
    ui->leAmount->setDouble(total);
}

void WStoreEntry::qtyChange(const QString &arg1)
{
    Q_UNUSED(arg1)
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    int row, col;
    if (!ui->tblData->findWidgetCell(l, row, col)) {
        return;
    }
    ui->tblData->lineEdit(row, 6)->setDouble(l->asDouble() * ui->tblData->lineEdit(row, 5)->asDouble());
    countTotal();
}

void WStoreEntry::priceChange(const QString &arg1)
{
    Q_UNUSED(arg1)
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    int row, col;
    if (!ui->tblData->findWidgetCell(l, row, col)) {
        return;
    }
    ui->tblData->lineEdit(row, 6)->setDouble(l->asDouble() * ui->tblData->lineEdit(row, 3)->asDouble());
    countTotal();
}

void WStoreEntry::totalChange(const QString &arg1)
{
    Q_UNUSED(arg1)
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    int row, col;
    if (!ui->tblData->findWidgetCell(l, row, col)) {
        return;
    }
    if (ui->tblData->lineEdit(row, 3)->asDouble() < 0.001) {
        return;
    }
    ui->tblData->lineEdit(row, 5)->setDouble(l->asDouble() / ui->tblData->lineEdit(row, 3)->asDouble());
    countTotal();
}

void WStoreEntry::store(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, ui->leStoreName, c);
}

void WStoreEntry::on_toolButton_clicked()
{
    fDockDish->show();
}

void WStoreEntry::on_toolButton_2_clicked()
{
    QModelIndexList ml = ui->tblData->selectionModel()->selectedRows();
    QSet<int> rows;
    foreach (QModelIndex m, ml) {
        rows << m.row();
    }
    for (int i = rows.count() - 1; i > -1; i--) {
        ui->tblData->removeRow(rows.toList().at(i));
    };
    countTotal();
}

void WStoreEntry::on_btnSave_clicked()
{
    if (ui->leStore->asInt() == 0) {
        message_error(tr("Store must be selected"));
        return;
    }
    if (ui->leDocNum->asInt() == 0) {
        fDbBind[":f_id"] = 0;
        ui->leDocNum->setInt(fDb.insert("st_header", fDbBind));
    }
    fDbBind[":f_date"] = ui->deDate->date();
    fDbBind[":f_store"] = ui->leStore->asInt();
    fDbBind[":f_amount"] = ui->leAmount->asDouble();
    fDb.update("st_header", fDbBind, where_id(ui->leDocNum->asInt()));
    fDbBind[":f_doc"] = ui->leDocNum->asInt();
    fDb.select("delete from st_body where f_doc=:f_doc", fDbBind, fDbRows);
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        fDbBind[":f_doc"] = ui->leDocNum->asInt();
        fDbBind[":f_goods"] = ui->tblData->toInt(i, 1);
        fDbBind[":f_qty"] = ui->tblData->lineEdit(i, 3)->asDouble();
        fDbBind[":f_amount"] = ui->tblData->lineEdit(i, 5)->asDouble();
        ui->tblData->setItemWithValue(i, 0, fDb.insert("st_body", fDbBind));
    }
    message_info(tr("Saved"));
}

void WStoreEntry::on_btnRemove_clicked()
{
    if (message_confirm_tr("Confirm to delete document") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] = ui->leDocNum->asInt();
    fDb.select("delete from st_header where f_id=:f_id", fDbBind, fDbRows);
    fDbBind[":f_doc"] = ui->leDocNum->asInt();
    fDb.select("delete from st_body where f_doc=:f_doc", fDbBind, fDbRows);
    fMainWindow->fTab->removeTab(fMainWindow->fTab->indexOf(this));
    close();
}

void WStoreEntry::on_btnCalculate_clicked()
{
    if (ui->leDocNum->asInt() == 0) {
        message_error(tr("Document must be saved first"));
        return;
    }
    if (ui->leStore->asInt() == 0) {
        message_error(tr("Store must be defined"));
        return;
    }
    QString query = "select f_id, f_en from r_dish ";
    DatabaseResult dr;
    dr.select(fDb, query, fDbBind);
    //get middle prices for input docs
    query = "select r.f_material, sum(r.f_qty*r.f_sign) as f_qty, sum(r.f_total*r.f_sign) as f_total from r_body r "
            "left join r_docs d on d.f_id=r.f_doc "
            "where d.f_state=1 and d.f_date between :date1 and :date2 and d.f_type in (1) and r.f_store=:f_store "
            "group by 1";
    DatabaseResult drt;
    fDbBind[":date2"]  = ui->deDate->date();
    fDbBind[":date1"] = ui->deLastDate->date();
    fDbBind[":f_store"] = ui->leStore->asInt();
    drt.select(fDb, query, fDbBind);
    //get middle prices from st_header
    query = "select r.f_goods, sum(r.f_qty) as f_qty, sum(r.f_amount) as f_total from st_body r "
            "left join st_header d on d.f_id=r.f_doc "
            "where d.f_date = :date1 and d.f_store=:f_store  "
            "group by 1";
    DatabaseResult drt2;
    fDbBind[":date1"]  = ui->deLastDate->date().addDays(-1);
    fDbBind[":f_store"] = ui->leStore->asInt();
    drt2.select(fDb, query, fDbBind);

    struct A {
        double qty;
        double total;
        A() {qty = 0.0; total = 0.0;}
    };

    QMap<int, A> goodsList;
    for (int i = 0; i < drt.rowCount(); i++) {
        if (!goodsList.contains(drt.value(i, "f_material").toInt())) {
            A a;
            goodsList[drt.value(i, "f_material").toInt()] = a;
        }
        goodsList[drt.value(i, "f_material").toInt()].qty = goodsList[drt.value(i, "f_material").toInt()].qty + drt.value(i, "f_qty").toDouble();
        goodsList[drt.value(i, "f_material").toInt()].total = goodsList[drt.value(i, "f_material").toInt()].total + drt.value(i, "f_total").toDouble();
    }

    for (int i = 0; i < drt2.rowCount(); i++) {
        if (!goodsList.contains(drt2.value(i, "f_material").toInt())) {
            A a;
            goodsList[drt2.value(i, "f_material").toInt()] = a;
        }
        goodsList[drt2.value(i, "f_material").toInt()].qty = goodsList[drt2.value(i, "f_material").toInt()].qty + drt2.value(i, "f_qty").toDouble();
        goodsList[drt2.value(i, "f_material").toInt()].total = goodsList[drt2.value(i, "f_material").toInt()].total + drt2.value(i, "f_total").toDouble();
    }

    //check for forgoted goods and insert that with 0 qty
    for (QMap<int, A>::const_iterator it = goodsList.begin(); it != goodsList.end(); it++) {
        bool found = false;
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            if (it.key() == ui->tblData->toInt(i, 1)) {
                found = true;
                break;
            }
        }
        if (!found) {
            CI_Dish *cd = CacheDish::instance()->get(it.key());
            if (cd) {
                newGoods(cd);
            }
        }
    }

    QMap<int, double> qty;
    QMap<int, double> price;
    //correct qty
    query = "select r.f_material, sum(r.f_qty*r.f_sign) as f_qty, sum(r.f_total*r.f_sign) as f_total from r_body r "
                "left join r_docs d on d.f_id=r.f_doc "
                "where d.f_state=1 and d.f_date between :date1 and :date2 and d.f_type in (3) "
                "group by 1";
    DatabaseResult drtm;
    fDbBind[":date2"]  = ui->deDate->date();
    fDbBind[":date1"] = ui->deLastDate->date();
    drtm.select(fDb, query, fDbBind);

    for (int j = 0; j < ui->tblData->rowCount(); j++) {
        bool f = false;
        for (QMap<int, A>::const_iterator it = goodsList.begin(); it != goodsList.end(); it++) {
            if (it.key() == ui->tblData->toInt(j, 1)) {
                f = true;
                if (it.value().qty > 0.001) {
                    price[it.key()] =  it.value().total / it.value().qty;
                } else {
                    price[it.key()] =  0;
                }
                qty[it.key()] = it.value().qty - ui->tblData->lineEdit(j, 3)->asDouble();
                for (int k = 0; k < drtm.rowCount(); k++) {
                    if (it.key() == drtm.value(k, "f_material").toInt()) {
                        qty[it.key()] = qty[it.key()] + drtm.value(k, "f_qty").toDouble();
                        break;
                    }
                }
                break;
            }
        }
        if (!f) {
            qty[ui->tblData->toInt(j, 1)] = ui->tblData->lineEdit(j, 3)->asDouble() * -1;
            price[ui->tblData->toInt(j, 1)] = 0;
        }
    }

    if (qty.count() == 0) {
        message_info(tr("Empry report"));
        return;
    }

    QString docId = uuid("ST", fAirDb);
    fDbBind[":f_id"] = docId;
    fDb.insertWithoutId("r_docs", fDbBind);
    fDbBind[":f_date"] = ui->deDate->date();
    fDbBind[":f_type"] = 3;
    fDbBind[":f_state"] = 1;
    fDbBind[":f_partner"] = 0;
    fDbBind[":f_inv"] = "";
    //fDbBind[":f_invDate"] = ui->leInvoiceNo->isEmpty() ? QVariant() : QVariant(ui->deInvoiceDate->date());
    fDbBind[":f_amount"] = 0;
    fDbBind[":f_remarks"] = "AUTO OUT";
    fDbBind[":f_op"] = WORKING_USERID;
    fDbBind[":f_fullDate"] = QDateTime::currentDateTime();
    fDbBind[":f_payment"] = 1;
    fDb.update("r_docs", fDbBind, where_id(ap(docId)));

    double total = 0;
    for (QMap<int, double>::const_iterator it = qty.begin(); it != qty.end(); it++) {
        if (it.value() == 0) {
            continue;
        }
        fDbBind[":f_id"] = 0;
        int newid = fDb.insert("r_body", fDbBind);
        fDbBind[":f_doc"] = docId;
        fDbBind[":f_store"] = ui->leStore->asInt();
        fDbBind[":f_material"] = it.key();
        fDbBind[":f_sign"] = -1;
        fDbBind[":f_qty"] = it.value();
        fDbBind[":f_price"] = price[it.key()];
        fDbBind[":f_total"] =  price[it.key()] * it.value();
        total += price[it.key()] * it.value();
        QMap<QString, QVariant> b = fDbBind;
        fDb.update("r_body", fDbBind, where_id(newid));
    }

    fDbBind[":f_amount"] = total;
    fDb.update("r_docs", fDbBind, where_id(ap(docId)));

    total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        double amount = ui->tblData->lineEdit(i, 3)->asDouble() * price[ui->tblData->toInt(i, 1)];
        total += amount;
        ui->tblData->lineEdit(i, 5)->setDouble(amount);
        ui->tblData->lineEdit(i, 4)->setDouble(price[ui->tblData->toInt(i, 1)]);
        fDbBind[":f_amount"] = amount;
        fDb.update("st_body", fDbBind, where_id(ui->tblData->toInt(i, 0)));
    }

    fDbBind[":f_amount"] = total;
    fDb.update("st_header", fDbBind, where_id(ui->leDocNum->asInt()));

    /* missing prices from &*/
    for (QMap<int, A>::const_iterator it = goodsList.begin(); it != goodsList.end(); it++) {
        if (it.value().qty > 0.001) {
            price[it.key()] =  it.value().total / it.value().qty;
        } else {
            price[it.key()] =  0;
        }
    }
    /* ------------------- */

    fDbBind[":date2"]  = ui->deDate->date();
    fDbBind[":date1"] = ui->deLastDate->date();
    fDbBind[":f_store"] = ui->leStore->asInt();
    dr.select(fDb, "select b.f_id, b.f_material from r_body b left join r_docs d on d.f_id=b.f_doc "
              "where d.f_type in (2, 3) and d.f_state=1 and d.f_date between :date1 and :date2 and f_store=:f_store ", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_id"] = dr.value(i, "f_id");
        fDbBind[":f_price"] = price[dr.value(i, "f_material").toInt()];
        fDb.select("update r_body set f_price=:f_price where f_id=:f_id", fDbBind, fDbRows);
        fDbBind[":f_id"] = dr.value(i, "f_id");
        fDb.select("update r_body set f_total=f_price*f_qty where f_id=:f_id", fDbBind, fDbRows);
    }

    fDbBind[":date2"]  = ui->deDate->date();
    fDbBind[":date1"] = ui->deLastDate->date();
    dr.select(fDb, "select b.f_doc, sum(b.f_total) as f_total from r_body b left join r_docs d on d.f_id=b.f_doc "
              "where ((d.f_type in (2) and d.f_state=1 and b.f_sign=1) or (d.f_type in (3) and d.f_state=1 and b.f_sign=-1)) and d.f_date between :date1 and :date2 group by 1", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_amount"] = dr.value(i, "f_total");
        fDb.update("r_docs", fDbBind, where_id(ap(dr.value(i, "f_doc").toString())));
    }

    message_info(tr("Saved"));
}

void WStoreEntry::on_btnPrint_clicked()
{
    PPrintStoreEntry::print(ui->leDocNum->text());
}

void WStoreEntry::on_leSearch_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        bool hidden = true;
        if (ui->tblData->item(i, 2)->text().contains(arg1, Qt::CaseInsensitive)) {
            hidden = false;
        }
        ui->tblData->setRowHidden(i, hidden);
    }
}

void WStoreEntry::on_btnExcel_clicked()
{
    int colCount = 6;
    int rowCount = ui->tblData->rowCount();
    if (colCount == 0 || rowCount == 0) {
        message_error_tr("Empty report!");
        return;
    }
    QList<int> cols;
    cols << 100 << 300 << 100 << 100 << 100 << 100;
    QStringList colName;
    colName << tr("Code") << tr("Name") << tr("Qty") << tr("Unit") << tr("Price") << tr("Amount");
    Excel e;
    for (int i = 0; i < colCount; i++) {
        e.setValue(colName[i], 2, i + 1);
        e.setColumnWidth(i + 1, cols[i] / 7);
    }
    QColor color = QColor::fromRgb(200, 200, 250);
    e.setBackground(e.address(1, 0), e.address(1, colCount - 1),
                     color.red(), color.green(), color.blue());
    e.setFontBold(e.address(0, 0), e.address(0, colCount - 1));
    e.setHorizontalAlignment(e.address(0, 0), e.address(0, colCount - 1), Excel::hCenter);

    e.setValue(ui->leDocNum->text(), 1, 1);
    e.setValue(ui->deDate->text(), 1, 2);

    for (int j = 0; j < ui->tblData->rowCount(); j++) {
        e.setValue(ui->tblData->item(j, 1)->text(), j + 3, 1);
        e.setValue(ui->tblData->item(j, 2)->text(), j + 3, 2);
        e.setValue(ui->tblData->lineEdit(j, 3)->text(), j + 3, 3);
        e.setValue(ui->tblData->item(j, 4)->text(), j + 3, 4);
        e.setValue(ui->tblData->lineEdit(j, 5)->text(), j + 3, 5);
        e.setValue(ui->tblData->lineEdit(j, 6)->text(), j + 3, 6);
    }

    e.setFontSize(e.address(0, 0), e.address(rowCount + 2, colCount), 10);
    e.show();
}
