#include "wstoreentry.h"
#include "excel.h"
#include "ui_wstoreentry.h"
#include "defstore.h"
#include "pprintstoreentry.h"
#include "storedoc.h"
#include "database2.h"

#define SEL_DISH 1

struct goods {
    int id;
    double qty;
    double price;
    goods() {
        id = 0;
        qty = 0.0;
        price = 0.0;
    }
};

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
    storeQty(0);
}

void WStoreEntry::newGoods(CI_Dish *c)
{
    if (!c) {
        return;
    }
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->itemValue(i, 1).toInt() == c->fCode) {
            ui->tblData->setCurrentCell(i, 4);
            return;
        }
    }
    int row = ui->tblData->rowCount();
    ui->tblData->setRowCount(row + 1);
    ui->tblData->setItemWithValue(row, 0, "");
    ui->tblData->setItemWithValue(row, 1, c->fCode);
    ui->tblData->setItemWithValue(row, 2, c->fName);
    EQLineEdit *l = ui->tblData->addLineEdit(row, 3, false);
    l->setValidator(new QDoubleValidator(0, 10000, 4));
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(qtyChange(QString)));
    ui->tblData->setItemWithValue(row, 4, c->fUnitName);
    l = ui->tblData->addLineEdit(row, 5, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(priceChange(QString)));
    l = ui->tblData->addLineEdit(row, 6, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(totalChange(QString)));
    ui->tblData->lineEdit(row, 3)->setFocus();
    storeQty(c->fCode.toInt());
}

void WStoreEntry::countTotal()
{
    float total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        total += ui->tblData->lineEdit(i, 6)->asDouble();
    }
    ui->leAmount->setDouble(total);
}

void WStoreEntry::storeQty(int goods)
{
    if (ui->leStore->asInt() == 0) {
        return;
    }
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_date"] = ui->deDate->date();
    db2[":f_store"] = ui->leStore->asInt();
    QString where ;
    if (goods > 0){
        where += QString("and r.f_material=%1 ").arg(goods);
    }
    db2.exec(QString("SELECT f_material, SUM(f_sign*f_qty) as f_qty "
            "FROM r_body r "
            "LEFT JOIN r_docs d ON d.f_id=r.f_doc "
            "WHERE d.f_date<=:f_date AND r.f_store=:f_store %1 "
            "GROUP BY 1 "
            "having SUM(f_sign*f_qty)<>0").arg(where));
    while (db2.next()) {
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            if (db2.integer("f_material") == ui->tblData->item(i, 1)->data(Qt::EditRole).toInt()) {
                ui->tblData->setItemWithValue(i, 7, db2.doubleValue("f_qty"));
            }
        }
    }
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
    // QMap<int, QString> colFilter;
    // colFilter[3] = ui->leStore->text();
    // fDockDish->setFilterColumn(colFilter);
    fDockDish->on_lineEdit_textEdited("");
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
        fDbBind[":f_amount"] = ui->tblData->lineEdit(i, 6)->asDouble();
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

    fDbBind[":f_date"] = ui->deDate->date();
    fDbBind[":f_store"] = ui->leStore->asInt();
    fDb.select("select b.f_goods, sum(b.f_qty*b.f_sign) as f_qty, "
            "abs(sum(b.f_price*b.f_qty)/sum(b.f_qty)) as f_price "
            "from r_store_acc b "
            "left join r_docs bd on bd.f_id=b.f_doc "
            "where bd.f_date<=:f_date and bd.f_state=1 "
            "and b.f_store=:f_store "
            "group by 1", fDbBind, fDbRows);

    QList<goods> over;
    QList<goods> miss;
    // if all fDbRows exists in tblData
    for (int i = 0; i < fDbRows.count(); i++) {
        for (int j = 0; j < ui->tblData->rowCount(); j++) {
            if (fDbRows.at(i).at(0).toInt() == ui->tblData->itemValue(j, 1).toInt()) {
                if (fDbRows.at(i).at(1).toDouble() > ui->tblData->lineEdit(j, 3)->asDouble()) {
                    //korust
                    goods g;
                    g.id = fDbRows.at(i).at(0).toInt();
                    g.qty = fDbRows.at(i).at(1).toDouble() - ui->tblData->lineEdit(j, 3)->asDouble();
                    g.price = fDbRows.at(i).at(2).toDouble();
                    miss.append(g);
                } else if (fDbRows.at(i).at(1).toDouble() < ui->tblData->lineEdit(j, 3)->asDouble()) {
                    // avel
                    goods g;
                    g.id = fDbRows.at(i).at(0).toInt();
                    g.qty = ui->tblData->lineEdit(j, 3)->asDouble() - fDbRows.at(i).at(1).toDouble();
                    g.price = fDbRows.at(i).at(2).toDouble();
                    over.append(g);
                }
                continue;
            }
        }
    }
    //check for missed in fDbRows
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        bool found = false;
        for (int j = 0; j < fDbRows.count(); j++) {
            if (fDbRows.at(j).at(0).toInt() == ui->tblData->itemValue(i, 1).toInt()) {
                found = true;
                continue;
            }
        }
        if (!found) {
            goods g;
            g.id = ui->tblData->itemValue(i, 1).toInt();
            g.qty = ui->tblData->lineEdit(i, 3)->asDouble();
            g.price = 0;
            over.append(g);
        }
    }

    if (miss.count() > 0) {
        fDbBind[":f_date"] = ui->deDate->date();
        fDbBind[":f_type"] = STORE_DOC_OUT;
        fDbBind[":f_state"] = 0;
        fDbBind[":f_partner"] = 0;
        fDbBind[":f_inv"] = "";
        fDbBind[":f_invDate"] = QVariant();
        fDbBind[":f_amount"] = 0;
        fDbBind[":f_remarks"] = QString("%1").arg(tr("Autocorrection, output"));
        fDbBind[":f_op"] = 1;
        fDbBind[":f_fullDate"] = QDateTime::currentDateTime();
        fDbBind[":f_payment"] = 1;
        fDbBind[":f_rest"] = 0;
        int docid = fDb.insert("r_docs", fDbBind);

        for (goods &g: miss){
            fDbBind[":f_doc"] = docid;
            fDbBind[":f_store"] =  ui->leStore->asInt();
            fDbBind[":f_material"] = g.id;
            fDbBind[":f_sign"] = -1;
            fDbBind[":f_qty"] = g.qty;
            fDbBind[":f_price"] = g.price;
            fDbBind[":f_total"] = g.qty * g.price;
            fDb.insert("r_body", fDbBind);
        }
        StoreDoc *d = addTab<StoreDoc>();
        d->loadDoc(docid);
    }

    if (over.count() > 0) {
        fDbBind[":f_date"] = ui->deDate->date();
        fDbBind[":f_type"] = STORE_DOC_IN;
        fDbBind[":f_state"] = 0;
        fDbBind[":f_partner"] = 0;
        fDbBind[":f_inv"] = "";
        fDbBind[":f_invDate"] = QVariant();
        fDbBind[":f_amount"] = 0;
        fDbBind[":f_remarks"] = QString("%1").arg(tr("Autocorrection, input"));
        fDbBind[":f_op"] = 1;
        fDbBind[":f_fullDate"] = QDateTime::currentDateTime();
        fDbBind[":f_payment"] = 1;
        fDbBind[":f_rest"] = 0;
        int docid = fDb.insert("r_docs", fDbBind);

        for (goods &g: over){
            fDbBind[":f_doc"] = docid;
            fDbBind[":f_store"] =  ui->leStore->asInt();
            fDbBind[":f_material"] = g.id;
            fDbBind[":f_sign"] = 1;
            fDbBind[":f_qty"] = g.qty;
            fDbBind[":f_price"] = g.price;
            fDbBind[":f_total"] = g.qty * g.price;
            fDb.insert("r_body", fDbBind);
        }
        StoreDoc *d = addTab<StoreDoc>();
        d->loadDoc(docid);
    }
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

void WStoreEntry::on_deDate_editingFinished()
{
    storeQty(0);
}
