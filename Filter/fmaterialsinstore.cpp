#include "fmaterialsinstore.h"
#include "ui_fmaterialsinstore.h"
#include "wreportgrid.h"
#include "ftstorereport.h"
#include "dlggetidname.h"
#define SEL_DISH 1

FMaterialsInStore::FMaterialsInStore(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FMaterialsInStore)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Materials in store"), ":/images/storage.png");
    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, SIGNAL(store(CI_RestStore*)), this, SLOT(store(CI_RestStore*)));
    connect(ui->leMaterial, &EQLineEdit::customButtonClicked, this, &FMaterialsInStore::goodsClick);
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
}

FMaterialsInStore::~FMaterialsInStore()
{
    delete ui;
}

QWidget* FMaterialsInStore::firstElement()
{
    return ui->deDate;
}

QString FMaterialsInStore::reportTitle()
{
    return QString("%1, %2")
           .arg("Materials in the store")
           .arg(ui->deDate->text());
}

void FMaterialsInStore::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(150, "", tr("Store"))
    .setColumn(0, "", tr("Code"))
    .setColumn(300, "", tr("Group"))
    .setColumn(300, "", tr("Material"))
    .setColumn(100, "", tr("Qty"))
    .setColumn(100, "", tr("Reminder"));
    QString where;

    if(!ui->leStore->isEmpty()) {
        where += " and b.f_store in (" + ui->leStore->fHiddenText + ") ";
    }

    if(!ui->leMaterial->isEmpty()) {
        where += " and b.f_goods in (" + ui->leMaterial->fHiddenText + ") ";
    }

    QString query = "select s.f_name as f_store, "
                    "b.f_goods,"
                    " dt.f_en as f_group,"
                    " d.f_en as f_material, "
                    "sum(b.f_qty*b.f_sign) as f_qty, "
                    "d.f_minreminder "
                    "from r_store_acc b "
                    "left join r_store s on s.f_id=b.f_store "
                    "left join r_dish d on d.f_id=b.f_goods "
                    "left join r_dish_type dt on dt.f_id=d.f_type "
                    "left join r_docs bd on bd.f_id=b.f_doc "
                    "where bd.f_date<=" + ui->deDate->dateMySql() + " and bd.f_state=1 " + where +
                    "group by 1, 2 "
                    "order by 1, 3 ";
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> col;
    col << 4;
    QList<double> val;
    rg->fModel->sumOfColumns(col, val);
    rg->setTblTotalData(col, val);
}

void FMaterialsInStore::store(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);

    if(c) {
        QMap<int, QString> colFilter;
        colFilter[3] = c->fCode;
    }
}

void FMaterialsInStore::doubleClickOnRow(const QList<QVariant>& row)
{
    if(row.count() == 0) {
        return;
    }

    FTStoreReport *f = FTStoreReport::openFilterReport<FTStoreReport, WReportGrid>();
    f->setParams(ui->leStore->fHiddenText, row.at(1).toString());
}

void FMaterialsInStore::goodsClick()
{
    QString id, name;

    if(DlgGetIDName::get(id, name, idname_dish, this)) {
        ui->leMaterial->setText(name);
        ui->leMaterial->fHiddenText = id;
    }
}
