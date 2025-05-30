#include "storedoc.h"
#include "ui_storedoc.h"
#include "defstore.h"
#include "rerestdish.h"
#include "cachecashdoc.h"
#include "excel.h"
#include "storeoutput.h"
#include "pprintstoredoc.h"
#include "database2.h"
#include "dlggetidname.h"
#include <QInputDialog>
#include <QFileDialog>
#include <qxml.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QXmlStreamReader>

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

    connect(ui->leStore, &EQLineEdit::customButtonClicked, this, &StoreDoc::editStoreClicked);

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

    ui->btnSaveDoc->setEnabled(check_permission(pr_edit_store_doc));
    ui->btnRemoveDoc->setEnabled(check_permission(pr_edit_store_doc));
    ui->btnDraftDoc->setEnabled(check_permission(pr_edit_store_doc));
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

void StoreDoc::openStoreDocument(int id)
{
    StoreDoc *s = addTab<StoreDoc>();
    if (id > 0) {
        s->loadDoc(id);
    }
}

void StoreDoc::loadDoc(int id)
{
    ui->leDocNumber->setInt(id);
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
    QString add;
    dh.select(fDb, "select b.f_id, b.f_material, m.f_en, b.f_qty, b.f_price, b.f_total, "
              "u.f_name as f_unitName, b.f_sign, b.f_store, b.f_vat "
              "from r_body b "
              "left join r_dish m on m.f_id=b.f_material "
              "left join r_unit u on u.f_id=m.f_unit "
              "where b.f_doc=:f_doc " + add, fDbBind);
    for (int i = 0; i < dh.rowCount(); i++) {
        CI_Dish *d = CacheDish::instance()->get(dh.value(i, "f_material").toString());
        if (!d) {
            message_error(tr("Program error. Please, contact with developer. Message: CI_Dish==0, loadDoc"));
            continue;
        }
        newGoods(d);
        ui->tblGoods->setItemWithValue(i, 0, dh.value(i, "f_id"));
        ui->tblGoods->lineEdit(i, 3)->setDouble(dh.value(i, "f_qty").toDouble());
        ui->tblGoods->setItemWithValue(i, 4, dh.value(i, "f_unitName"));
        ui->tblGoods->lineEdit(i, 5)->setDouble(dh.value(i, "f_price").toDouble());
        ui->tblGoods->lineEdit(i, 6)->setDouble(dh.value(i, "f_total").toDouble());
        ui->tblGoods->lineEdit(i, 7)->setDouble(dh.value(i, "f_vat").toDouble());
        ui->tblGoods->lineEdit(i, 8)->setDouble(dh.value(i, "f_qty").toDouble() * dh.value(i, "f_vat").toDouble());
        if (dh.value(i, "f_sign").toInt() == 1) {
            CI_RestStore *rs = CacheRestStore::instance()->get(dh.value(i, "f_store").toString());
            store(rs);
        } else {
            CI_RestStore *rs = CacheRestStore::instance()->get(dh.value(i, "f_store").toString());
            if (ui->leAction->fHiddenText.toInt() == STORE_DOC_MOVE) {
                continue;
            }

            store2(rs);
        }
    }
    countTotal();
    bool v = state == 0;
    ui->wCommon->setEnabled(v);
    for (int j = 0; j < ui->tblGoods->rowCount(); j++) {
        ui->tblGoods->lineEdit(j, 3)->setReadOnly(!v);
        ui->tblGoods->lineEdit(j, 5)->setReadOnly(!v);
        ui->tblGoods->lineEdit(j, 6)->setReadOnly(!v);
    }
    ui->wGoodsOps->setEnabled(v);

}

void StoreDoc::copyDoc(int id)
{
    loadDoc(id);
    ui->leDocNumber->clear();
    for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
        ui->tblGoods->setItemWithValue(i, 0, 0);
    }
    ui->wCommon->setEnabled(true);
    for (int i = 0; i < ui->tabWidget->count(); i++) {
        ui->tabWidget->widget(i)->setEnabled(true);
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
        ui->leStore->setVisible(false);
        ui->lbStorein->setVisible(false);
        ui->wAcc->disable();
        break;
    default:
        break;
    }
}

void StoreDoc::countTotal()
{
    float total = 0, totalVat = 0;
    for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
        total += ui->tblGoods->lineEdit(i, 6)->asDouble();
        totalVat += ui->tblGoods->lineEdit(i, 8)->asDouble();
    }
    ui->leTotal->setDouble(total);
    ui->leTotalVat->setDouble(totalVat);
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
    int store = 0;
    switch (ui->leAction->fHiddenText.toInt()) {
    case STORE_DOC_IN:
        store = ui->leStore->fHiddenText.toInt();
        if (ui->leStore->fHiddenText.toInt() == 0) {
            errors += tr("Input store must be selected.") + "<br>";
        }
        break;
    case STORE_DOC_OUT:
        store = ui->leStoreout->fHiddenText.toInt();
        if (ui->leStoreout->fHiddenText.toInt() == 0) {
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
    if (docState == 1) {
        for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
            if (ui->tblGoods->lineEdit(i, 3)->asDouble() < 0.001) {
                errors += tr("Check the quantity of goods.") + "<br>";
                break;
            }
        }
    }
    if (ui->tblGoods->rowCount() == 0) {
        errors += tr("Goods list cannot be empty.") + "<br>";
    }
    if (!errors.isEmpty()) {
        message_error(errors);
        return;
    }

    fDb.fDb.transaction();

    QJsonObject jdoc;
    QJsonArray jgoods;

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

    jdoc["f_date"] = ui->deDate->date().toString("yyyy-MM-dd");
    jdoc["f_type"] = ui->leAction->fHiddenText.toInt();
    jdoc["f_state"] = docState;
    jdoc["f_partner"] = ui->lePartnerCode->asInt();
    jdoc["f_inv"] = ui->leInvoiceNo->text();
    jdoc["f_invDate"] = ui->deInvoiceDate->date().toString("yyyy-MM-dd");
    jdoc["f_amount"] = ui->leTotal->asDouble();
    jdoc["f_remarks"] = ui->leComments->text();
    jdoc["f_op"] = WORKING_USERID;
    jdoc["f_fulldate"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    jdoc["f_payment"] = ui->cbPayment->currentData().toInt();
    if (ui->leDocNumber->asInt() == 0) {
        ui->leDocNumber->setInt(fDb.insert("r_docs", fDbBind));
        if (ui->leDocNumber->asInt() == 0) {
            fDb.fDb.rollback();
            message_error(fDb.fLastError);
            return;
        }
    } else if (!fDb.update("r_docs", fDbBind, where_id(ap(ui->leDocNumber->text())))) {
        fDb.fDb.rollback();
        message_error(fDb.fLastError);
        return;
    }

    for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
        QJsonObject jg;
        if (ui->leAction->fHiddenText.toInt() == STORE_DOC_OUT ||
                ui->leAction->fHiddenText.toInt() == STORE_DOC_IN) {
            fDbBind[":f_doc"] = ui->leDocNumber->text();
            fDbBind[":f_store"] =  store;
            fDbBind[":f_material"] = ui->tblGoods->toInt(i, 1);
            fDbBind[":f_sign"] = ui->leAction->fHiddenText.toInt() == STORE_DOC_OUT ? -1 : 1;
            fDbBind[":f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
            fDbBind[":f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            fDbBind[":f_total"] = ui->tblGoods->lineEdit(i, 6)->asDouble();
            fDbBind[":f_vat"] = ui->tblGoods->lineEdit(i, 7)->asDouble();

            jg["f_doc"] = ui->leDocNumber->text();
            jg["f_store"] = store;
            jg["f_material"] = ui->tblGoods->toInt(i, 1);
            jg["f_sign"] = ui->leAction->fHiddenText.toInt() == STORE_DOC_OUT ? -1 : 1;
            jg["f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
            jg["f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            jg["f_total"] = ui->tblGoods->lineEdit(i, 6)->asDouble();
            jg["f_vat"] = ui->tblGoods->lineEdit(i, 7)->asDouble();
            jgoods.append(jg);
            if (ui->tblGoods->toInt(i, 0) == 0) {
                int newid = fDb.insert("r_body", fDbBind);
                if (newid < 0) {
                    fDb.fDb.rollback();
                    message_error(fDb.fLastError);
                    return;
                }
                ui->tblGoods->setItemWithValue(i, 0, newid);
            } else if (!fDb.update("r_body", fDbBind, where_id(ui->tblGoods->toInt(i, 0)))) {
                fDb.fDb.rollback();
                message_error(fDb.fLastError);
                return;
            }
            if (ui->leAction->fHiddenText.toInt() == STORE_DOC_IN) {
             fDbBind[":f_lastprice"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
             fDb.update("r_dish", fDbBind, where_id(ui->tblGoods->toInt(i, 1)));
            }
        } else {
            fDbBind[":f_doc"] = ui->leDocNumber->text();
            fDbBind[":f_store"] =  ui->leStore->fHiddenText.toInt();
            fDbBind[":f_material"] = ui->tblGoods->toInt(i, 1);
            fDbBind[":f_sign"] = 1;
            fDbBind[":f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
            fDbBind[":f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            fDbBind[":f_total"] = ui->tblGoods->lineEdit(i, 6)->asDouble();
            fDbBind[":f_vat"] = ui->tblGoods->lineEdit(i, 7)->asDouble();

            jg["f_doc"] = ui->leDocNumber->text();
            jg["f_store"] = store;
            jg["f_material"] = ui->tblGoods->toInt(i, 1);
            jg["f_sign"] = 1;
            jg["f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
            jg["f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            jg["f_total"] = ui->tblGoods->lineEdit(i, 6)->asDouble();
            jg["f_vat"] = ui->tblGoods->lineEdit(i, 7)->asDouble();
            jgoods.append(jg);
            if (ui->tblGoods->toInt(i, 0) == 0) {
                int newid = fDb.insert("r_body", fDbBind);
                if (newid < 0) {
                    fDb.fDb.rollback();
                    message_error(fDb.fLastError);
                    return;
                }
                //ui->tblGoods->setItemWithValue(i, 0, newid);
            } else if (!fDb.update("r_body", fDbBind, where_id(ui->tblGoods->toInt(i, 0)))) {
                fDb.fDb.rollback();
                message_error(fDb.fLastError);
                return;
            }

            fDbBind[":f_doc"] = ui->leDocNumber->text();
            fDbBind[":f_store"] = ui->leStoreout->fHiddenText.toInt();
            fDbBind[":f_material"] = ui->tblGoods->toInt(i, 1);
            fDbBind[":f_sign"] = -1;
            fDbBind[":f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
            fDbBind[":f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            fDbBind[":f_total"] = ui->tblGoods->lineEdit(i, 6)->asDouble();
            fDbBind[":f_vat"] = ui->tblGoods->lineEdit(i, 7)->asDouble();

            jg["f_doc"] = ui->leDocNumber->text();
            jg["f_store"] = store;
            jg["f_material"] = ui->tblGoods->toInt(i, 1);
            jg["f_sign"] = -1;
            jg["f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
            jg["f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            jg["f_total"] = ui->tblGoods->lineEdit(i, 6)->asDouble();
            jg["f_vat"] = ui->tblGoods->lineEdit(i, 7)->asDouble();
            jgoods.append(jg);
            if (ui->tblGoods->toInt(i, 0) == 0) {
                int newid = fDb.insert("r_body", fDbBind);
                if (newid < 0) {
                    fDb.fDb.rollback();
                    message_error(fDb.fLastError);
                    return;
                }
                ui->tblGoods->setItemWithValue(i, 0, newid);
            } else if (!fDb.update("r_body", fDbBind, where_id(ui->tblGoods->toInt(i, 0)))) {
                fDb.fDb.rollback();
                message_error(fDb.fLastError);
                return;
            }

        }
    }
    jdoc["goods"] = jgoods;
    fDbBind[":f_doc"] = ui->leDocNumber->text();
    fDbBind[":f_body"] = QString(QJsonDocument(jdoc).toJson(QJsonDocument::Compact));
    fDb.insert("r_docs_json", fDbBind);


    if (ui->leAction->fHiddenText.toInt() == 1) {
        if (docState == 1) {
            ui->wAcc->docType(1);
            ui->wAcc->partner(ui->lePartnerCode->asInt());
            ui->wAcc->docNum(ui->leDocNumber->text());
            ui->wAcc->save();
        } else {
            fDbBind[":f_docType"] = CASHDOC_STORE;
            fDbBind[":f_docNum"] = ui->leDocNumber->text();
            if (fDb.select("delete from c_cash where f_docType=:f_docType and f_docNum=:f_docNum", fDbBind, fDbRows) < 0) {
                fDb.fDb.rollback();
                message_error(fDb.fLastError);
                return;
            }

        }
    }

    /* WRITE STORE RECORD */
    fDbBind[":f_doc"] = ui->leDocNumber->asInt();
    fDb.select("delete from r_store_acc where f_doc=:f_doc", fDbBind, fDbRows);
    if (docState == 1) {
        for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
            if (ui->leAction->fHiddenText.toInt() == STORE_DOC_IN
                    || ui->leAction->fHiddenText.toInt() == STORE_DOC_MOVE) {
                fDbBind[":f_doc"] = ui->leDocNumber->asInt();
                fDbBind[":f_docrow"] = ui->tblGoods->itemValue(i, 0).toInt();
                fDbBind[":f_base"] = ui->tblGoods->itemValue(i, 0).toInt();
                fDbBind[":f_store"] = ui->leStore->fHiddenText.toInt();
                fDbBind[":f_goods"] = ui->tblGoods->itemValue(i, 1).toInt();
                fDbBind[":f_qty"] = ui->tblGoods->lineEdit(i, 3)->asDouble();
                fDbBind[":f_price"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
                fDbBind[":f_sign"] = 1;
                fDb.insert("r_store_acc", fDbBind);
            }
        }
        if (ui->leAction->fHiddenText.toInt() == STORE_DOC_OUT
                || ui->leAction->fHiddenText.toInt() == STORE_DOC_MOVE) {
            QMap<int, double> priceList;
            StoreOutput so(fDb, ui->leDocNumber->asInt());
            so.output(priceList);
            for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
                int code = ui->tblGoods->toInt(i, 0);
                ui->tblGoods->lineEdit(i, 5)->setDouble(priceList[code]);
                ui->tblGoods->lineEdit(i, 5)->textEdited(ui->tblGoods->lineEdit(i, 5)->text());
            }
        }
    }
    /* END WRITE STORE RECORD */

    fDb.fDb.commit();
    bool v = docState == 0;
    ui->wCommon->setEnabled(v);
    for (int j = 0; j < ui->tblGoods->rowCount(); j++) {
        ui->tblGoods->lineEdit(j, 3)->setReadOnly(!v);
        ui->tblGoods->lineEdit(j, 5)->setReadOnly(!v);
        ui->tblGoods->lineEdit(j, 6)->setReadOnly(!v);
    }
    ui->wGoodsOps->setEnabled(v);
    if (docState == 1 && ui->leAction->fHiddenText.toInt() == STORE_DOC_IN) {
        for (int i = 0; i < ui->tblGoods->rowCount(); i++) {
            fDbBind[":f_lastPrice"] = ui->tblGoods->lineEdit(i, 5)->asDouble();
            fDb.update("r_dish", fDbBind, where_id(ui->tblGoods->toInt(i, 1)));
        }
    }
    if (docState == 1) {
        message_info(tr("Saved"));
    } else {
        message_info(tr("Draft saved"));
    }
}

void StoreDoc::editStoreClicked()
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_store, this)) {
        ui->leStore->setText(name);
        ui->leStore->fHiddenText = id;
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
    ui->tblGoods->lineEdit(row, 6)->setDouble(l->asDouble() * ui->tblGoods->lineEdit(row, 5)->asDouble());
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
    ui->tblGoods->lineEdit(row, 6)->setDouble(l->asDouble() * ui->tblGoods->lineEdit(row, 3)->asDouble());
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
    qDebug() << l->text();
    ui->tblGoods->lineEdit(row, 5)->setDouble(l->asDouble() / ui->tblGoods->lineEdit(row, 3)->asDouble());
    countTotal();
}

void StoreDoc::store(CI_RestStore *c)
{
    if (!c) {
        return;
    }
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);
}

void StoreDoc::store2(CI_RestStore *c)
{
    if (!c) {
        return;
    }
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

int StoreDoc::newGoods(CI_Dish *c)
{
    if (!c) {
        return -1;
    }
    int row = ui->tblGoods->rowCount();
    ui->tblGoods->setRowCount(row + 1);
    ui->tblGoods->setItemWithValue(row, 0, "");
    ui->tblGoods->setItemWithValue(row, 1, c->fCode);
    ui->tblGoods->setItemWithValue(row, 2, c->fName);
    ui->tblGoods->setItemWithValue(row, 4, c->fUnitName);
    EQLineEdit *l = ui->tblGoods->addLineEdit(row, 3, false);
    l->setValidator(new QDoubleValidator(0, 10000, 4));
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(qtyChange(QString)));
    l = ui->tblGoods->addLineEdit(row, 5, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(priceChange(QString)));
    EQLineEdit *price = l;
    l = ui->tblGoods->addLineEdit(row, 6, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, SIGNAL(textEdited(QString)), this, SLOT(totalChange(QString)));
    ui->tblGoods->lineEdit(row, 3)->setFocus();
    if (ui->leAction->fHiddenText.toInt() == STORE_DOC_IN) {
        fDbBind[":f_id"] = c->fCode;
        fDb.select("select f_lastprice from r_dish where f_id=:f_id", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            price->setDouble( fDbRows.at(0)[0].toDouble());
        }
    }
    l = ui->tblGoods->addLineEdit(row, 7, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    connect(l, &EQLineEdit::textChanged, [=](const QString &str) {

    });
    l = ui->tblGoods->addLineEdit(row, 8, false);
    l->setValidator(new QDoubleValidator(0, 1000000000, 2));
    l->setReadOnly(true);
    return row;
}

void StoreDoc::on_btnAddMaterial_clicked()
{
    // QMap<int, QString> colFilter;
    // switch (ui->leAction->fHiddenText.toInt()) {
    // case STORE_DOC_IN:
    //     colFilter[3] = ui->leStore->fHiddenText;
    //     break;
    //     colFilter[3] = ui->leStoreout->fHiddenText;
    //     break;
    // }
    // fDockDish->setFilterColumn(colFilter);
    fDockDish->on_lineEdit_textEdited("");
    fDockDish->show();
    fDockDish->activateWindow();
    fDockDish->raise();
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
    fDbBind[":f_doc"] = ui->leDocNumber->asInt();
    fDb.select("delete from r_body where f_doc=:f_doc", fDbBind, fDbRows);

    fDbBind[":f_id"] = ui->leDocNumber->asInt();
    fDb.select("delete from r_docs where f_id=:f_id", fDbBind, fDbRows);

    fDbBind[":f_docType"] = CASHDOC_STORE;
    fDbBind[":f_docNum"] = ui->leDocNumber->asInt();
    fDb.select("delete from c_cash where f_docType=:f_docType and f_docNum=:f_docNum", fDbBind, fDbRows);

    fDbBind[":f_doc"] = ui->leDocNumber->asInt();
    fDb.select("delete from r_store_acc where f_doc=:f_doc", fDbBind, fDbRows);

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
    CI_Dish *d = CacheDish::instance()->get(dr.value("f_dish").toString());
    if (d) {
        newGoods(d);
    }

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
        fDbBind[":f_id"] = ui->tblGoods->toInt(r.at(i), 0);
        fDb.select("delete from r_body where f_id=:f_id", fDbBind, fDbRows);
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
    ui->wAcc->amount(QLocale().toDouble(arg1));
}

void StoreDoc::on_leComments_textChanged(const QString &arg1)
{
    ui->wAcc->comment(arg1);
}

void StoreDoc::on_btnPrintDoc_clicked()
{
    PPrintStoreDoc::print(ui->leDocNumber->text());
}

void StoreDoc::on_btnExcel_clicked()
{
    int colCount = 6;
    int rowCount = ui->tblGoods->rowCount();
    if (colCount == 0 || rowCount == 0) {
        message_error_tr("Empty report!");
        return;
    }
    QList<int> cols;
    cols << 100 << 300 << 100 << 100 << 100 << 100;
    QStringList colName;
    colName << tr("Code") << tr("Name") << tr("Qty") << tr("Unit") << tr("Price") <<  tr("Amount");
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

    e.setValue(ui->leDocNumber->text(), 1, 1);
    e.setValue(ui->deDate->text(), 1, 2);

    for (int j = 0; j < rowCount; j++) {
        e.setValue(ui->tblGoods->item(j, 0)->text(), j + 3, 1);
        e.setValue(ui->tblGoods->item(j, 2)->text(), j + 3, 2);
        e.setValue(ui->tblGoods->lineEdit(j, 3)->text(), j + 3, 3);
        e.setValue(ui->tblGoods->item(j, 4)->text(), j + 3, 4);
        e.setValue(ui->tblGoods->lineEdit(j, 5)->text(), j + 3, 5);
        e.setValue(ui->tblGoods->lineEdit(j, 6)->text(), j + 3, 6);
    }

    e.setFontSize(e.address(0, 0), e.address(rowCount + 2, colCount ), 10);
    e.show();
}

void StoreDoc::on_btnImportXML_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "XML File", "", "*.xml");
    if (fileName.isEmpty()) {
        return;
    }
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::ReadOnly)){
        return;
    }
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2.exec("select f_id, f_armsoftname from r_dish where length(f_armsoftname) >0");
    QMap<QString, int> nameMap;
    while (db2.next()) {
        nameMap[db2.string("f_armsoftname")] = db2.integer("f_id");
    }
    auto *xmlReader = new QXmlStreamReader(&xmlFile);
    QString name, price, qty, priceVat, vatRate;
    bool good = false;
    while (!xmlReader->atEnd() && !xmlReader->hasError()) {
        QXmlStreamReader::TokenType tt = xmlReader->readNext();
        if (tt == QXmlStreamReader::StartDocument) {
            continue;
        }
        if (tt == QXmlStreamReader::StartElement) {
            if (xmlReader->name() == "Good") {
                good = true;
            } else if (xmlReader->name() == "Description") {
                name = xmlReader->readElementText();
            } else if (xmlReader->name() == "PricePerUnit") {
                price = xmlReader->readElementText();
            } else if (xmlReader->name() == "Amount") {
                qty = xmlReader->readElementText();
            } else if (xmlReader->name() == "Price") {
                priceVat = xmlReader->readElementText();
            } else if (xmlReader->name() == "VATRate") {
                vatRate = xmlReader->readElementText();
            }
        } else
        if (tt == QXmlStreamReader::EndElement) {
            if (xmlReader->name() == "Good") {
               good = false;
               if (!nameMap.contains(name.toUpper())) {
                   message_error(name + "<br> Անհայտ անվանում");
               } else {
                   fDbBind[":f_id"] = nameMap[name.toUpper()];
                   DatabaseResult dr;
                   dr.select(fDb, "select f_id from r_dish where f_id=:f_id", fDbBind);
                   if (dr.rowCount() == 0) {
                       message_error(tr("Code not found"));
                       return;
                   }
                   CI_Dish *d = CacheDish::instance()->get(dr.value("f_dish").toString());
                   if (d) {
                       int row = newGoods(d);
                       ui->tblGoods->lineEdit(row, 3)->setText(qty);
                       ui->tblGoods->lineEdit(row, 5)->setText(price);
                       ui->tblGoods->lineEdit(row, 6)->setText(float_str(qty.toDouble() * price.toDouble(), 2));
                       ui->tblGoods->lineEdit(row, 7)->setText(float_str(price.toDouble() + (price.toDouble() * vatRate.toDouble() / 100), 2));
                       ui->tblGoods->lineEdit(row, 8)->setText(float_str(qty.toDouble() * ui->tblGoods->lineEdit(row, 7)->asDouble(), 2));
                   }
               }
            }
        }
    }
    if(xmlReader->hasError()) {
            QMessageBox::critical(this,
            "xmlFile.xml Parse Error",xmlReader->errorString(),
            QMessageBox::Ok);
            return;
    }

    xmlReader->clear();
    xmlFile.close();
    countTotal();
}
