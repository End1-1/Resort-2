#include "fstoremovement.h"
#include "ui_fstoremovement.h"
#include "wreportgrid.h"
#include "storedoc.h"
#include "dwselectorreststore.h"
#include "dwselectorstoredoctype.h"
#include "dwselectordish.h"
#include "dwselectorstorepartners.h"

#define SEL_DOC_TYPE 1
#define SEL_DISH 2
#define SEL_PARTNER 3

FStoreMovement::FStoreMovement(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FStoreMovement)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Store movement"), ":/images/storage.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClicked(QList<QVariant>)));

    DWSelectorStoreDocType *dt = new DWSelectorStoreDocType(this);
    dt->configure();
    dt->setSelector(ui->leAction);
    dt->setDialog(this, SEL_DOC_TYPE);

    DWSelectorRestStore *fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, &DWSelectorRestStore::store, [this](CI_RestStore *c) {
       dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);
    });

    DWSelectorDish *dd = new DWSelectorDish(this);
    dd->configure();
    dd->setSelector(ui->leMaterial);
    dd->setDialog(this, SEL_DISH);

    DWSelectorStorePartners *dp = new DWSelectorStorePartners(this);
    dp->configure();
    dp->setSelector(ui->lePartner);
    dp->setDialog(this, SEL_PARTNER);
}

FStoreMovement::~FStoreMovement()
{
    delete ui;
}

void FStoreMovement::selector(int number, const QVariant &value)
{
    switch (number) {
    case SEL_DOC_TYPE: {
        CI_StoreDocType *c = value.value<CI_StoreDocType*>();
        dockResponse<CI_StoreDocType, CacheStoreDocType>(ui->leAction, c);
        break;
    }
    case SEL_DISH: {
        CI_Dish *c = value.value<CI_Dish*>();
        dockResponse<CI_Dish, CacheDish>(ui->leMaterial, c);
        break;
    }
    case SEL_PARTNER: {
        CI_StorePartners *ci = value.value<CI_StorePartners*>();
        dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartner, ci);
        break;
    }
    }
}

QWidget *FStoreMovement::firstElement()
{
    return ui->deStart;
}

QString FStoreMovement::reportTitle()
{
    return QString("%1, %2-%3")
            .arg(tr("Store movement"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FStoreMovement::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Document"))
            .setColumn(120, "", tr("Partner"))
            .setColumn(100, "", tr("Date"))
            .setColumn(120, "", tr("Action"))
            .setColumn(150, "", tr("Store"))
            .setColumn(80, "", tr("Code"))
            .setColumn(200, "", tr("Material"))
            .setColumn(80, "", tr("Qty"))
            .setColumn(80, "", tr("Price"))
            .setColumn(80, "", tr("Total"));
    QString query = "select b.f_doc, pn.f_name, d.f_date, dt.f_name as f_action, s.f_name as f_store, "
            "b.f_material as matcode, m.f_en as f_material, b.f_qty * b.f_sign as f_qty, b.f_price, b.f_total * b.f_sign as f_total "
            "from r_body b "
            "left join r_docs d on d.f_id=b.f_doc "
            "left join r_partners pn on pn.f_id=d.f_partner "
            "left join r_doc_type dt on dt.f_id=d.f_type "
            "left join r_store s on s.f_id=b.f_store "
            "left join r_dish m on m.f_id=b.f_material "
            "where d.f_date between " + ui->deStart->dateMySql() + " and " + ui->deEnd->dateMySql() +
            " and d.f_state=1 ";
    if (!ui->leAction->isEmpty()) {
        query += " and d.f_type in (" + ui->leAction->fHiddenText + ") ";
    }
    if (!ui->leStore->isEmpty()) {
        query += " and b.f_store in (" + ui->leStore->fHiddenText + ") ";
    }
    if (!ui->leMaterial->isEmpty()) {
        query += " and b.f_material in (" + ui->leMaterial->fHiddenText + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        query +=  " and d.f_partner in (" + ui->lePartner->fHiddenText + ") ";
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> col;
    col << 5 << 7;
    QList<double> val;
    rg->fModel->sumOfColumns(col, val);
    rg->setTblTotalData(col, val);
}

void FStoreMovement::doubleClicked(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    StoreDoc *d = addTab<StoreDoc>();
    d->loadDoc(row.at(0).toString());
}

void FStoreMovement::on_btnDateLeft_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FStoreMovement::on_btnDateRight_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}
