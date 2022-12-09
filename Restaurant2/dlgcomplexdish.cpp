#include "dlgcomplexdish.h"
#include "ui_dlgcomplexdish.h"
#include <QScrollBar>

DlgComplexDish::DlgComplexDish(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgComplexDish)
{
    ui->setupUi(this);
    fMode = 0;
    show();
    qApp->processEvents();
}

DlgComplexDish::~DlgComplexDish()
{
    delete ui;
}

DishComplexStruct *DlgComplexDish::complex(QWidget *parent)
{
    DishComplexStruct *dc = 0;
    DlgComplexDish *d = new DlgComplexDish(parent);
    d->setup();
    if (d->exec() == QDialog::Accepted) {
        dc = new DishComplexStruct();
        *dc = *(d->fResult);
    }
    delete d;
    return dc;
}

void DlgComplexDish::on_btnCancel_clicked()
{
    reject();
}

void DlgComplexDish::setup()
{
    fMode = 0;
    ui->btnOk->setEnabled(false);
    ui->btnBack->setEnabled(false);
    ui->tblData->clear();
    Utils::setupTableFullColumnWidth(ui->tblData, 200, DishesTable::fDishComplex.count());
    int row = 0;
    int col = 0;
    QListIterator<DishComplexStruct*> it(DishesTable::fDishComplex);
    while (it.hasNext()) {
        DishComplexStruct *dc = it.next();
        QTableWidgetItem *item = new QTableWidgetItem(dc->fName[def_lang] + " [" +  float_str(dc->fPrice, 2) + "]");
        item->setData(Qt::UserRole, qVariantFromValue(dc));
        ui->tblData->setItem(row, col++, item);
        if (col >= ui->tblData->columnCount()) {
            col = 0;
            row++;
        }
    }
    ui->tblData->setSelectionMode(QAbstractItemView::SingleSelection);
}

void DlgComplexDish::setupDishes()
{
    fMode = 1;
    ui->btnBack->setEnabled(true);
    ui->tblData->clear();
    Utils::setupTableFullColumnWidth(ui->tblData, 200, fResult->fDishes.count());
    int row = 0;
    int col = 0;
    QListIterator<DishStruct*> it(fResult->fDishes);
    while (it.hasNext()) {
        DishStruct *ds = it.next();
        ui->tblData->setItem(row, col++, Utils::tableItem(ds->fName, qVariantFromValue(ds)));
        if (col >= ui->tblData->columnCount()) {
            col = 0;
            row++;
        }
    }
    ui->tblData->setSelectionMode(QAbstractItemView::MultiSelection);
}

void DlgComplexDish::on_tblData_clicked(const QModelIndex &index)
{
    ui->btnOk->setEnabled(false);
    if (!index.isValid()) {
        fResult = 0;
        return;
    }
    switch (fMode) {
    case 0:
        fResult = index.data(Qt::UserRole).value<DishComplexStruct*>();
        if (!fResult) {
            return;
        }
        ui->lbTitle->setText(QString("%1 %2")
                             .arg(tr("Select dishes for "))
                             .arg(fResult->fName[def_lang]));
        setupDishes();
        ui->btnSelectAll->setEnabled(true);
        break;
    case 1:
        if (ui->tblData->selectionModel()->selectedIndexes().count() > 0) {
            ui->btnOk->setEnabled(true);
        }
        ui->btnSelectAll->setEnabled(false);
        break;
    }
}

void DlgComplexDish::on_btnOk_clicked()
{
    QModelIndexList sel = ui->tblData->selectionModel()->selectedIndexes();
    if (sel.count() == 0) {
        return;
    }
    for (int i = 0; i < sel.count(); i++) {
        if (sel.at(i).isValid()) {
            DishStruct *d = sel.at(i).data(Qt::UserRole).value<DishStruct*>();
            if (d) {
                fDishes.append(d);
            }
        }
    }
    if (fDishes.count() > 0) {
        DishComplexStruct *dc = new DishComplexStruct();
        *dc = *fResult;
        dc->fDishes = fDishes;
        fResult = dc;
        accept();
    }
}

void DlgComplexDish::on_btnBack_clicked()
{
    if (fMode == 1) {
        setup();
    }
}

void DlgComplexDish::on_btnUp_clicked()
{
    ui->tblData->verticalScrollBar()->setValue(ui->tblData->verticalScrollBar()->value() - 6);
}

void DlgComplexDish::on_btnDown_clicked()
{
    ui->tblData->verticalScrollBar()->setValue(ui->tblData->verticalScrollBar()->value() + 6);
}

void DlgComplexDish::on_btnSelectAll_clicked()
{
    ui->tblData->selectAll();
    on_btnOk_clicked();
}
