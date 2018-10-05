#include "wreportbuilder.h"
#include "ui_wreportbuilder.h"
#include "dlgreportbuildergroups.h"
#include "ecomboboxcompleter.h"

WReportBuilder::WReportBuilder(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WReportBuilder)
{
    ui->setupUi(this);
    fDb.select("select f_id, f_name from serv_reports order by f_name", fDbBind, fDbRows);
    foreach_rows {
        QListWidgetItem *item = new QListWidgetItem(ui->lwNames);
        item->setText(it->at(1).toString());
        item->setData(Qt::UserRole, it->at(0));
        ui->lwNames->addItem(item);
    }
}

WReportBuilder::~WReportBuilder()
{
    delete ui;
}

void WReportBuilder::setup()
{
    setupTabTextAndIcon(tr("Report builder"), ":/images/reports.png");
}

void WReportBuilder::on_lwNames_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->leColField->fRow = -1;
    ui->leColTitle->fRow = -1;
    ui->leColWidth->fRow = -1;
    ui->lwCols->clear();
    ui->leColField->clear();
    ui->leColTitle->clear();
    ui->leColWidth->clear();    
    clearFilterFields();
    ui->lwFilters->clear();
    fDbBind[":f_id"] = index.data(Qt::UserRole);
    fDb.select("select f_name, f_group, f_groupAccess, f_sql, f_widths, f_titles_en, "
               "f_sum, f_filter, f_fields, f_menu, f_subtotal "
               "from serv_reports where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leCode->setText(index.data(Qt::UserRole).toString());
        ui->leName->setText(fDbRows.at(0).at(0).toString());
        ui->cbMenu->setCurrentIndex(ui->cbMenu->findData(fDbRows.at(0).at(1)));
        ui->leGroupAccess->setText(fDbRows.at(0).at(2).toString());
        ui->teSQL->setPlainText(fDbRows.at(0).at(3).toString());
        ui->leSum->setText(fDbRows.at(0).at(6).toString());
        ui->cbMainMenu->setCurrentIndex(fDbRows.at(0).at(9).toInt());
        QStringList fields = fDbRows.at(0).at(8).toString().split(";", QString::SkipEmptyParts);
        QStringList titles = fDbRows.at(0).at(5).toString().split(";", QString::SkipEmptyParts);
        QStringList widths = fDbRows.at(0).at(4).toString().split(";", QString::SkipEmptyParts);
        QStringList filter = fDbRows.at(0).at(7).toString().split(";", QString::SkipEmptyParts);
        ui->leSubtotals->setText(fDbRows.at(0).at(10).toString());
        for (int i = 0; i < fields.count(); i++) {
            QListWidgetItem *item = new QListWidgetItem(ui->lwCols);
            item->setText(fields.at(i).trimmed());
            item->setData(Qt::UserRole, titles.at(i));
            item->setData(Qt::UserRole + 1, widths.at(i));
            ui->lwCols->addItem(item);
        }
        for (int i = 0; i < filter.count(); i++) {
             QStringList f = filter.at(i).split("^", QString::KeepEmptyParts);
             QListWidgetItem *item = new QListWidgetItem(ui->lwFilters);
             item->setText(f.at(1));
             item->setData(Qt::UserRole, f.at(0)); //field
             item->setData(Qt::UserRole + 1, f.at(2)); // data type
             item->setData(Qt::UserRole + 2, f.at(3)); //buid
             item->setData(Qt::UserRole + 3, f.at(4)); // mark
             item->setData(Qt::UserRole + 4, f.at(5)); //default expr
             ui->lwFilters->addItem(item);
        }
    }
}

void WReportBuilder::on_lwCols_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->leColTitle->fRow = -1;
    ui->leColWidth->fRow = -1;
    ui->leColField->fRow = -1;
    ui->leColField->setText(index.data(Qt::DisplayRole).toString());
    ui->leColTitle->setText(index.data(Qt::UserRole).toString());
    ui->leColWidth->setText(index.data(Qt::UserRole + 1).toString());
    ui->leColField->fRow = index.row();
    ui->leColTitle->fRow = index.row();
    ui->leColWidth->fRow = index.row();
}

void WReportBuilder::on_btnNew_clicked()
{
    ui->leCode->clear();
    ui->leName->clear();
    ui->leGroupAccess->clear();
    ui->leColField->clear();
    ui->leColTitle->clear();
    ui->leColWidth->clear();
    ui->lwCols->clear();
    ui->leSum->clear();
    ui->teSQL->clear();
    ui->leFilterField->clear();
    ui->leFilterMark->clear();
    ui->leFilterTitle->clear();
    ui->leFilterBuild->clear();
    ui->leColTitle->fRow = -1;
    ui->leColWidth->fRow = -1;
    ui->leColField->fRow = -1;
    ui->leFilterField->fRow = -1;
    ui->leFilterMark->fRow = -1;
    ui->leFilterTitle->fRow = -1;
    ui->leFilterBuild->fRow = -1;
}

void WReportBuilder::on_btnSave_clicked()
{
    QString titles;
    QString widths;
    QString filter;
    QString fields;
    for (int i = 0; i < ui->lwCols->count(); i++) {
        fields += ui->lwCols->item(i)->text() + ";";
        titles += ui->lwCols->item(i)->data(Qt::UserRole).toString() + ";";
        widths += ui->lwCols->item(i)->data(Qt::UserRole + 1).toString() + ";";
    }
    for (int i = 0; i < ui->lwFilters->count(); i++) {
        filter += ui->lwFilters->item(i)->data(Qt::UserRole).toString() + "^";
        filter += ui->lwFilters->item(i)->text() + "^";
        filter += ui->lwFilters->item(i)->data(Qt::UserRole + 1).toString() + "^";
        filter += ui->lwFilters->item(i)->data(Qt::UserRole + 2).toString() + "^";
        filter += ui->lwFilters->item(i)->data(Qt::UserRole + 3).toString() + "^";
        filter += ui->lwFilters->item(i)->data(Qt::UserRole + 4).toString() + "^";
        filter += ";";
    }
    fDbBind[":f_name"] = ui->leName->text();
    fDbBind[":f_group"] = ui->cbMenu->currentData();
    fDbBind[":f_groupAccess"] = ui->leGroupAccess->text();
    fDbBind[":f_menu"] = ui->cbMainMenu->currentIndex();
    fDbBind[":f_sql"] = ui->teSQL->toPlainText();
    fDbBind[":f_fields"] = fields;
    fDbBind[":f_titles_en"] = titles;
    fDbBind[":f_widths"] = widths;
    fDbBind[":f_sum"] = ui->leSum->text();
    fDbBind[":f_filter"] = filter;
    fDbBind[":f_subtotal"] = ui->leSubtotals->text();
    if (ui->leCode->asInt() == 0) {
        ui->leCode->setInt(fDb.insert("serv_reports", fDbBind));
        QListWidgetItem *item = new QListWidgetItem(ui->lwNames);
        item->setText(ui->leName->text());
        item->setData(Qt::UserRole, ui->leCode->asInt());
        ui->lwNames->addItem(item);
    } else {
        fDb.update("serv_reports", fDbBind, where_id(ui->leCode->asInt()));
        for (int i = 0; i < ui->lwNames->count(); i++) {
            if (ui->lwNames->item(i)->data(Qt::UserRole).toInt() == ui->leCode->asInt()) {
                ui->lwNames->item(i)->setText(ui->leName->text());
                break;
            }
        }
    }
    message_info_tr("Saved");
}

void WReportBuilder::on_btnExtractCols_clicked()
{
    if (ui->leCode->asInt() > 0) {
        if (message_confirm_tr("Reset colums?") != QDialog::Accepted) {
            return;
        }
    }
    ui->lwCols->clear();
    QString sql = ui->teSQL->toPlainText();
    QStringList fields = sql.mid(7, sql.indexOf("from") - 7).split(",", QString::SkipEmptyParts);
    for (int i = 0; i < fields.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem(ui->lwCols);
        item->setText(fields.at(i).trimmed());
        item->setData(Qt::UserRole, fields.at(i).trimmed());
        item->setData(Qt::UserRole + 1, "80");
        ui->lwCols->addItem(item);
    }
}

void WReportBuilder::on_btnRemove_clicked()
{
    if (ui->leCode->asInt() == 0) {
        return;
    }
    if (message_confirm(tr("Confirm to remove ") + ui->leName->text()) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] = ui->leCode->asInt();
    fDb.select("delete from serv_reports where f_id=:f_id", fDbBind, fDbRows);
    for (int i = 0; i < ui->lwNames->count(); i++) {
        if (ui->lwNames->item(i)->data(Qt::UserRole).toInt() == ui->leCode->asInt()) {
            delete ui->lwNames->item(i);
            on_btnNew_clicked();
            break;
        }
    }
}

void WReportBuilder::on_leColTitle_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    if (ui->lwCols->count() == 0) {
        return;
    }
    ui->lwCols->item(l->fRow)->setData(Qt::UserRole, arg1.toUpper());
}

void WReportBuilder::on_leColWidth_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    if (ui->lwCols->count() == 0) {
        return;
    }
    ui->lwCols->item(l->fRow)->setData(Qt::UserRole + 1, arg1);
}

void WReportBuilder::on_lwFilters_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->leFilterExprEmpty->fRow = -1;
    ui->leFilterField->fRow = -1;
    ui->leFilterMark->fRow = -1;
    ui->leFilterTitle->fRow = -1;
    ui->leFilterBuild->fRow = -1;
    ui->cbFilterType->fRow = -1;
    ui->leFilterField->setText(index.data(Qt::UserRole).toString());
    ui->leFilterTitle->setText(index.data(Qt::DisplayRole).toString());
    ui->leFilterMark->setText(index.data(Qt::UserRole + 3).toString());
    ui->cbFilterType->setCurrentIndex(ui->cbFilterType->findText(index.data(Qt::UserRole + 1).toString()));
    ui->leFilterBuild->setText(index.data(Qt::UserRole + 2).toString());
    ui->leFilterExprEmpty->setText(index.data(Qt::UserRole + 4).toString());
    ui->leFilterField->fRow = index.row();
    ui->leFilterMark->fRow = index.row();
    ui->leFilterTitle->fRow = index.row();
    ui->leFilterBuild->fRow = index.row();
    ui->cbFilterType->fRow = index.row();
    ui->leFilterExprEmpty->fRow = index.row();
}

void WReportBuilder::on_leFilterField_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    ui->lwFilters->item(l->fRow)->setData(Qt::UserRole, arg1);
}

void WReportBuilder::on_leFilterTitle_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    ui->lwFilters->item(l->fRow)->setText(arg1);
}

void WReportBuilder::on_leFilterMark_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    ui->lwFilters->item(l->fRow)->setData(Qt::UserRole + 3, arg1);
}

void WReportBuilder::on_cbFilterType_currentIndexChanged(const QString &arg1)
{
    EQComboBox *c = static_cast<EQComboBox*>(sender());
    if (c->fRow < 0) {
        return;
    }
    ui->lwFilters->item(c->fRow)->setData(Qt::UserRole + 1, arg1);
}

void WReportBuilder::on_btnAddFilter_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(ui->lwFilters);
    if (ui->leFilterTitle->text().isEmpty()) {
        ui->leFilterTitle->setText("New filter");
    }
    item->setText(ui->leFilterTitle->text());
    item->setData(Qt::UserRole, ui->leFilterField->text()); //field
    item->setData(Qt::UserRole + 1, ui->cbFilterType->currentText()); // data type
    item->setData(Qt::UserRole + 2, ui->leFilterBuild->text()); //buid
    item->setData(Qt::UserRole + 3, ui->leFilterMark->text()); // mark
    ui->lwFilters->addItem(item);
    clearFilterFields();
}

void WReportBuilder::on_btnRemoveFilter_clicked()
{
    int row = ui->lwFilters->currentRow();
    if (row < 0) {
        return;
    }
    clearFilterFields();
    delete ui->lwFilters->item(row);
}

void WReportBuilder::clearFilterFields()
{
    ui->leFilterField->fRow = -1;
    ui->leFilterMark->fRow = -1;
    ui->leFilterTitle->fRow = -1;
    ui->leFilterBuild->fRow = -1;
    ui->leFilterExprEmpty->fRow = -1;
    ui->leFilterField->clear();
    ui->leFilterMark->clear();
    ui->leFilterTitle->clear();
    ui->leFilterBuild->clear();
    ui->leFilterExprEmpty->clear();
}

void WReportBuilder::on_btnEditGrup_clicked()
{
    DlgReportBuilderGroups *d = new DlgReportBuilderGroups(this);
    d->exec();
    delete d;
    ui->cbMenu->refreshData();
}

void WReportBuilder::on_leFilterBuild_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    ui->lwFilters->item(l->fRow)->setData(Qt::UserRole + 2, arg1);
}

void WReportBuilder::on_btnAddFilter_2_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(ui->lwCols);
    item->setText("New field");
    item->setData(Qt::UserRole, "New field");
    item->setData(Qt::UserRole + 1, 80);
    ui->lwCols->addItem(item);
}

void WReportBuilder::on_btnRemoveFilter_2_clicked()
{
    QListWidgetItem *item = ui->lwCols->currentItem();
    if (item) {
        ui->leColField->fRow = -1;
        ui->leColTitle->fRow = -1;
        ui->leColWidth->fRow = -1;
        ui->leColField->clear();
        ui->leColTitle->clear();
        ui->leColWidth->clear();
        ui->lwCols->removeItemWidget(item);
    }
}

void WReportBuilder::on_leColField_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    ui->lwCols->item(l->fRow)->setData(Qt::DisplayRole, arg1);
}

void WReportBuilder::on_leFilterExprEmpty_textChanged(const QString &arg1)
{
    EQLineEdit *l = static_cast<EQLineEdit*>(sender());
    if (l->fRow < 0) {
        return;
    }
    ui->lwFilters->item(l->fRow)->setData(Qt::UserRole + 4, arg1);
}
