#include "fcategorytosell.h"
#include "ui_fcategorytosell.h"
#include "wreportgrid.h"

FCategoryToSell::FCategoryToSell(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCategoryToSell)
{
    ui->setupUi(this);
}

FCategoryToSell::~FCategoryToSell()
{
    delete ui;
}

void FCategoryToSell::setup()
{
    fReportGrid->setupTabTextAndIcon(tr("Category to sell"), ":/images/app.png");
}

void FCategoryToSell::apply(WReportGrid *rg)
{
    QStringList querySet;
    querySet << QString("call category_to_cell(%1, %2)").arg(ui->deStart->dateMySql()).arg(ui->deEnd->dateMySql());
    QString query = "select ";
    if (ui->chGroupByView->isChecked()) {
        query += "r.f_view,";
    }
    if (ui->chGroupByBed->isChecked()) {
        query += "r.f_bed,";
    }
    query += "  r.f_cat, r.f_description, \
            sum(f_total), sum(f_conf), sum(f_guar), sum(f_tent), \
            sum(f_wait), sum(f_block), sum(f_check), sum(f_tosell) \
            from rep r \
            group by 1, 2";
    if (ui->chGroupByView->isChecked()) {
        query += ",3";
    }
    if (ui->chGroupByBed->isChecked()) {
        query += ",4";
    }
    querySet << query;
    rg->fModel->clearColumns();
    if (ui->chGroupByView->isChecked()) {
        rg->fModel->setColumn(80, "", tr("View"));
    }
    if (ui->chGroupByBed->isChecked()) {
        rg->fModel->setColumn(80, "", tr("Bed"));
    }
    rg->fModel->setColumn(80, "", tr("Cat."))
            .setColumn(200, "", tr("Description"));
    rg->fModel->setColumn(80, "", tr("Total"))
            .setColumn(80, "", tr("Confirmed"))
            .setColumn(80, "", tr("Guaranted"))
            .setColumn(80, "", tr("Tentative"))
            .setColumn(80, "", tr("Waiting"))
            .setColumn(80, "", tr("Block"))
            .setColumn(80, "", tr("CheckIn"))
            .setColumn(80, "", tr("Still to sell"));
    rg->fModel->apply(querySet);
    int off = 0;
    if (ui->chGroupByBed->isChecked()) {
        off ++;
    }
    if (ui->chGroupByView->isChecked()) {
        off++;
    }
    QList<int> cols;
    cols << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    for (int i = 0; i < cols.count(); i++) {
        cols[i] = cols[i] + off;
    }
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

QWidget *FCategoryToSell::firstElement()
{
    return ui->deStart;
}
