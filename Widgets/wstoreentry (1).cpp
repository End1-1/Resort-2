#include "wstoreentry.h"
#include "ui_wstoreentry.h"

#define SEL_DISH 1

WStoreEntry::WStoreEntry(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WStoreEntry)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                     0, 0, 300, 80, 80, 80);

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
    fDbBind[":f_doc"] = doc;
    dr.select(fDb, "select st.f_goods, g.f_en, st.f_qty, st.f_amount from st_body st "
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
            ui->tblData->lineEdit(i, 4)->setDouble(dr.value(i, "f_total").toDouble() / dr.value(i, "f_qty").toDouble());
        } else {
            ui->tblData->lineEdit(i, 4)->setDouble(0);
        }
        ui->tblData->lineEdit(i, 5)->setDouble(dr.value(i, "f_total").toDouble());
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
    l = ui->tblData->addLineEdit(row, 4, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(priceChange(QString)));
    l = ui->tblData->addLineEdit(row, 5, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(totalChange(QString)));
    ui->tblData->lineEdit(row, 3)->setFocus();
}

void WStoreEntry::countTotal()
{
    float total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        total += ui->tblData->lineEdit(i, 5)->asDouble();
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
    ui->tblData->lineEdit(row, 5)->setDouble(l->asDouble() * ui->tblData->lineEdit(row, 4)->asDouble());
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
    ui->tblData->lineEdit(row, 5)->setDouble(l->asDouble() * ui->tblData->lineEdit(row, 3)->asDouble());
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
    ui->tblData->lineEdit(row, 4)->setDouble(l->asDouble() / ui->tblData->lineEdit(row, 3)->asDouble());
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
        ui->tblData->removeRow(i);
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
        fDb.insert("st_body", fDbBind);
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
    if (ui->leStore->asInt() == 0) {
        message_error(tr("Store must be defined"));
        return;
    }
    QString query = "select f_id, f_en from r_dish ";
    DatabaseResult dr;
    dr.select(fDb, query, fDbBind);

    query = "select r.f_material, sum(r.f_qty) as f_qty, sum(r.f_total) as f_total from r_body r "
            "left join r_docs d on d.f_id=r.f_doc "
            "where d.f_state=1 and d.f_date between :date1 and :date2 and d.f_type=1 "
            "group by 1";
    DatabaseResult drt;
    fDbBind[":date2"]  = ui->deDate->date();
    fDbBind[":date1"] = ui->deLastDate->date().addDays(-1);
    drt.select(fDb, query, fDbBind);

    QMap<int, double> qty;
    QMap<int, double> price;

    for (int j = 0; j < ui->tblData->rowCount(); j++) {
        for (int i = 0; i < drt.rowCount(); i++) {
            if (drt.value(i, "f_material").toInt() == ui->tblData->toInt(j, 1)) {
                qty[drt.value(i, "f_material").toInt()] = drt.value(i, "f_qty").toDouble() - ui->tblData->lineEdit(j, 3)->asDouble();
                price[drt.value(i, "f_material").toInt()] =  drt.value(i, "f_total").toDouble() / drt.value(i, "f_qty").toDouble();
                break;
            }
        }
    }

    if (qty.count() == 0) {
        message_info(tr("Empry report"));
        return;
    }

    QString docId = intid("ST", fDb);
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

    for (QMap<int, double>::const_iterator it = qty.begin(); it != qty.end(); it++) {

        fDbBind[":f_id"] = 0;
        int newid = fDb.insert("r_body", fDbBind);
        fDbBind[":f_doc"] = docId;
        fDbBind[":f_store"] = ui->leStore->asInt();
        fDbBind[":f_material"] = it.key();
        fDbBind[":f_sign"] = -1;
        fDbBind[":f_qty"] = it.value();
        fDbBind[":f_price"] = price[it.key()];
        fDbBind[":f_total"] =  price[it.key()] * it.value();
        QMap<QString, QVariant> b = fDbBind;
        fDb.update("r_body", fDbBind, where_id(newid));
    }

    message_info(tr("Saved"));
}
