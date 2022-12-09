#include "fstoredocs.h"
#include "ui_fstoredocs.h"
#include "wreportgrid.h"
#include "storedoc.h"
#include "cachecashdoc.h"
#include <QMenu>

#define SEL_DOC_STATE 1
#define SEL_DOC_TYPE 2
#define SEL_PARTNER 3

FStoreDocs::FStoreDocs(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FStoreDocs)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Documents in store"), ":/images/storage.png");
    if (check_permission(pr_edit_store_doc)) {
        fReportGrid->addToolBarButton(":/images/copy.png", tr("Copy"), SLOT(copyDoc()), this)->setFocusPolicy(Qt::NoFocus);
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

QWidget *FStoreDocs::firstElement()
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
    if (!ui->leDocType->isEmpty()) {
        where += " and d.f_type in (" + ui->leDocType->fHiddenText + ") ";
    }
    if (!ui->leState->isEmpty()) {
        where += " and d.f_state in (" + ui->leState->fHiddenText + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        where += " and d.f_partner in (" + ui->lePartner->fHiddenText + ") ";
    }
    if (!ui->leStore->isEmpty()) {
        where += " and b.f_store in (" + ui->leStore->fHiddenText + ") ";
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
    switch (number) {
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
    for (QModelIndex &mi: sr) {
        rows.insert(mi.row());
    }
    QList<int> rl = rows.toList();
    if (rl.count() > 0) {
        if (message_confirm_tr("Confirm to delete document") != QDialog::Accepted) {
            return;
        }
    } else {
        return;
    }
    for (int i = rl.count() - 1; i > -1; i--) {
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
    if (row < 0) {
        message_error(tr("Nothing is seleted."));
        return;
    }
    StoreDoc *d = addTab<StoreDoc>();
    d->copyDoc(out.at(0).toInt());
}

void FStoreDocs::doubleClicked(const QList<QVariant> &row)
{
    if (row.count() == 0) {
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
