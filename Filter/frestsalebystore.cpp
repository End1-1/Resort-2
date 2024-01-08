#include "frestsalebystore.h"
#include "ui_frestsalebystore.h"
#include "wreportgrid.h"
#include "dlggetidname.h"

FRestSaleByStore::FRestSaleByStore(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FRestSaleByStore)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Sale by store"), ":/images/category.png");
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FRestSaleByStore::branchEditDoubleClick);
    connect(ui->leHall, &EQLineEdit::customButtonClicked, this, &FRestSaleByStore::hallEditDoubleClick);
}

FRestSaleByStore::~FRestSaleByStore()
{
    delete ui;
}

QWidget *FRestSaleByStore::firstElement()
{
    return ui->leDateStart;
}

QString FRestSaleByStore::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Sale report by store"))
            .arg(ui->leDateStart->text())
            .arg(ui->leDateEnd->text());
}

void FRestSaleByStore::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    if (ui->chShowDates->isChecked()) {
        rg->fModel->setColumn(120, "", tr("Date"));
    }
    rg->fModel->setColumn(200, "", tr("Store"));
    if (ui->chShowItems->isChecked()) {
        rg->fModel->setColumn(200, "", tr("Dish"))
                .setColumn(100, "", tr("Qty"));
    }
    rg->fModel->setColumn(100, "", tr("Amount"));
    QString sql = "select ";
    if (ui->chShowDates->isChecked()) {
        sql += " h.f_dateCash, ";
    }
    sql += " s.f_name as store, ";
    if (ui->chShowItems->isChecked()) {
        sql += " dish.f_en, sum(d.f_qty), sum(d.f_total) ";
    } else {
        sql += " sum(d.f_total) ";
    }
    QString group = " group by s.f_name ";
    if (ui->chShowDates->isChecked()) {
        group += ",h.f_dateCash ";
    }
    QString where;
    where += ui->leBranch->fHiddenText.isEmpty() ? "" : QString(" and h.f_branch in (%1) ").arg(ui->leBranch->fHiddenText);
    where += ui->leHall->fHiddenText.isEmpty() ? "" : QString(" and h.f_hall in (%1) ").arg(ui->leHall->fHiddenText);
    if (ui->chShowItems->isChecked()) {
        sql += " from o_dish d "
                "left join o_header h on d.f_header=h.f_id "
                "left join r_dish dish on dish.f_id=d.f_dish "
                "left join r_store s on s.f_id=d.f_store "
                "where h.f_state=2 and d.f_state=1 " + where +
                "and h.f_dateCash between " + ui->leDateStart->dateMySql() + " and " + ui->leDateEnd->dateMySql();
        if (ui->chShowItems->isChecked()) {
            group += ",dish.f_en ";
        }
    } else {
        sql += " from o_dish d "
                "left join o_header h on d.f_header=h.f_id "
                "left join r_dish dish on dish.f_id=d.f_dish "
                "left join r_store s on s.f_id=d.f_store "
                "where h.f_state=2 and d.f_state=1 "
                "and h.f_dateCash between " + ui->leDateStart->dateMySql() + " and " + ui->leDateEnd->dateMySql() + where;
    }
    sql += group;
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    int colSum = 1;
    int colQty = -1;
    if (ui->chShowDates->isChecked()) {
        colSum++;
    }
    if (ui->chShowItems->isChecked()) {
        colSum += 2;
        colQty = 2;
        if (ui->chShowDates->isChecked()) {
            colQty++;
        }
    }
    if (colQty > -1) {
        cols << colQty;
    }
    cols << colSum;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FRestSaleByStore::on_btnNext_clicked()
{
    ui->leDateStart->setDate(ui->leDateStart->date().addDays(1));
    ui->leDateEnd->setDate(ui->leDateEnd->date().addDays(1));
    apply(fReportGrid);
}

void FRestSaleByStore::on_btnBack_clicked()
{
    ui->leDateStart->setDate(ui->leDateStart->date().addDays(-1));
    ui->leDateEnd->setDate(ui->leDateEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FRestSaleByStore::branchEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}

void FRestSaleByStore::hallEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_hall, this)) {
        ui->leHall->setText(name);
        ui->leHall->fHiddenText = id;
    }
}
