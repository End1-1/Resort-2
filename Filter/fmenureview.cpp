#include "fmenureview.h"
#include "ui_fmenureview.h"
#include "wreportgrid.h"
#include "dwselectorrestmenu.h"

FMenuReview::FMenuReview(QWidget *parent)
    : WFilterBase(parent), ui(new Ui::FMenuReview)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Menu review"), ":/images/notepad.png");
    DWSelectorRestMenu *dockStore = new DWSelectorRestMenu(this);
    dockStore->configure();
    dockStore->setSelector(ui->leMenu);
    connect(dockStore, &DWSelectorRestMenu::menu, [this](CI_RestMenu * ci) {
        dockResponse<CI_RestMenu, CacheRestMenu>(ui->leMenu, ci);

        if(ci) {
            fMenu = ci->fCode;
        }
    });
}

FMenuReview::~FMenuReview() { delete ui; }

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
    if(fMenu.isEmpty()) {
        return;
    }

    QString where = "and f_menu in (" + fMenu + ") ";
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

    for(int i = 0; i < rg->fModel->rowCount(); i++) {
        if(i == 0) {
            continue;
        }

        if(rg->fModel->data(i, 0).toInt() != rg->fModel->data(i - 1, 0).toInt()) {
            rg->fModel->insertRow(i, vals);
            i++;
        }
    }
}
