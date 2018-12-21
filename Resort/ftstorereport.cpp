#include "ftstorereport.h"
#include "ui_ftstorereport.h"
#include "wreportgrid.h"

#include "dwselectorreststore.h"
#include "dwselectorstorepartners.h"
#include "storedoc.h"

#define SEL_DISH 1
#define SEL_PARTNER 2

FTStoreReport::FTStoreReport(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FTStoreReport)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("T report"), ":/images/notepad.png");
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });

    connect(fReportGrid, SIGNAL(doubleClickOnRow2(int,int,QList<QVariant>)), this, SLOT(doubleClickOnRow(int,int,QList<QVariant>)));

    fDockDish = new DWSelectorDish(this);
    fDockDish->configure();
    fDockDish->setSelector(ui->leGoods);
    fDockDish->setDialog(this, SEL_DISH);

    DWSelectorRestStore *dockStore = new DWSelectorRestStore(this);
    dockStore->configure();
    dockStore->setSelector(ui->leStore);
    connect(dockStore, &DWSelectorRestStore::store, [this](CI_RestStore *ci) {
        dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, ci);
        if (ci) {
            fStore = ci->fCode;
            QMap<int, QString> colFilter;
            colFilter[3] = fStore;
            fDockDish->setFilterColumn(colFilter);
        }
    });

    DWSelectorStorePartners *dockPartner = new DWSelectorStorePartners(this);
    dockPartner->configure();
    dockPartner->setSelector(ui->lePartner);
    dockPartner->setDialog(this, SEL_PARTNER);

}

FTStoreReport::~FTStoreReport()
{
    delete ui;
}

QString FTStoreReport::reportTitle()
{
    QString title = tr("T report %1 - %2").arg(ui->wd->ds1()).arg(ui->wd->ds2());
    if (!ui->lePartner->isEmpty()) {
        title += ", " + ui->lePartner->text();
    }
    if (!ui->leGoods->isEmpty()) {
        title += ", " + ui->leGoods->text();
    }
    return title;
}

QWidget *FTStoreReport::firstElement()
{
    return ui->wd->fw();
}

void FTStoreReport::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->clearData();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(80, "", tr("Begin, qty"))
            .setColumn(80, "", tr("Begin, amount"))
            .setColumn(80, "", tr("Input, qty"))
            .setColumn(80, "", tr("Input, amount"))
            .setColumn(80, "", tr("Auto input, qty"))
            .setColumn(80, "", tr("Auto input, amount"))
            .setColumn(80, "", tr("Output, qty"))
            .setColumn(80, "", tr("Output, amount"))
            .setColumn(80, "", tr("Final, qty"))
            .setColumn(80, "", tr("Final, amount"))
            .setColumn(80, "", tr("Check inv"))
            ;
    QDate d1 = ui->wd->d1();
    QDate d2 = ui->wd->d2();
    QMap<QDate, int> dateMap;
    int r = d1.daysTo(d2) + 1;
    for (int i = 0; i < r; i++) {
        dateMap[d1.addDays(i)] = i;
        QList<QVariant> row;
        row << d1.addDays(i)
            << 0
            << 0
            << 0
            << 0
            << 0
            << 0
            << 0
            << 0
            << 0
            << 0
            << 0;
        rg->fModel->appendRow(row);
    }
    rg->fModel->apply(rg);

    QString where;
    if (!ui->leStore->isEmpty()) {
        where += " and b.f_store in (" + ui->leStore->fHiddenText + ") ";
    }
    if (!ui->leGoods->isEmpty()) {
        where += " and b.f_material in (" + ui->leGoods->fHiddenText + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        where += " and d.f_partner in (" + ui->lePartner->fHiddenText + ") ";
    }
    //input, auto input, out
    QString query = "select d.f_date, "
            "b.f_material as matcode, m.f_en as f_material, b.f_qty, b.f_price, b.f_total, b.f_sign, d.f_remarks "
            "from r_body b "
            "left join r_docs d on d.f_id=b.f_doc "
            "left join r_partners pn on pn.f_id=d.f_partner "
            "left join r_doc_type dt on dt.f_id=d.f_type "
            "left join r_store s on s.f_id=b.f_store "
            "left join r_dish m on m.f_id=b.f_material "
            "where d.f_date between " + ui->wd->ds1() + " and " + ui->wd->ds2() +
            " and d.f_state=1 " +  where;
    fDb.select(query, fDbBind, fDbRows);
    for (int i = 0; i < fDbRows.count(); i++) {
        r = dateMap[fDbRows.at(i).at(0).toDate()];
        int col;
        if (fDbRows.at(i).at(6).toInt() == 1) {
            if (fDbRows.at(i).at(7).toString() == "AUTO INPUT") {
                col = 5;
            } else {
                col = 3;
            }
        } else {
            col = 7;
        }
        rg->fModel->setData(r, col, rg->fModel->data(r, col).toDouble() + fDbRows.at(i).at(3).toDouble());
        rg->fModel->setData(r, col + 1, rg->fModel->data(r, col + 1).toDouble() + fDbRows.at(i).at(5).toDouble());
    }

    where = "";
    if (!ui->leStore->isEmpty()) {
        where += " and b.f_store in (" + ui->leStore->fHiddenText + ") ";
    }
    if (!ui->leGoods->isEmpty()) {
        where += " and b.f_material in (" + ui->leGoods->fHiddenText + ") ";
    }
    //begin
    query = "select sum(b.f_qty*b.f_sign) as f_qty, "
            "sum(b.f_total)/sum(b.f_qty) as f_price, sum(b.f_total*b.f_sign) as f_total "
            "from r_body b "
            "left join r_store s on s.f_id=b.f_store "
            "left join r_dish d on d.f_id=b.f_material "
            "left join r_docs bd on bd.f_id=b.f_doc "
            "where bd.f_date<" + ui->wd->ds1() + " and bd.f_state=1 " + where ;

    fDb.select(query, fDbBind, fDbRows);
    if (rg->fModel->rowCount() > 0) {
        rg->fModel->setData(0, 1, fDbRows.at(0).at(0));
        rg->fModel->setData(0, 2, fDbRows.at(0).at(2));
    }

    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        rg->fModel->setData(i, 9, rg->fModel->data(i, 1, Qt::EditRole).toDouble()
                            + rg->fModel->data(i, 3, Qt::EditRole).toDouble()
                            + rg->fModel->data(i, 5, Qt::EditRole).toDouble()
                            - rg->fModel->data(i, 7, Qt::EditRole).toDouble());
        rg->fModel->setData(i, 10, rg->fModel->data(i, 2, Qt::EditRole).toDouble()
                            + rg->fModel->data(i, 4, Qt::EditRole).toDouble()
                            + rg->fModel->data(i, 6, Qt::EditRole).toDouble()
                            - rg->fModel->data(i, 8, Qt::EditRole).toDouble());
        if (i < rg->fModel->rowCount() - 1) {
            rg->fModel->setData(i + 1, 1, rg->fModel->data(i, 9, Qt::EditRole));
            rg->fModel->setData(i + 1, 2, rg->fModel->data(i, 10, Qt::EditRole));
        }
    }

    QList<int> cols;
    cols << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    if (vals.count() > 0 && rg->fModel->rowCount() > 0) {
        vals[0] = rg->fModel->data(0, 1, Qt::EditRole).toDouble();
        vals[1] = rg->fModel->data(0, 2, Qt::EditRole).toDouble();
        vals[7] = vals[0] + vals[2] + vals[4] - vals[6];
        vals[8] = vals[1] + vals[3] + vals[5] - vals[7];
        rg->setTblTotalData(cols, vals);
    }

    if (ui->leStore->fHiddenText.toInt() > 0 && ui->leGoods->fHiddenText.toInt() > 0) {
        QDate d1 = ui->wd->d1();
        QDate d2 = ui->wd->d2();
        while (d1 < d2.addDays(1)) {
            if (d1.day() == d1.daysInMonth()) {
                fDbBind[":f_date"] = d1;
                fDbBind[":f_store"] = ui->leStore->fHiddenText.toInt();
                fDbBind[":f_goods"] = ui->leGoods->fHiddenText.toInt();
                DatabaseResult drs;
                drs.select(fDb, "select h.f_date, sum(b.f_qty) as f_qty from st_body b "
                           "inner join st_header h on h.f_id=b.f_doc "
                           "where h.f_date=:f_date and h.f_store=:f_store and b.f_goods=:f_goods "
                           "group by 1 ", fDbBind);
                if (drs.rowCount() > 0) {
                    int rr = dateMap[drs.value(0).toDate()];
                    rg->fModel->setData(rr, 11, drs.value("f_qty"));
                }
            }
            d1 = d1.addDays(1);
        }
    }

    for (int i = rg->fModel->rowCount() - 1; i > -1; i--) {
        QDate d1 = rg->fModel->data(i, 0, Qt::EditRole).toDate();
        if (d1.day() == d1.daysInMonth()) {
            continue;
        }
        if (rg->fModel->data(i, 3, Qt::EditRole).toDouble() < 0.001 && rg->fModel->data(i, 5, Qt::EditRole).toDouble() < 0.001 && rg->fModel->data(i, 7, Qt::EditRole).toDouble() < 0.001) {
            rg->fModel->removeRow(i);
        }
    }

    for (int i = rg->fModel->rowCount() - 1; i > -1; i--) {
        QDate d1 = rg->fModel->data(i, 0, Qt::EditRole).toDate();
        if (d1.day() != d1.daysInMonth()) {
            continue;
        }
        if (rg->fModel->data(i, 11, Qt::EditRole).toDouble() != rg->fModel->data(i, 9, Qt::EditRole).toDouble()) {
            rg->fModel->setBackgroundColor(i, Qt::red);
        }
    }
}

void FTStoreReport::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_DISH: {
        CI_Dish *ci = value.value<CI_Dish*>();
        dockResponse<CI_Dish, CacheDish>(ui->leGoods, ci);
        break;
    }
    case SEL_PARTNER: {
        CI_StorePartners *ci = value.value<CI_StorePartners*>();
        dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartner, ci);
        break;
    }
    }
}

void FTStoreReport::setParams(const QString &store, const QString &goods)
{
    ui->leStore->fHiddenText = store;
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, 0);
    ui->leGoods->fHiddenText = goods;
    dockResponse<CI_Dish, CacheDish>(ui->leGoods, 0);
    apply(fReportGrid);
}

void FTStoreReport::doubleClickOnRow(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    if (ui->leGoods->fHiddenText.isEmpty()) {
        return;
    }
    QString addAnd;
    switch (column) {
    case 3:
    case 4:
        addAnd += " and d.f_id in (select f_doc from r_body where f_sign=1 and f_material=:f_material) ";
        break;
    case 5:
    case 6:
        addAnd += " and d.f_id in (select f_doc from r_body where f_sign=1 and f_material=:f_material) and d.f_remarks='AUTO INPUT' ";
        break;
    case 7:
    case 8:
        addAnd += " and d.f_id in (select f_doc from r_body where f_sign=-1 and f_material=:f_material) ";
        break;
    default:
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_date"] = values.at(0).toDate();
    fDbBind[":f_material"] = ui->leGoods->fHiddenText.toInt();
    dr.select(fDb, "select d.f_id from r_docs d "
              "where d.f_state=1 and d.f_date=:f_date " + addAnd, fDbBind);
    if (dr.rowCount() == 0) {
        return;
    }
    for (int i = 0; i < dr.rowCount(); i++) {
        StoreDoc *d = addTab<StoreDoc>();
        d->loadDoc(dr.value(0).toString());
    }
}
