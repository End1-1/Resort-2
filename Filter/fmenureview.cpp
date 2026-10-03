#include "fmenureview.h"
#include "ui_fmenureview.h"
#include "wreportgrid.h"
#include "dwselectorrestmenu.h"
#include "databaseresult.h"
#include "defines.h"
#include "rerestdish.h"
#include <QButtonGroup>

FMenuReview::FMenuReview(QWidget *parent)
    : WFilterBase(parent), ui(new Ui::FMenuReview), fViewMode(ViewRecipe)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Menu review"), ":/images/notepad.png");
    DWSelectorRestMenu *dockStore = new DWSelectorRestMenu(this);
    dockStore->configure();
    dockStore->setSelector(ui->leMenu);
    connect(dockStore, &DWSelectorRestMenu::menu, [this](CI_RestMenu *ci) {
        dockResponse<CI_RestMenu, CacheRestMenu>(ui->leMenu, ci);
    });

    QButtonGroup *modeGroup = new QButtonGroup(this);
    modeGroup->setExclusive(true);
    modeGroup->addButton(ui->btnRecipe, ViewRecipe);
    modeGroup->addButton(ui->btnPrices, ViewPrices);
    ui->btnRecipe->setChecked(true);
    connect(modeGroup, &QButtonGroup::idClicked, this, &FMenuReview::viewModeChanged);
    connect(fReportGrid, &WReportGrid::doubleClickOnRow, this, &FMenuReview::doubleClickOnRow);
}

void FMenuReview::doubleClickOnRow(const QList<QVariant> &values)
{
    if (values.isEmpty()) {
        return;
    }
    const int dishId = values.at(0).toInt();
    if (dishId <= 0) {
        return;
    }
    if (RERestDish::openEditor(dishId, this)) {
        fReportGrid->on_btnRefresh_clicked();
    }
}

FMenuReview::~FMenuReview() { delete ui; }

void FMenuReview::viewModeChanged(int id)
{
    fViewMode = static_cast<ViewMode>(id);
    if (fReportGrid) {
        fReportGrid->on_btnRefresh_clicked();
    }
}

QString FMenuReview::menuFilterIds() const
{
    const QString hidden = ui->leMenu->fHiddenText.trimmed();
    if (!hidden.isEmpty()) {
        return hidden;
    }
    const QString code = ui->leMenu->text().trimmed();
    if (code.isEmpty()) {
        return QString();
    }
    if (code.contains(',')) {
        return code;
    }
    return code;
}

QString FMenuReview::reportTitle()
{
    return tr("Menu review");
}

QWidget* FMenuReview::firstElement()
{
    return ui->leMenu;
}

void FMenuReview::apply(WReportGrid *rg)
{
    if (menuFilterIds().isEmpty()) {
        return;
    }
    if (fViewMode == ViewPrices) {
        applyPrices(rg);
        return;
    }
    applyRecipe(rg);
}

void FMenuReview::applyRecipe(WReportGrid *rg)
{
    const QString menuIds = menuFilterIds();
    const QString where = "and m.f_menu in (" + menuIds + ") ";
    rg->fModel->clearColumns();
    rg->fModel->setColumns({
        {0, "f_dish", tr("Dish code")},
        {200, "f_dishname", tr("Dish")},
        {200, "f_goodsname", tr("Goods")},
        {100, "f_price", tr("Price")},
        {100, "f_qty", tr("Qty")},
        {100, "f_measname", tr("Meas.")}});
    QString sql = R"(
SELECT m.f_dish, d.f_en as f_dishname, d2.f_en as f_goodsname, m.f_price, r.f_qty,
u.f_name
FROM r_menu m
LEFT JOIN r_dish d ON d.f_id=m.f_dish
LEFT JOIN r_recipe r ON r.f_dish=m.f_dish
LEFT JOIN r_dish d2 ON d2.f_id=r.f_part
LEFT JOIN r_unit u ON u.f_id=d2.f_unit
WHERE m.f_state=1
    %where%
order by 1
    )";
    sql.replace("%where%", where);
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<QVariant> vals = {QVariant(), QVariant(), QVariant(), QVariant(), QVariant(), QVariant()};

    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        if (i == 0) {
            continue;
        }

        if (rg->fModel->data(i, 0).toInt() != rg->fModel->data(i - 1, 0).toInt()) {
            rg->fModel->insertRow(i, vals);
            i++;
        }
    }
}

void FMenuReview::applyPrices(WReportGrid *rg)
{
    const QString menuIds = menuFilterIds();
    QMap<QString, QVariant> bind;

    DatabaseResult menuDr;
    menuDr.select(fDb,
                  QString("select f_id, f_%1 from r_menu_names where f_id in (%2) order by f_id")
                      .arg(def_lang, menuIds),
                  bind);

    if (menuDr.rowCount() == 0) {
        rg->fModel->clearColumns();
        rg->fModel->setSqlQuery(QString());
        rg->fModel->setDataFromSource(QList<QList<QVariant>>());
        return;
    }

    QList<int> menuIdList;
    QStringList menuTitles;
    for (int i = 0; i < menuDr.rowCount(); i++) {
        menuIdList << menuDr.value(i, 0).toInt();
        menuTitles << menuDr.value(i, 1).toString();
    }

    bind.clear();
    const QString sql = QString(
                            "select d.f_id as f_dish_id, p.f_%1 as f_group, d.f_%1 as f_dish, "
                            "s.f_name as f_store, m.f_print2, m.f_print1, m.f_menu, m.f_price "
                            "from r_menu m "
                            "inner join r_menu_names mn on mn.f_id=m.f_menu and mn.f_enabled=1 "
                            "inner join r_dish d on d.f_id=m.f_dish "
                            "inner join r_dish_type t on t.f_id=d.f_type "
                            "inner join r_dish_part p on p.f_id=t.f_part "
                            "inner join r_store s on s.f_id=m.f_store "
                            "where m.f_state=1 and m.f_menu in (%2) "
                            "order by p.f_%1, d.f_%1, m.f_menu")
                            .arg(def_lang, menuIds);

    DatabaseResult dr;
    dr.select(fDb, sql, bind);

    struct DishRow {
        QString group;
        QString dish;
        QString store;
        QString print;
        QMap<int, QVariant> prices;
    };

    QMap<int, DishRow> byDish;
    QList<int> dishOrder;

    for (int i = 0; i < dr.rowCount(); i++) {
        const int dishId = dr.value(i, "f_dish_id").toInt();
        const int menuId = dr.value(i, "f_menu").toInt();
        if (!byDish.contains(dishId)) {
            DishRow row;
            row.group = dr.value(i, "f_group").toString();
            row.dish = dr.value(i, "f_dish").toString();
            row.store = dr.value(i, "f_store").toString();
            QString print = dr.value(i, "f_print2").toString();
            if (print.isEmpty()) {
                print = dr.value(i, "f_print1").toString();
            }
            row.print = print;
            byDish.insert(dishId, row);
            dishOrder << dishId;
        }
        byDish[dishId].prices[menuId] = dr.value(i, "f_price");
    }

    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "f_dish_id", tr("Code"));
    rg->fModel->setColumn(120, "f_group", tr("Group"));
    rg->fModel->setColumn(220, "f_dish", tr("Dish"));
    rg->fModel->setColumn(140, "f_store", tr("Store"));
    rg->fModel->setColumn(120, "f_print", tr("Service receipt printer"));
    for (int i = 0; i < menuIdList.count(); i++) {
        rg->fModel->setColumn(80, QString("m_%1").arg(menuIdList.at(i)), menuTitles.at(i));
    }

    QList<QList<QVariant>> rows;
    for (int dishId : dishOrder) {
        const DishRow &d = byDish[dishId];
        QList<QVariant> row;
        row << dishId << d.group << d.dish << d.store << d.print;
        for (int menuId : menuIdList) {
            row << d.prices.value(menuId, QVariant());
        }
        rows << row;
    }

    rg->fModel->setSqlQuery(QString());
    rg->fModel->setDataFromSource(rows);
}
