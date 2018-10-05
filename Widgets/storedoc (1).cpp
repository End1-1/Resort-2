#include "storedoc.h"
#include "ui_storedoc.h"
#include "defstore.h"
#include "rerestdish.h"
#include "cachecashdoc.h"
#include <QInputDialog>

#define SEL_DOC_TYPE 10
#define SEL_PARTNER 2
#define SEL_DISH 3

StoreDoc::StoreDoc(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::StoreDoc)
{
    ui->setupUi(this);
    fFirstTab = ui->tabWidget->widget(0);
    fGoodsTab = ui->tabWidget->widget(1);
    fAccTab = ui->tabWidget->widget(2);

    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, SIGNAL(store(CI_RestStore*)), this, SLOT(store(CI_RestStore*)));

    fDockStore2 = new DWSelectorRestStore(this);
    fDockStore2->configure();
    fDockStore2->setSelector(ui->leStoreout);
    connect(fDockStore2, SIGNAL(store(CI_RestStore*)), this, SLOT(store2(CI_RestStore*)));

    fDockType = new DWSelectorStoreDocType(this);
    fDockType->configure();
    fDockType->setSelector(ui->leAction);
    fDockType->setDialog(this, SEL_DOC_TYPE);

    fDockPartner = new DWSelectorStorePartners(this);
    fDockPartner->configure();
    fDockPartner->setSelector(ui->lePartnerCode);
    fDockPartner->setDialog(this, SEL_PARTNER);

    fDockDish = new DWSelectorDish(this);
    fDockDish->configure();
    fDockDish->setDialog(this, SEL_DISH);

    Utils::tableSetColumnWidths(ui->tblGoods, ui->tblGoods->columnCount(),
                                     0, 100, 300, 80, 80, 80);

    ui->wAcc->docType(CASHDOC_STORE);
    ui->wAcc->setSaveVisible(false);
    ui->wAcc->cashOp(CASHOP_OUT);
}

StoreDoc::~StoreDoc()
{
    delete ui;
}

void StoreDoc::setup()
{
    setupTabTextAndIcon(tr("New store document"), ":/images/storage.png");
}

void StoreDoc::selector(int number, const QVariant &value)
{
    switch (number) {
    case SEL_DOC_TYPE: {
        CI_StoreDocType *c = value.value<CI_StoreDocType*>();
        dockResponse<CI_StoreDocType, CacheStoreDocType>(ui->leAction, c);
        prepareDoc();
        break;
    }
    case SEL_PARTNER: {
        CI_StorePartners *c = value.value<CI_StorePartners*>();
        dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, c);
        if (c) {
            ui->wAcc->partner(c->fCode.toInt());
        } else {
            ui->wAcc->partner(0);
        }
        break;
    }
    case SEL_DISH: {
        CI_Dish *c = value.value<CI_Dish*>();
        if (c) {
            newGoods(c);
        }
        break;
    }
    }
}

void StoreDoc::openStoreDocument(const QString &id)
{
    StoreDoc *s = addTab<StoreDoc>();
    if (!id.isEmpty()) {
        s->loadDoc(id);
    }
}

void StoreDoc::loadDoc(const QString &id)
{
    ui->leDocNumber->setText(id);
    fDbBind[":f_id"] = id;
    DatabaseResult dh;
    dh.select(fDb, "select * from r_docs where f_id=:f_id", fDbBind);
    if (dh.rowCount() == 0) {
        message_error(tr("Invalid document number"));
        return;
    }
    int state = dh.value("f_state").toInt();
    CI_StoreDocType *ds = CacheStoreDocType::instance()->get(dh.value("f_type").toString());
    selector(SEL_DOC_TYPE, qVariantFromValue(ds));
    CI_StorePartners *sp = CacheStorePartners::instance()->get(dh.value("f_partner").toString());
    dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, sp);
    prepareDoc();
    ui->deDate->setDate(dh.value("f_date").toDate());
    ui->leInvoiceNo->setText(dh.value("f_inv").toString());
    ui->deInvoiceDate->setDate(dh.value("f_invDate").toDate());
    ui->leComments->setText(dh.value("f_remarks").toString());

    fDbBind[":f_doc"] = ui->leDocNumber->text();
    dh.select(fDb, "select distinct(f_store) as f_store, f_sign from r_body where f_doc=:f_doc", fDbBind);
    if (dh.rowCount() == 1) {
        CI_RestStore *rs = CacheRestStore::instance()->get(dh.value("f_store").toString());
        store(rs);
    } else {
        CI_RestStore *rs = CacheRestStore::instance()->get(dh.value(0, "f_store").toString());
        if (dh.value(0, "f_sign").toInt() > 0) {
            store(rs);
        } else {
            store2(rs);
        }
        rs = CacheRestStore::instance()->get(dh.value(1, "f_store").toString());
        if (dh.value(1, "f_sign").toInt() > 0) {
            store(rs);
        } else {
            store2(rs);
        }
    }
    fDbBind[":f_doc"] = ui->leDocNumber->text();
    QString add;
    if (ui->leAction->fHiddenText.toInt() == STORE_DOC_MOVE) {
        add = "and b.f_sign=1";
    }
    dh.select(fDb, "select b.f_id, b.f_material, m.f_en, b.f_qty, b.f_price, b.f_total "
              "from r_body b "
              "left join r_dish m on m.f_id=b.f_material "
              "where b.f_doc=:f_doc " + add, fDbBind);
    for (int i = 0; i < dh.rowCount(); i++) {
        CI_Dish *d = CacheDish::instance()->get(dh.value(i, "f_material").toString());
        if (!d) {
            message_error(tr("Program error. Please, contact with developer. Message: CI_Dish==0, loadDoc"));
            return;
        }
        newGoods(d);
        ui->tblGoods->setItemWithValue(i, 0, dh.value(i, "f_id"));
        ui->tblGoods->lineEdit(i, 3)->setDouble(dh.value(i, "f_qty").toDouble());
        ui->tblGoods->lineEdit(i, 4)->setDouble(dh.value(i, "f_price").toDouble());
        ui->tblGoods->lineEdit(i, 5)->setDouble(dh.value(i, "f_total").toDouble());
    }
    countTotal();
    if (state == 1) {
        ui->wCommon->setEnabled(false);
        for (int i = 0; i < ui->tabWidget->count(); i++) {
            ui->tabWidget->widget(i)->setEnabled(false);
        }
    }
}

void StoreDoc::prepareDoc()
{
    switch (ui->leAction->fHiddenText.toInt()) {
    case STORE_DOC_IN:
        if (ui->tabWidget->count() == 1) {
            ui->tabWidget->insertTab(0, fFirstTab, QIcon(), tr("Common"));
            ui->tabWidget->insertTab(2, fAccTab, QIcon(), tr("Accounts"));
        }
        ui->leStoreout->setVisible(false);
        ui->lbStoreout->setVisible(false);
        break;
    case STORE_DOC_MOVE:
        if (ui->tabWidget->count() > 1) {
            ui->tabWidget->removeTab(0);
            ui->tabWidget->removeTab(1);
        }
        ui->tabCommon->setVisible(false);
        ui->leStoreout->setVisible(true);
        ui->lbStoreout->setVisible(true);
        ui->wAcc->disable();
        break;
    case STORE_DOC_OUT:
        if (ui->tabWidget->count() > 1) {
            ui->tabWidget->removeTab(0);
            ui->tabWidget->removeTab(1);
        }
        ui->tabCommon->setVisible(false);
        ui->leStoreout->setVisible(false);
        ui->lbStoreout->setVisible(false);
        ui->wAcc->disable();
        break;
    default:
        break;
    }
}

void StoreDoc::countTotal()
{
    float total = 0;
    for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
        total += ui->tblGoods->lineEdit(i, 5)->asDouble();
    }
    ui->leTotal->setDouble(total);
}

void StoreDoc::saveDoc(int docState)
{
    QString errors;
    if (ui->leAction->fHiddenText.toInt() == 1) {
        QString err;
        if (!ui->wAcc->isCorrent(err)) {
            errors += err;
        }
    }
    if (ui->leAction->fHiddenText.toInt() == 0) {
        errors += tr("Action must be selected.") + "<br>";
    }
    switch (ui->leAction->fHiddenText.toInt()) {
    case STORE_DOC_IN:
        if (ui->leStore->fHiddenText.toInt() == 0) {
            errors += tr("Input store must be selected.") + "<br>";
        }
        break;
    case STORE_DOC_OUT:
        if (ui->leStore->fHiddenText.toInt() == 0) {
            errors += tr("Output store must be selected.") + "<br>";
        }
        break;
    case STORE_DOC_MOVE:
        if (ui->leStore->fHiddenText.toInt() == 0) {
            errors += tr("Input store must be selected.") + "<br>";
        }
        if (ui->leStoreout->fHiddenText.toInt() == 0) {
            errors += tr("Output store must be selected.") + "<br>";
        }
        if (ui->leStore->fHiddenText.toInt() > 0) {
            if (ui->leStore->fHiddenText.toInt() == ui->leStoreout->fHiddenText.toInt()) {
                errors += tr("Input and output storages cannot be same.");
            }
        }
        break;
    }
    for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
        if (ui->tblGoods->lineEdit(i, 3)->asDouble() < 0.001) {
            errors += tr("Check the quantity of goods.") + "<br>";
            break;
        }
    }
    if (ui->tblGoods->rowCount() == 0) {
        errors += tr("Goods list cannot be empty.") + "<br>";
    }
    if (!errors.isEmpty()) {
        message_error(errors);
        return;
    }
    bool isNew = false;
    if (ui->leDocNumber->isEmpty()) {
        ui->leDocNumber->setText(intid("ST", fDb));
        isNew = true;
    }
    fDb.fDb.transaction();
    if (isNew) {
        fDbBind[":f_id"] = ui->leDocNumber->text();
        fDb.insertWithoutId("r_docs", fDbBind);
    }
    fDbBind[":f_date"] = ui->deDate->date();
    fDbBind[":f_type"] = ui->leAction->fHiddenText.toInt();
    fDbBind[":f_state"] = docState;
    fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
    fDbBind[":f_inv"] = ui->leInvoiceNo->text();
    fDbBind[":f_invDate"] = ui->leInvoiceNo->isEmpty() ? QVariant() : QVariant(ui->deInvoiceDate->date());
    fDbBind[":f_amount"] = ui->leTotal->asDouble();
    fDbBind[":f_remarks"] = ui->leComments->text();
    fDbBind[":f_op"] = WORKING_USERID;
    fDbBind[":f_fullDate"] = QDateTime::currentDateTime();
    fDbBind[":f_payment"] = ui->cbPayment->currentData();
    fDb.update("r_docs", fDbBind, where_id(ap(ui->leDocNumber->text())));
    for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
        if (ui->tblGoods->toInt(i, 0) == 0) {
            fDbBind[":f_id"] = 0;
            int newid = fDb.insert("r_body", fDbBind);
            ui->tblGoods->setItemWithValue(i, 0, newid);
        }
        fDbBind[":f_doc"] = ui->leDocNumber->text();
        fDbBind[":f_store"] = ui->leStore->fHiddenText.toInt();
        fDbBind[":f_material"] = ui->tblGoods->toInt(i, 1);
        fDbBind[":f_sign"] = ui->leAction->fHiddenText.toInt() == STORE_DOC_OUT ? -1 : 1;
        fDbBind[":f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
        fDbBind[":f_price"] = ui->tblGoods->lineEdit(i, 4)->asDouble();
        fDbBind[":f_total"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
        QMap<QString, QVariant> b = fDbBind;
        fDb.update("r_body", fDbBind, where_id(ui->tblGoods->toInt(i, 0)));
        if (ui->leAction->fHiddenText.toInt() == STORE_DOC_MOVE) {
            b[":f_sign"] = -1;
            b[":f_store"] = ui->leStoreout->fHiddenText.toInt();
            fDb.insert("r_body", b);
        }
    }
    if (ui->leAction->fHiddenText.toInt() == 1) {
        if (docState == 1) {
            ui->wAcc->docType(1);
            ui->wAcc->docNum(ui->leDocNumber->asInt());
            ui->wAcc->partner(ui->lePartnerCode->asInt());
            ui->wAcc->save();
        } else {
            fDbBind[":f_docType"] = CASHDOC_STORE;
            fDbBind[":f_docNum"] = ui->leDocNumber->text();
            fDb.select("delete from c_cash where f_docType=:f_docType and f_docNum=:f_docNum", fDbBind, fDbRows);
        }
    }
    fDb.fDb.commit();
    bool v = docState == 0;
    ui->wCommon->setEnabled(v);
    for (int i = 0; i < ui->tabWidget->count(); i++) {
        ui->tabWidget->widget(i)->setEnabled(v);
    }
    if (docState == 1 && ui->leAction->fHiddenText.toInt() == STORE_DOC_IN) {
        for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
            fDbBind[":f_lastPrice"] = ui->tblGoods->lineEdit(i, 4)->asDouble();
            fDb.update("r_dish", fDbBind, where_id(ui->tblGoods->toInt(i, 1)));
        }
    }
    if (docState == 1) {
        message_info(tr("Saved"));
    } else {
        message_info(tr("Draft saved"));
    }
}

void StoreDoc::qtyChange(const QString &arg1)
{
    Q_UNUSED(arg1)
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    int row, col;
    if (!ui->tblGoods->findWidgetCell(l, row, col)) {
        return;
    }
    ui->tblGoods->lineEdit(row, 5)->setDouble(l->asDouble() * ui->tblGoods->lineEdit(row, 4)->asDouble());
    countTotal();
}

void StoreDoc::priceChange(const QString &arg1)
{
    Q_UNUSED(arg1)
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    int row, col;
    if (!ui->tblGoods->findWidgetCell(l, row, col)) {
        return;
    }
    ui->tblGoods->lineEdit(row, 5)->setDouble(l->asDouble() * ui->tblGoods->lineEdit(row, 3)->asDouble());
    countTotal();
}

void StoreDoc::totalChange(const QString &arg1)
{
    Q_UNUSED(arg1)
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    int row, col;
    if (!ui->tblGoods->findWidgetCell(l, row, col)) {
        return;
    }
    if (ui->tblGoods->lineEdit(row, 3)->asDouble() < 0.001) {
        return;
    }
    ui->tblGoods->lineEdit(row, 4)->setDouble(l->asDouble() / ui->tblGoods->lineEdit(row, 3)->asDouble());
    countTotal();
}

void StoreDoc::store(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);
}

void StoreDoc::store2(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStoreout, c);
}

void StoreDoc::docType(CI_StoreDocType *c)
{
    dockResponse<CI_StoreDocType, CacheStoreDocType>(ui->leAction, c);
}

void StoreDoc::on_btnSaveDoc_clicked()
{
    saveDoc(1);
}

void StoreDoc::on_btnNewMaterial_clicked()
{
    QList<QVariant> values;
    RERestDish *d = new RERestDish(values, this);
    if (d->exec() == QDialog::Accepted) {
        CI_Dish *c = CacheDish::instance()->get(values.at(0).toString());
        newGoods(c);
    }
}

void StoreDoc::newGoods(CI_Dish *c)
{
    if (!c) {
        return;
    }
    int row = ui->tblGoods->rowCount();
    ui->tblGoods->setRowCount(row + 1);
    ui->tblGoods->setItemWithValue(row, 0, "");
    ui->tblGoods->setItemWithValue(row, 1, c->fCode);
    ui->tblGoods->setItemWithValue(row, 2, c->fName);
    EQLineEdit *l = ui->tblGoods->addLineEdit(row, 3, false);
    l->setValidator(new QDoubleValidator(0, 10000, 4));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(qtyChange(QString)));
    l = ui->tblGoods->addLineEdit(row, 4, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(priceChange(QString)));
    l = ui->tblGoods->addLineEdit(row, 5, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(totalChange(QString)));
    ui->tblGoods->lineEdit(row, 3)->setFocus();
}

void StoreDoc::on_btnAddMaterial_clicked()
{
    fDockDish->show();
}

void StoreDoc::on_btnDraftDoc_clicked()
{
    saveDoc(0);
}

void StoreDoc::on_btnRemoveDoc_clicked()
{
    if (message_confirm_tr("Confirm to delete document") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_doc"] = ui->leDocNumber->text();
    fDb.select("delete from r_body where f_doc=:f_doc", fDbBind, fDbRows);
    fDbBind[":f_id"] = ui->leDocNumber->text();
    fDb.select("delete from r_docs where f_id=:f_id", fDbBind, fDbRows);
    fDbBind[":f_docType"] = CASHDOC_STORE;
    fDbBind[":f_docNum"] = ui->leDocNumber->text();
    fDb.select("delete from c_cash where f_docType=:f_docType and f_docNum=:f_docNum", fDbBind, fDbRows);
    fMainWindow->fTab->removeTab(fMainWindow->fTab->indexOf(this));
    close();
}

void StoreDoc::on_btnAddMaterialByScan_clicked()
{
    bool ok = false;
    QString code;
    code = QInputDialog::getText(this, tr("Scancode"), "", QLineEdit::Normal, code, &ok);
    if (!ok) {
        return;
    }
    if (code.isEmpty()) {
        return;
    }
    fDbBind[":f_code"] = code;
    DatabaseResult dr;
    dr.select(fDb, "select f_dish from r_dish_scancode where f_code=:f_code", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Code not found"));
        return;
    }
    QStringList list;
    list << dr.value("f_dish").toString();
    fDockDish->setFilterList(list);
    fDockDish->show();
}

void StoreDoc::on_btnRemoveMaterial_clicked()
{
    QModelIndexList ml = ui->tblGoods->selectionModel()->selectedRows();
    QSet<int> rows;
    foreach (QModelIndex m, ml) {
        rows << m.row();
    }
    QList<int> r = rows.toList();
    for (int i = r.count() - 1; i > -1; i--) {
        ui->tblGoods->removeRow(r.at(i));
    };
    countTotal();
}

void StoreDoc::on_deDate_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->wAcc->date(ui->deDate->date());
}

void StoreDoc::on_leTotal_textChanged(const QString &arg1)
{
    ui->wAcc->amount(arg1.toDouble());
}

void StoreDoc::on_leComments_textChanged(const QString &arg1)
{
    ui->wAcc->comment(arg1);
}
