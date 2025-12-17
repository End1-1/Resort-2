#include "fstoredocs.h"
#include "ui_fstoredocs.h"
#include "wreportgrid.h"
#include "storedoc.h"
#include "cachecashdoc.h"
#include "database2.h"
#include <QMenu>
#include <QDir>
#include <QProcess>
#include <QTemporaryFile>

#define SEL_DOC_STATE 1
#define SEL_DOC_TYPE 2
#define SEL_PARTNER 3

FStoreDocs::FStoreDocs(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FStoreDocs)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Documents in store"), ":/images/storage.png");

    if(check_permission(pr_edit_store_doc)) {
        fReportGrid->addToolBarButton(":/images/copy.png", tr("Copy"), SLOT(copyDoc()), this)->setFocusPolicy(Qt::NoFocus);
        fReportGrid->addToolBarButton(":/images/AS.png", tr("Import"), SLOT(importAS()), this)->setFocusPolicy(Qt::NoFocus);
    }

    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClicked(QList<QVariant>)));
    fDockState = new DWSelectorStoreDocState(this);
    fDockState->configure();
    fDockState->setSelector(ui->leState);
    fDockState->setDialog(this, SEL_DOC_STATE);
    fDockType = new DWSelectorStoreDocType(this);
    fDockType->configure();
    fDockType->setSelector(ui->leDocType);
    fDockType->setDialog(this, SEL_DOC_TYPE);
    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, SIGNAL(store(CI_RestStore*)), this, SLOT(store(CI_RestStore*)));
    fDockPartner = new DWSelectorStorePartners(this);
    fDockPartner->configure();
    fDockPartner->setSelector(ui->lePartner);
    fDockPartner->setDialog(this, SEL_PARTNER);
    fReportGrid->fGridMenu->addAction(QIcon(":/garbage.png"), tr("Remove selected"), this, SLOT(removeSelected()));
}

FStoreDocs::~FStoreDocs()
{
    delete ui;
}

QWidget* FStoreDocs::firstElement()
{
    return ui->deStart;
}

QString FStoreDocs::reportTitle()
{
    return QString("%1 %2-%3")
           .arg(tr("Documents in store"))
           .arg(ui->deStart->text())
           .arg(ui->deEnd->text());
}

void FStoreDocs::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Document number"))
    .setColumn(120, "", tr("Date"))
    .setColumn(220, "", tr("Type"))
    .setColumn(120, "", tr("State"))
    .setColumn(120, "", tr("Store"))
    .setColumn(100, "", tr("Amount"))
    .setColumn(100, "", tr("Partner"))
    .setColumn(150, "", tr("Operator"))
    .setColumn(150, "", tr("Registered"))
    .setColumn(400, "", tr("Comment"));
    QString query = "select distinct(d.f_id) as f_id, d.f_date, dt.f_name, ds.f_name, "
                    "st.f_name, d.f_amount, p.f_name, u.f_username, d.f_fullDate, d.f_remarks "
                    "from r_docs d "
                    "left join r_doc_type dt on dt.f_id=d.f_type "
                    "left join r_doc_state ds on ds.f_id=d.f_state "
                    "left join r_partners p on p.f_id=d.f_partner "
                    "left join r_body b on b.f_doc=d.f_id "
                    "left join r_store st on st.f_id=b.f_store "
                    "left join users u on u.f_id=d.f_op ";
    QString where = "where d.f_date between " + ui->deStart->dateMySql() + " and " + ui->deEnd->dateMySql();

    if(!ui->leDocType->isEmpty()) {
        where += " and d.f_type in (" + ui->leDocType->fHiddenText + ") ";
    }

    if(!ui->leState->isEmpty()) {
        where += " and d.f_state in (" + ui->leState->fHiddenText + ") ";
    }

    if(!ui->lePartner->isEmpty()) {
        where += " and d.f_partner in (" + ui->lePartner->fHiddenText + ") ";
    }

    if(!ui->leStore->isEmpty()) {
        where += " and b.f_store in (" + ui->leStore->fHiddenText + ") ";
    }

    if(ui->chArmSoft->isChecked()) {
        where += " and d.f_as is not null";
    }

    rg->fModel->setSqlQuery(query + where);
    rg->fModel->apply(rg);
    QList<int> col;
    col << 5;
    QList<double> val;
    rg->fModel->sumOfColumns(col, val);
    rg->setTblTotalData(col, val);
}

void FStoreDocs::selector(int number, const QVariant &value)
{
    switch(number) {
    case SEL_DOC_STATE: {
        CI_StoreDocState *c = value.value<CI_StoreDocState*>();
        dockResponse<CI_StoreDocState, CacheStoreDocState>(ui->leState, c);
        break;
    }

    case SEL_DOC_TYPE: {
        CI_StoreDocType *c = value.value<CI_StoreDocType*>();
        dockResponse<CI_StoreDocType, CacheStoreDocType>(ui->leDocType, c);
        break;
    }

    case SEL_PARTNER: {
        CI_StorePartners *c = value.value<CI_StorePartners*>();
        dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartner, c);
        break;
    }
    }
}

void FStoreDocs::openReport()
{
    openFilterReport<FStoreDocs, WReportGrid>();
}

void FStoreDocs::store(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);
}

void FStoreDocs::removeSelected()
{
    QModelIndexList sr = fReportGrid->fTableView->selectionModel()->selectedIndexes();
    QSet<int> rows;

    for(QModelIndex &mi : sr) {
        rows.insert(mi.row());
    }

    QList<int> rl = rows.toList();

    if(rl.count() > 0) {
        if(message_confirm_tr("Confirm to delete document") != QDialog::Accepted) {
            return;
        }
    } else {
        return;
    }

    for(int i = rl.count() - 1; i > -1; i--) {
        int docnum = fReportGrid->fModel->data(i, 0).toInt();
        fDbBind[":f_doc"] = docnum;
        fDb.select("delete from r_body where f_doc=:f_doc", fDbBind, fDbRows);
        fDbBind[":f_id"] = docnum;
        fDb.select("delete from r_docs where f_id=:f_id", fDbBind, fDbRows);
        fDbBind[":f_docType"] = CASHDOC_STORE;
        fDbBind[":f_docNum"] = docnum;
        fDb.select("delete from c_cash where f_docType=:f_docType and f_docNum=:f_docNum", fDbBind, fDbRows);
        fDbBind[":f_doc"] = docnum;
        fDb.select("delete from r_store_acc where f_doc=:f_doc", fDbBind, fDbRows);
    }

    message_info(tr("Done"));
}

void FStoreDocs::copyDoc()
{
    QList<QVariant> out;
    int row = fReportGrid->fillRowValuesOut(out);

    if(row < 0) {
        message_error(tr("Nothing is seleted."));
        return;
    }

    StoreDoc *d = addTab<StoreDoc>();
    d->copyDoc(out.at(0).toInt());
}

void FStoreDocs::importAS()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_date1"] = ui->deStart->date();
    db2[":f_date2"] = ui->deEnd->date();
    db2.exec("select f_id from r_docs where f_date between :f_date1 and :f_date2 and f_as is not null");
    QList<int> ids;

    while(db2.next()) {
        ids.append(db2.integer("f_id"));
    }

    for(auto id : ids) {
        db2[":f_doc"] = id;
        db2.exec("delete from r_body where f_doc=:f_doc");
        db2[":f_id"] = id;
        db2.exec("delete from r_docs where f_id=:f_id");
        db2[":f_docType"] = CASHDOC_STORE;
        db2[":f_docNum"] = id;
        db2.exec("delete from c_cash where f_docType=:f_docType and f_docNum=:f_docNum");
        db2[":f_doc"] = id;
        db2.exec("delete from r_store_acc where f_doc=:f_doc");
    }

    QMap<QString, int> goodsMap;
    db2.exec("SELECT f_id, f_as FROM r_dish WHERE LENGTH(f_as)>0");

    while(db2.next()) {
        goodsMap[db2.string("f_as")] = db2.integer("f_id");
    }

    QMap<QString, int> storeMap;
    QMap<QString, int> storeDocMap;
    QMap<QString, bool> emptyDocs;
    db2.exec("select f_id, f_as from r_store where length(f_as)>0");

    while(db2.next()) {
        storeMap[db2.string("f_as")] = db2.integer("f_id");
    }

    QSqlDatabase sqlDB = QSqlDatabase::addDatabase("QODBC", "F");
    sqlDB.setDatabaseName(
        "Driver={ODBC Driver 17 for SQL Server};"
        "Server=DESKTOP-2011289\\ASACC;"
        "Database=2025;"
        "Uid=sa;"
        "Pwd=SaSa111;"
        "TrustServerCertificate=Yes;"
    );

    if(!sqlDB.open()) {
        message_error(sqlDB.lastError().databaseText());
        return;
    }

    QStringList errors;
    QMap<QString, int> docsIdMap;
    QSqlQuery q(sqlDB);
    q.prepare(R"(
    select fISN, fDOCNUM,fSUMM,fBODY,fDOCTYPE,fDATE,TRY_CAST(
            LEFT(
                SUBSTRING(
                    fBODY,
                    CHARINDEX('STORAGE:', fBODY) + LEN('STORAGE:'),
                    20
                ),
                PATINDEX('%[^0-9]%',
                    SUBSTRING(
                        fBODY,
                        CHARINDEX('STORAGE:', fBODY) + LEN('STORAGE:'),
                        20
                    ) + 'X'
                ) - 1
            ) AS INT
        ) AS STORAGE
    from [2025].dbo.DOCUMENTS
    where fDATE between :date1 and :date2 and fDOCSTATE=2
    and fDOCTYPE in (8);
    )");
    q.bindValue(":date1", ui->deStart->date());
    q.bindValue(":date2", ui->deEnd->date());
    q.exec();
    int rowcount = 0;

    while(q.next()) {
        if(!storeMap.contains(q.value("STORAGE").toString())) {
            // errors.append(QString("Storage not exists %1").arg(q.value("STORAGE").toString()));
            continue;
        }

        emptyDocs[q.value("fISN").toString()] = true;
        rowcount++;
        db2[":f_type"] = 1;
        db2[":f_date"] = q.value("fDATE");
        db2[":f_state"] = 1;
        db2[":f_partner"] = QVariant();
        db2[":f_inv"] = QVariant();
        db2[":f_amount"] = q.value("fSUMM");
        db2[":f_remarks"] = q.value("fDOCNUM");
        db2[":f_as"] = q.value("fISN");
        int newid;

        if(!db2.insert("r_docs", newid)) {
            errors.append(db2.lastDbError());
        }

        storeDocMap[q.value("fISN").toString()] = storeMap[q.value("STORAGE").toString()];
        docsIdMap[q.value("fISN").toString()] = newid;
    }

    errors.append(QString("Records count %1").arg(rowcount));
    QSqlQuery q2(sqlDB);

    if(!q2.prepare(R"(
    select d.fISN,t.fMTCODE  as food, m.fQTY as qty,
    m.fCOSTSUMM as amount, m.fCOSTSUMM / m.fQTY as price,
    m.fDBCR
    from [2025].dbo.MTHI m, [2025].dbo.MATERIALS t , [2025].dbo.DOCUMENTS d
    where m.fMTID=t.fMTID and d.fISN=m.fBASE and d.fDOCTYPE in (8)
    and d.fDATE between :date1 and :date2
    order by m.fPARTYBASE, m.fPARTYROW
    )")) {
        errors.append(q2.lastError().databaseText());
    }

    q2.bindValue(":date1", ui->deStart->date());
    q2.bindValue(":date2", ui->deEnd->date());

    if(!q2.exec()) {
        errors.append(q2.lastError().databaseText());
    }

    int docrow = 1;
    QString isn;

    while(q2.next()) {
        if(!storeDocMap.contains(q2.value("fISN").toString())) {
            continue;
        }

        if(!goodsMap.contains(q2.value("food").toString())) {
            errors.append(QString("Code of dish not exists %1").arg(q2.value("food").toString()));
            continue;
        }

        if(isn != q2.value("fISN").toString()) {
            docrow = 1;
            isn = q2.value("fISN").toString();
        }

        db2[":f_doc"] = docsIdMap[q2.value("fISN").toString()];
        db2[":f_store"] = storeDocMap[q2.value("fISN").toString()];
        db2[":f_material"] = goodsMap[q2.value("food").toString()];
        db2[":f_qty"] = q2.value("qty");
        db2[":f_price"] = q2.value("price").toDouble();
        db2[":f_total"] =  q2.value("qty").toDouble() * q2.value("price").toDouble();
        db2[":f_sign"] = 1;
        int newid = 0;

        if(!db2.insert("r_body", newid)) {
            errors.append(db2.lastDbError());
            continue;
        }

        db2[":f_doc"] = docsIdMap[q2.value("fISN").toString()];
        db2[":f_docrow"] = docrow++;
        db2[":f_base"] = 0;
        db2[":f_store"] = storeDocMap[q2.value("fISN").toString()];
        db2[":f_goods"] = goodsMap[q2.value("food").toString()];
        db2[":f_qty"] = q2.value("qty");
        db2[":f_price"] = q2.value("price");
        db2[":f_sign"] = 1;

        if(!db2.insert("r_store_acc", newid)) {
            errors.append(db2.lastDbError());
        } else {
            emptyDocs[q2.value("fISN").toString()] = false;
        }
    }

    for(auto it = emptyDocs.constBegin(); it != emptyDocs.constEnd(); it++) {
        if(it.value()) {
            db2[":f_id"] = docsIdMap[it.key()];
            db2.exec("delete from r_docs where f_id=:f_id");
        }
    }

    QTemporaryFile tempFile(QDir::tempPath() + "/errors_XXXXXX.txt");
    tempFile.setAutoRemove(false);

    if(!tempFile.open()) {
        message_info(tr("Cannot open temp file"));
        return;
    }

    QTextStream out(&tempFile);
    out.setCodec("UTF-8");

    for(const QString& err : errors) {
        out << err << "\r\n";
    }

    tempFile.close();

    if(!errors.isEmpty()) {
        QProcess::startDetached(
            "notepad.exe",
            QStringList() << QDir::toNativeSeparators(tempFile.fileName())
        );
    }

    message_info(tr("Done"));
}

void FStoreDocs::doubleClicked(const QList<QVariant>& row)
{
    if(row.count() == 0) {
        message_error(tr("Nothing is seleted."));
        return;
    }

    StoreDoc *d = addTab<StoreDoc>();
    d->loadDoc(row.at(0).toInt());
}

void FStoreDocs::on_btnDateLeft_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FStoreDocs::on_btnDateRight_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}
