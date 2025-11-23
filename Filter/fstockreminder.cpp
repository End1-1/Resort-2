#include "fstockreminder.h"
#include "ui_fstockreminder.h"
#include "wreportgrid.h"
#include "dwselectorreststore.h"

FStockReminder::FStockReminder(QWidget *parent)
    : WFilterBase(parent), ui(new Ui::FStockReminder)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Stock reminder"), ":/images/notepad.png");
    DWSelectorRestStore *dockStore = new DWSelectorRestStore(this);
    dockStore->configure();
    dockStore->setSelector(ui->leStore);
    connect(dockStore, &DWSelectorRestStore::store, [this](CI_RestStore * ci) {
        dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, ci);

        if(ci) {
            fStore = ci->fCode;
        }
    });
    connect(fReportGrid, SIGNAL(doubleClickOnRow2(int, int, QList<QVariant>)), this, SLOT(doubleClickOnRow(int, int, QList<QVariant>)));
}

FStockReminder::~FStockReminder() { delete ui; }

void FStockReminder::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumns({
        {150, "f_name", tr("Store")},
        {0, "f_id", tr("Goods code")},
        {250, "f_en", tr("Name")},
        {100, "f_minreminder", tr("Reminder")},
        {100, "f_stock", tr("Stock")}});
    QString sql = R"(
    select s.f_name, g.f_id, g.f_en, g.f_minreminder, r.f_qty
     from r_dish g
     LEFT JOIN (SELECT f_store, f_goods, SUM(f_qty*f_sign) AS f_qty FROM r_store_acc GROUP BY 1,2) r ON  g.f_id=r.f_goods
    LEFT JOIN r_store s ON s.f_id=r.f_store
    %where%
)";
    QString where = "where g.f_minreminder>0 ";
    if (!ui->leStore->text().isEmpty()) {
        where += " and r.f_store in (" + ui->leStore->fHiddenText + ") ";
    }
    if (!ui->chAll->isChecked()) {
        where += " and g.f_minreminder >= r.f_qty";
    }


    sql.replace("%where%", where);
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    //heirg->fTableView->resizeColumnsToContents();
}

QString FStockReminder::reportTitle()
{
    return  tr("Store reminder");
}

QWidget* FStockReminder::firstElement()
{
    return ui->leStore;
}

void FStockReminder::doubleClickOnRow(int row, int column, const QList<QVariant>& values)
{
    if(values.length() > 0) {
    }
}
