#include "rerestdishcomplex.h"
#include "ui_rerestdishcomplex.h"

RERestDishComplex::RERestDishComplex(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_DISH_COMPLEX, parent),
    ui(new Ui::RERestDishComplex)
{
    ui->setupUi(this);
    ui->leMenuCode->setVisible(false);
    ui->leDishCode->setVisible(false);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leNameEn, "Name, en");
    addWidget(ui->leNameRu, "Name, ru");
    addWidget(ui->teStart, "Start time");
    addWidget(ui->teEnd, "End time");
    addWidget(ui->lePrice, "Price");
    addWidget(ui->lePriceDeviation, "Price deviation");
    addWidget(ui->leAdgt, "ADGT");
    addWidget(ui->chEnabled, "Enabled");
    ui->lePrice->setValidator(new QDoubleValidator(0, 0, 2));
    ui->lePriceDeviation->setValidator(new QIntValidator());
    fTable = "r_dish_complex";
    Utils::tableSetColumnWidths(ui->tblDishes, ui->tblDishes->columnCount(),
                                0, 0, 150, 200, 80, 30);
    fDockMenu = new DWSelectorRestMenu(this);
    fDockMenu->configure();
    fDockMenu->setSelector(ui->leMenu);
    connect(fDockMenu, SIGNAL(menu(CI_RestMenu*)), this, SLOT(menu(CI_RestMenu*)));
    fDockDish = new DESelectorMenuDishes(this);
    fDockDish->configure();
    fDockDish->setSelector(ui->leDishName);
    connect(fDockDish, SIGNAL(menuDish(CI_MenuDishes*)), this, SLOT(dish(CI_MenuDishes*)));
}

RERestDishComplex::~RERestDishComplex()
{
    delete ui;
}

void RERestDishComplex::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    if (!isNew) {
        fDbBind[":f_complex"] = ui->leCode->asInt();
        fDb.select("select rc.f_id, rc.f_dish, mn.f_" + def_lang + ", d.f_" + def_lang + ", m.f_price "
                   "from r_dish_complex_list rc "
                   "inner join r_menu m on m.f_id=rc.f_dish "
                   "inner join r_menu_names mn on mn.f_id=m.f_menu "
                   "inner join r_dish d on d.f_id=m.f_dish "
                   "where rc.f_complex=:f_complex", fDbBind, fDbRows);
        int row = 0;
        foreach_rows {
            ui->tblDishes->setRowCount(row + 1);
            ui->tblDishes->setRowCount(row + 1);
            ui->tblDishes->setItem(row, 0, Utils::tableItem(it->at(0)));
            ui->tblDishes->setItem(row, 1, Utils::tableItem(it->at(1)));
            ui->tblDishes->setItem(row, 2, Utils::tableItem(it->at(2)));
            ui->tblDishes->setItem(row, 3, Utils::tableItem(it->at(3)));
            ui->tblDishes->setItem(row, 4, Utils::tableItem(it->at(4)));
            ui->tblDishes->addButton(row, 5, SLOT(removeRow(int)), this, QIcon(":/images/garbage.png"));
            row++;
        }
    }
}

void RERestDishComplex::clearWidgets()
{
    RowEditorDialog::clearWidgets();
    ui->tblDishes->clearContents();
    ui->tblDishes->setRowCount(0);
    ui->leMenu->clear();
    ui->leMenuCode->clear();
    ui->leDishCode->clear();
    ui->leDishName->clear();
    ui->leDishesTotal->setText("0");
}

void RERestDishComplex::menu(CI_RestMenu *c)
{
    if (c) {
        ui->leMenuCode->setText(c->fCode);
        ui->leMenu->setText(c->fName);
        fDockDish->setMenu(c->fCode);
    }
}

void RERestDishComplex::dish(CI_MenuDishes *c)
{
    if (c) {
        ui->leDishCode->setText(c->fCode);
        ui->leDishName->setText(c->fName);
    }
}

void RERestDishComplex::removeRow(int row)
{
    EPushButton *btn = static_cast<EPushButton*>(sender());
    row = -1;
    for (int i = 0; i < ui->tblDishes->rowCount(); i++) {
        if (btn == ui->tblDishes->cellWidget(i, 5)) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    if (message_confirm_tr("Confirm to delete selected row") != QDialog::Accepted) {
        return;
    }
    if (ui->tblDishes->toInt(row, 0) > 0) {
        fDbBind[":f_dish"] = ui->tblDishes->toInt(row, 1);
        fDbBind[":f_complex"] = ui->leCode->asInt();
        fDb.select("delete from r_dish_complex_list where f_complex=:f_complex and f_dish=:f_dish", fDbBind, fDbRows);
        fTrackControl->insert("Remove dish", QString("%1 / %2")
                                  .arg(ui->tblDishes->toString(row, 3))
                                  .arg(ui->tblDishes->toString(row, 4)), "");
    }
    ui->tblDishes->removeRow(row);
}

void RERestDishComplex::on_btnCancel_clicked()
{
    reject();
}

void RERestDishComplex::on_btnOK_clicked()
{
    save();
    for (int i = 0; i < ui->tblDishes->rowCount(); i++) {
        if (ui->tblDishes->toInt(i, 0) == 0) {
            fDbBind[":f_complex"] = ui->leCode->asInt();
            fDbBind[":f_dish"] = ui->tblDishes->toInt(i, 1);
            fDb.insert("r_dish_complex_list", fDbBind);
            fTrackControl->insert("Append dish", QString("%1 / %2")
                                      .arg(ui->tblDishes->toString(i, 3))
                                      .arg(ui->tblDishes->toString(i, 4)), "");
        }
    }
    fTrackControl->saveChanges();
}

void RERestDishComplex::on_btnAdd_clicked()
{
    CI_MenuDishes *c = CacheMenuDishes::instance()->get(ui->leDishCode->text());
    if (!c) {
        return;
    }
    int row = ui->tblDishes->rowCount();
    ui->tblDishes->setRowCount(row + 1);
    ui->tblDishes->setItem(row, 0, new QTableWidgetItem("0"));
    ui->tblDishes->setItem(row, 1, Utils::tableItem(c->fCode));
    ui->tblDishes->setItem(row, 2, Utils::tableItem(c->fMenu));
    ui->tblDishes->setItem(row, 3, Utils::tableItem(c->fName));
    ui->tblDishes->setItem(row, 4, Utils::tableItem(c->fPrice));
    ui->tblDishes->addButton(row, 5, SLOT(removeRow(int)), this, QIcon(":/images/garbage.png"));
    countTotal();
}

void RERestDishComplex::countTotal()
{
    double total = 0;
    for (int i = 0; i < ui->tblDishes->rowCount(); i++) {
        total += ui->tblDishes->toDouble(i, 4);
    }
    ui->leDishesTotal->setDouble(total);
}
