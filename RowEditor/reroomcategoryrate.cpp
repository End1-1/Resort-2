#include "reroomcategoryrate.h"
#include "ui_reroomcategoryrate.h"
#include "reroomcategoryraterow.h"
#include "wreportgrid.h"

RERoomCategoryRate::RERoomCategoryRate(WReportGrid *parent) :
    RECustomAction(parent),
    ui(new Ui::RERoomCategoryRate)
{
    ui->setupUi(this);
    setup(":/images/price-tag.png", tr("Prices"));
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 80, 80, 80, 80, 80, 0, 80);
}

RERoomCategoryRate::~RERoomCategoryRate()
{
    delete ui;
}

void RERoomCategoryRate::handleValues(const QList<QVariant> &out)
{
    if (out.count() == 0) {
        return;
    }
    fCategory = out.at(0).toInt();
    ui->leCategory->setText(out.at(1).toString());
    ui->leCategoryName->setText(out.at(2).toString());
    load();
    exec();
}

void RERoomCategoryRate::on_btnCancel_clicked()
{
    reject();
}

void RERoomCategoryRate::on_btnOk_clicked()
{
    accept();
}

void RERoomCategoryRate::on_btnAddRate_clicked()
{
    QList<QVariant> values;
    RERoomCategoryRateRow *r = new RERoomCategoryRateRow(fCategory, values);
    r->valuesToWidgets();
    if (r->exec() == QDialog::Accepted) {
        load();
    }
    delete r;
}

void RERoomCategoryRate::load()
{
    fDbBind[":f_category"] = fCategory;
    fDb.select("select r.f_id, f_month1, f_day1, f_month2, f_day2, f_bed, b.f_name, f_rate from "
               "f_room_rate r "
               "left join f_room_bed b on b.f_id=r.f_bed "
               "where f_category=:f_category", fDbBind, fDbRows);
    ui->tblData->setRowCount(fDbRows.count());
    Utils::fillTableWithData(ui->tblData, fDbRows);
}


void RERoomCategoryRate::on_btnRemoveRate_clicked()
{
    QModelIndexList sel = ui->tblData->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    if (message_confirm_tr("Are you sure to delete selected row") != QDialog::Accepted) {
        return;
    }
    TrackControl t(TRACK_ROOM_CATEGORY_RATE);
    t.insert("Rate removed", ui->tblData->item(sel.at(0).row(), 0)->data(Qt::EditRole).toString(), "");
    fDbBind[":f_id"] = ui->tblData->item(sel.at(0).row(), 0)->data(Qt::EditRole).toInt();
    fDb.select("delete from f_room_rate where f_id=:f_id", fDbBind, fDbRows);
    ui->tblData->removeRow(sel.at(0).row());
}

void RERoomCategoryRate::on_btnEditRate_clicked()
{
    QModelIndexList sel = ui->tblData->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    QList<QVariant> values;
    values << ui->tblData->item(sel.at(0).row(), 0)->data(Qt::EditRole).toInt()
           << fCategory
           << ui->tblData->item(sel.at(0).row(), 1)->data(Qt::EditRole)
           << ui->tblData->item(sel.at(0).row(), 2)->data(Qt::EditRole)
           << ui->tblData->item(sel.at(0).row(), 3)->data(Qt::EditRole)
           << ui->tblData->item(sel.at(0).row(), 4)->data(Qt::EditRole)
           << ui->tblData->item(sel.at(0).row(), 5)->data(Qt::EditRole)
           << ui->tblData->item(sel.at(0).row(), 6)->data(Qt::EditRole)
           << ui->tblData->item(sel.at(0).row(), 7)->data(Qt::EditRole);
    RERoomCategoryRateRow *r = new RERoomCategoryRateRow(fCategory, values);
    r->valuesToWidgets();
    if (r->exec() == QDialog::Accepted) {
        load();
    }
    delete r;
}
