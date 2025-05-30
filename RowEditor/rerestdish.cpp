#include "rerestdish.h"
#include "ui_rerestdish.h"
#include "dlgtracking.h"
#include "eqcheckbox.h"
#include "dwselectorunit.h"
#include "cachedish.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QBuffer>

#define SEL_DISH 1
#define SEL_UNIT 2

RERestDish::RERestDish(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_DISH, parent),
    ui(new Ui::RERestDish)
{
    ui->setupUi(this);
    ui->leTextColor->setText("-16777216");
    addWidget(ui->leCode, "Code");
    addWidget(ui->leDishPart, "dishpart");
    addWidget(ui->leDefStore, "Store code");
    addWidget(ui->leDefStoreName, "Store name");
    addWidget(ui->leTypeCode, "");
    addWidget(ui->leTypeName, "Type");
    addWidget(ui->leNameAm, "Name");
    addWidget(ui->leArmSoftName, "ՀԾ անվանում");
    addWidget(ui->teAm, "Text");
    addWidget(ui->leBgColor, "Backgroun color");
    addWidget(ui->leTextColor, "Text color");
    addWidget(ui->leQueue, "Queue");
    addWidget(ui->leADGT, "ADGT");
    addWidget(ui->leArmSoft, "ArmSoft code");
    addWidget(ui->leLastPrice, "Last input price");
    addWidget(ui->leUnitCode, "Unit code");
    addWidget(ui->leUnitName, "Unit name");
    addWidget(ui->leMinReminder, "Min reminder");
    addWidget(ui->leFiscalDebt, "Fiscal debt");
    /*
    addWidget(ui->leDefStore, "Def store name");
    addWidget(ui->leDefStoreName, "Def store name");*/
    fTable = "r_dish";
    ui->leQueue->setValidator(new QIntValidator());
    Utils::tableSetColumnWidths(ui->tblMenu, ui->tblMenu->columnCount(),
                                30, 0, 0, 100, 100, 100, 100, 0, 100, 50);
    fDb.select("select f_id, f_en from r_menu_names", fDbBind, fDbRows);
    ui->tblMenu->setRowCount(fDbRows.count());
    int row = 0;
    foreach_rows {
        QCheckBox *check = new QCheckBox(this);
        check->setChecked(false);
        ui->tblMenu->setCellWidget(row, 0, check);
        fTrackControl->addWidget(check, "Checkbox of " + it->at(1).toString());
        for (int i = 1, count = ui->tblMenu->columnCount() - 1; i < count; i++)
        {
            EQLineEdit *l = createLineEdit(row, i);
            switch (i) {
                case 2:
                    l->setText(it->at(0).toString());
                    break;
                case 3:
                    l->setText(it->at(1).toString());
                    l->setReadOnly(true);
                    break;
                case 4:
                    fTrackControl->addWidget(l, "Price for " + it->at(1).toString());
                    break;
                case 5:
                    l->setShowButtonOnFocus(true);
                    connect(l, SIGNAL(focusIn()), this, SLOT(printLineEditFocusIn()));
                    connect(l, SIGNAL(focusOut()), this, SLOT(printLineEditFocusOut()));
                    fTrackControl->addWidget(l, "Printer1 for " + it->at(1).toString());
                    break;
                case 6:
                    l->setShowButtonOnFocus(true);
                    connect(l, SIGNAL(focusIn()), this, SLOT(printLineEditFocusIn()));
                    connect(l, SIGNAL(focusOut()), this, SLOT(printLineEditFocusOut()));
                    fTrackControl->addWidget(l, "Printer2 for " + it->at(1).toString());
                    break;
                case 8:
                    l->setShowButtonOnFocus(true);
                    connect(l, SIGNAL(focusIn()), this, SLOT(storeLineEditFocusIn()));
                    connect(l, SIGNAL(focusOut()), this, SLOT(storeLineEditFocusOut()));
                    fTrackControl->addWidget(l, "Store for " + it->at(1).toString());
                    break;
                    break;
            }
        }
        EQCheckBox *checkComplex = new EQCheckBox(this);
        checkComplex->setChecked(false);
        checkComplex->fRow = row;
        checkComplex->fColumn = 9;
        connect(checkComplex, SIGNAL(clicked(bool)), this, SLOT(checkComplex(bool)));
        ui->tblMenu->setCellWidget(row, 9, checkComplex);
        row++;
    }
    fDockPrint = new DWSelectorRestPrinter(this);
    fDockPrint->configure();
    connect(fDockPrint, SIGNAL(printer(CI_RestPrinter *)), this, SLOT(printer(CI_RestPrinter *)));
    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    connect(fDockStore, SIGNAL(store(CI_RestStore *)), this, SLOT(store(CI_RestStore *)));
    Utils::tableSetColumnWidths(ui->tblModifier, ui->tblModifier->columnCount(), 0, 0, 300, 30);
    fDockMod = new DWSelectorDishMod(this);
    fDockMod->configure();
    connect(fDockMod, SIGNAL(dishMod(CI_RestDishMod *)), this, SLOT(dishMod(CI_RestDishMod *)));
    fDockType = new DWSelectorDishType();
    fDockType->configure();
    fDockType->setSelector(ui->leTypeCode);
    fImageChanged = false;
    fImageLoaded = false;
    connect(fDockType, SIGNAL(dishType(CI_RestDishType *)), this, SLOT(dishType(CI_RestDishType *)));
    connect(ui->tabPage, SIGNAL(currentChanged(int)), this, SLOT(tabPageIndexChanged(int)));
    fDockDish = new DWSelectorDish(this);
    fDockDish->configure();
    fDockDish->setDialog(this, SEL_DISH);
    DWSelectorUnit *fDockUnit = new DWSelectorUnit(this);
    fDockUnit->configure();
    fDockUnit->setSelector(ui->leUnitCode);
    fDockUnit->setDialog(this, SEL_UNIT);
    fRestStore = new DWSelectorRestStore(this);
    fRestStore->configure();
    fRestStore->setSelector(ui->leDefStore);
    connect(fRestStore, SIGNAL(store(CI_RestStore *)), this, SLOT(store2(CI_RestStore *)));
    fCacheId = cid_dish;
}

RERestDish::~RERestDish()
{
    delete ui;
}

void RERestDish::selector(int number, const QVariant &value)
{
    switch (number) {
        case SEL_DISH: {
            CI_Dish *c = value.value<CI_Dish *>();
            if (!c) {
                return;
            }
            addDishRow(c->fCode, c->fName, 0);
            break;
        }
        case SEL_UNIT: {
            CI_Unit *c = value.value<CI_Unit *>();
            dockResponse<CI_Unit, CacheUnit>(ui->leUnitCode, ui->leUnitName, c);
            break;
        }
    }
}

bool RERestDish::isDataCorrect()
{
    if (ui->leTypeCode->asInt() == 0) {
        fDataErrors.append(tr("Type code must be defined"));
    }
    if (ui->leQueue->asInt() == 0) {
        DatabaseResult dr;
        dr.select(fDb, "select max(f_queue) as f_queue from r_dish", fDbBind);
        ui->leQueue->setInt(dr.value("f_queue").toInt() + 1);
    }
    if (ui->leUnitCode->asInt() == 0) {
        fDataErrors << tr("Unit must be defined");
    }
    if (ui->leFiscalDebt->asInt() == 0) {
        fDataErrors << tr("Fiscal department cannot be empty");
    }
    return fDataErrors.count() == 0;
}

void RERestDish::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    ui->tblRecipe->clearContents();
    ui->tblRecipe->setRowCount(0);
    if (!isNew) {
        fDbBind[":f_dish"] = ui->leCode->asInt();
        fDb.select("select m.f_id, m.f_state, m.f_menu, m.f_price, m.f_print1, m.f_print2, "
                   "m.f_store, s.f_name, m.f_complex "
                   "from r_menu m "
                   "inner join r_store s on s.f_id=m.f_store "
                   "where m.f_dish=:f_dish", fDbBind, fDbRows);
        foreach_rows {
            int row = 0;
            bool found = false;
            for (int i = 0, count = ui->tblMenu->rowCount(); i < count; i++)
            {
                if (cellValue(i, 2) == it->at(2).toString()) {
                    row = i;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                continue;
            }
            setCellValue(row, 1, it->at(0).toString()); //table rec id
            setCellValue(row, 4, it->at(3).toString()); //price
            setCellValue(row, 5, it->at(4).toString()); //prn1
            setCellValue(row, 6, it->at(5).toString()); //prn2
            setCellValue(row, 7, it->at(6).toString()); //store id
            setCellValue(row, 8, it->at(7).toString()); //store name
            if (it->at(1).toInt() == 1)
            {
                QCheckBox *check = static_cast<QCheckBox *>(ui->tblMenu->cellWidget(row, 0));
                check->setChecked(true);
            }
            if (it->at(8).toInt() == 1)
            {
                QCheckBox *check = static_cast<QCheckBox *>(ui->tblMenu->cellWidget(row, 9));
                check->setChecked(true);
            }
        }
        fDbBind[":f_dish"] = ui->leCode->asInt();
        DatabaseResult dr;
        dr.select("select r.f_part, d.f_en, r.f_qty "
                  "from r_recipe r "
                  "left join r_dish d on d.f_id=r.f_part "
                  "where r.f_dish=:f_dish", fDbBind);
        for (int i = 0; i < dr.rowCount(); i++) {
            addDishRow(dr.value(i, "f_part").toString(), dr.value(i, "f_en").toString(), dr.value(i, "f_qty").toDouble());
        }
        fDbBind[":f_dish"] = ui->leCode->text();
        dr.select(fDb, "select f_code from r_dish_scancode where f_dish=:f_dish", fDbBind);
        for (int  i = 0; i < dr.rowCount(); i++) {
            QListWidgetItem *item = new QListWidgetItem(ui->lstScancodes);
            item->setText(dr.value(i, "f_scancode").toString());
            ui->lstScancodes->addItem(item);
        }
    }
    fDbBind[":f_dish"] = ui->leCode->asInt();
    fDb.select("select m.f_id, m.f_mod, mn.f_" + def_lang + " "
               "from r_dish_mod_required m "
               "inner join r_dish_mod mn on mn.f_id=m.f_mod "
               "where m.f_dish=:f_dish", fDbBind, fDbRows);
    CI_RestDishMod mod;
    foreach_rows {
        mod.fCode = it->at(1).toString();
        mod.fName = it->at(2).toString();
        dishMod( &mod);
        ui->tblModifier->item(ui->tblModifier->rowCount() - 1, 0)->setText(it->at(0).toString());
    }
    CI_Unit *cu = CacheUnit::instance()->get(ui->leUnitCode->text());
    dockResponse<CI_Unit, CacheUnit>(ui->leUnitCode, ui->leUnitName, cu);
    DatabaseResult dr;
    fDbBind[":f_id"] = ui->leCode->asInt();
    dr.select(fDb, "select f_tax, f_defstore from r_dish where f_id=:f_id", fDbBind);
    ui->chAutoPrintTax->setChecked(dr.value("f_tax").toBool());
    ui->leDefStore->setText(dr.value("f_defstore").toString());
    dockResponse<CI_RestStore, CacheRestStore>(ui->leDefStore, ui->leDefStoreName, 0);
    fTrackControl->resetChanges();
}

void RERestDish::clearWidgets()
{
    RowEditorDialog::clearWidgets();
    for (int i = 0, count = ui->tblMenu->rowCount(); i < count; i++) {
        QCheckBox *check = static_cast<QCheckBox *>(ui->tblMenu->cellWidget(i, 0));
        check->setChecked(false);
        check = static_cast<QCheckBox *>(ui->tblMenu->cellWidget(i, 9));
        check->setChecked(false);
        for (int j = 1, colCount = ui->tblMenu->columnCount() - 1; j < colCount; j++) {
            switch (j) {
                case 2:
                case 3:
                    continue;
                    break;
                default:
                    setCellValue(i, j, "");
                    break;
            }
        }
    }
    ui->tblModifier->clearContents();
    ui->tblModifier->setRowCount(0);
    ui->lstScancodes->clear();
    fImageChanged = false;
    fImageLoaded = false;
    ui->lbImage->setPixmap(QPixmap(":/images/cancel.png"));
}

void RERestDish::save()
{
    RowEditorDialog::save();
    for (int i = 0, rowCount = ui->tblMenu->rowCount(); i < rowCount; i++) {
        int id = cellValue(i, 1).toInt();
        QCheckBox *check = static_cast<QCheckBox *>(ui->tblMenu->cellWidget(i, 0));
        fDbBind[":f_state"] = (int) check->isChecked();
        fDbBind[":f_menu"] = cellValue(i, 2);
        fDbBind[":f_dish"] = ui->leCode->text();
        fDbBind[":f_price"] = QLocale().toFloat(cellValue(i, 4));
        fDbBind[":f_print1"] = cellValue(i, 5);
        fDbBind[":f_print2"] = cellValue(i, 6);
        fDbBind[":f_store"] = cellValue(i, 7);
        check = static_cast<QCheckBox *>(ui->tblMenu->cellWidget(i, 9));
        fDbBind[":f_complex"] = (int) check->isChecked();
        if (id == 0) {
            fDb.insert("r_menu", fDbBind);
        } else {
            fDb.update("r_menu", fDbBind, where_id(id));
        }
    }
    for (QStringList::const_iterator it = fRemovedModifiers.begin(); it != fRemovedModifiers.end(); it++) {
        fDbBind[":f_id"] = *it;
        fDb.select("delete from r_dish_mod_required where f_id=:f_id", fDbBind, fDbRows);
    }
    for (int i = 0, rowCount = ui->tblModifier->rowCount(); i < rowCount; i++) {
        if (ui->tblModifier->item(i, 0)->text().isEmpty()) {
            fDbBind[":f_dish"] = ui->leCode->asInt();
            fDbBind[":f_mod"] = ui->tblModifier->item(i, 1)->text();
            fDb.insert("r_dish_mod_required", fDbBind);
        }
    }
    if (fImageChanged) {
        const QPixmap *p = ui->lbImage->pixmap();
        QByteArray bytes;
        QBuffer buff( &bytes);
        buff.open(QIODevice::WriteOnly);
        p->save( &buff, "PNG");
        fDbBind[":f_image"] = bytes;
        fDb.update("r_dish", fDbBind, where_id(ui->leCode->asInt()));
    }
    /* correct queue */
    fDbBind[":f_queue"] = ui->leQueue->asInt();
    fDb.select("update r_dish set f_queue=f_queue+1 where f_queue>=:f_queue", fDbBind, fDbRows);
    fDbBind[":f_queue"] = ui->leQueue->asInt();
    fDb.update("r_dish", fDbBind, where_id(ui->leCode->asInt()));
    /* recipe */
    fDbBind[":f_dish"] = ui->leCode->asInt();
    fDb.select("delete from r_recipe where f_dish=:f_dish", fDbBind, fDbRows);
    for (int i = 0; i < ui->tblRecipe->rowCount(); i++) {
        fDbBind[":f_dish"] = ui->leCode->asInt();
        fDbBind[":f_part"] = ui->tblRecipe->toInt(i, 0);
        fDbBind[":f_qty"] = ui->tblRecipe->lineEdit(i, 2)->asDouble();
        fDb.insert("r_recipe", fDbBind);
    }
    fDbBind[":f_lastPrice"] = ui->leLastPrice->asDouble();
    fDbBind[":f_unit"] = ui->leUnitCode->text();
    fDb.update("r_dish", fDbBind, where_id(ui->leCode->asInt()));
    /* scancodes */
    fDbBind[":f_dish"] = ui->leCode->text();
    fDb.select("delete from r_dish_scancode where f_dish=:f_dish", fDbBind, fDbRows);
    for (int i = 0; i < ui->lstScancodes->count(); i++) {
        fDbBind[":f_code"] = ui->lstScancodes->item(i)->text();
        fDb.select("delete from r_dish_scancode where f_code=:f_code", fDbBind, fDbRows);
        fDbBind[":f_dish"] = ui->leCode->text();
        fDbBind[":f_code"] = ui->lstScancodes->item(i)->text();
        fDb.insert("r_dish_scancode", fDbBind);
    }
    fDbBind[":f_tax"] = ui->chAutoPrintTax->isChecked();
    fDbBind[":f_defstore"] = ui->leDefStore->asInt();
    fDb.update("r_dish", fDbBind, where_id(ui->leCode->asInt()));
}

void RERestDish::hide()
{
    if (ui->tabPage->currentIndex() == 2) {
        ui->tabPage->setCurrentIndex(0);
    }
}

void RERestDish::returnCtrlPressed()
{
    on_btnOk_clicked();
}

void RERestDish::checkComplex(bool v)
{
    EQCheckBox *check = static_cast<EQCheckBox *>(sender());
    fTrackControl->insert("Complex for menu",
                          QString("%1 %2")
                          .arg(cellValue(check->fRow, 3))
                          .arg(int(!v)),
                          QString("%1 %2")
                          .arg(cellValue(check->fRow, 3))
                          .arg((int) v));
}

void RERestDish::tabPageIndexChanged(int index)
{
    if (index == 3) {
        if (!fImageLoaded) {
            if (ui->leCode->asInt() > 0) {
                fDbBind[":f_id"] = ui->leCode->asInt();
                fDb.select("select f_image from r_dish where f_id=:f_id", fDbBind, fDbRows);
                QPixmap p;
                p.loadFromData(fDbRows.at(0).at(0).toByteArray());
                int w = ui->lbImage->width();
                int h = ui->lbImage->height();
                ui->lbImage->setPixmap(p.scaled(w, h));
            }
        }
    }
}

void RERestDish::printLineEditFocusIn()
{
    EQLineEdit *l = static_cast<EQLineEdit *>(sender());
    fDockPrint->setSelector(l);
}

void RERestDish::printLineEditFocusOut()
{
}

void RERestDish::printer(CI_RestPrinter *p)
{
    EQLineEdit *l = static_cast<DWSelectorRestPrinter *>(sender())->selector();
    if (p) {
        l->setText(p->fName);
    }
}

void RERestDish::store(CI_RestStore *s)
{
    EQLineEdit *l = static_cast<DWSelectorRestPrinter *>(sender())->selector();
    if (s) {
        l->setText(s->fName);
        setCellValue(l->fRow, l->fColumn - 1, s->fCode);
    }
}

void RERestDish::store2(CI_RestStore *s)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leDefStore, ui->leDefStoreName, s);
}

void RERestDish::dishMod(CI_RestDishMod *m)
{
    if (m) {
        int row = ui->tblModifier->rowCount();
        ui->tblModifier->setRowCount(row + 1);
        for (int i = 0, colCount = ui->tblModifier->columnCount(); i < colCount; i++) {
            ui->tblModifier->setItem(row, i, new QTableWidgetItem());
        }
        ui->tblModifier->item(row, 1)->setText(m->fCode);
        ui->tblModifier->item(row, 2)->setText(m->fName);
        EPushButton *b = new EPushButton(this);
        b->setMinimumSize(25, 25);
        b->setMaximumSize(25, 25);
        b->setIcon(QIcon(":/images/garbage.png"));
        b->setTag(row);
        connect(b, SIGNAL(clickedWithTag(int)), this, SLOT(btnRemoveModifier(int)));
        ui->tblModifier->setCellWidget(row, 3, b);
    }
}

void RERestDish::dishType(CI_RestDishType *c)
{
    dockResponse<CI_RestDishType, CacheRestDishType>(ui->leTypeCode, ui->leTypeName, c);
}

void RERestDish::storeLineEditFocusIn()
{
    EQLineEdit *l = static_cast<EQLineEdit *>(sender());
    fDockStore->setSelector(l);
}

void RERestDish::storeLineEditFocusOut()
{
}

void RERestDish::btnRemoveModifier(int row)
{
    fRemovedModifiers.append(ui->tblModifier->item(row, 0)->text());
    ui->tblModifier->removeRow(row);
}

void RERestDish::on_btnTextColor_clicked()
{
    QColor color = QColorDialog::getColor(ui->leTextColor->color(), this, tr("Background color"));
    ui->leTextColor->setText(QString::number((int)color.rgb()));
}

void RERestDish::on_btnBgColor_clicked()
{
    QColor color = QColorDialog::getColor(ui->leBgColor->color(), this, tr("Background color"));
    ui->leBgColor->setText(QString::number((int)color.rgb()));
}

void RERestDish::on_btnCancel_clicked()
{
    reject();
}

void RERestDish::on_btnOk_clicked()
{
    if (ui->leMinReminder->isEmpty()) {
        ui->leMinReminder->setText("0");
    }
    if (!isDataCorrect()) {
        message_error(fDataErrors.join("<br>"));
        fDataErrors.clear();
        return;
    }
    save();
}

EQLineEdit *RERestDish::createLineEdit(int row, int column)
{
    EQLineEdit *le = new EQLineEdit(this);
    le->setReadOnly(false);
    le->fRow = row;
    le->fColumn = column;
    le->setFrame(false);
    if (column == 4) {
        le->setValidator(new QDoubleValidator());
    }
    if (column == 7) {
        le->setValidator(new QIntValidator());
    }
    ui->tblMenu->setCellWidget(row, column, le);
    return le;
}

QString RERestDish::cellValue(int row, int column)
{
    EQLineEdit *l = static_cast<EQLineEdit *>(ui->tblMenu->cellWidget(row, column));
    return l->text();
}

void RERestDish::setCellValue(int row, int column, const QString &value)
{
    EQLineEdit *l = static_cast<EQLineEdit *>(ui->tblMenu->cellWidget(row, column));
    l->setText(value);
}

void RERestDish::addDishRow(const QString &code, const QString &name, double qty)
{
    int row = ui->tblRecipe->rowCount();
    ui->tblRecipe->setRowCount(row + 1);
    ui->tblRecipe->setItemWithValue(row, 0, code);
    ui->tblRecipe->setItemWithValue(row, 1, name);
    EQLineEdit *l = ui->tblRecipe->addLineEdit(row, 2, false);
    l->setValidator(new QDoubleValidator(0, 10000, 3));
    l->setDouble(qty);
}

void RERestDish::on_btnAddModifier_clicked()
{
    fDockMod->thisShow(true);
}

void RERestDish::on_btnLoadImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image"), "", "*.jpg;*.png;*.jpeg");
    if (!fileName.isEmpty()) {
        fImageChanged = true;
        int w = ui->lbImage->width();
        int h = ui->lbImage->height();
        QPixmap p(fileName);
        ui->lbImage->setPixmap(p.scaled(w, h));
    }
}

void RERestDish::on_btnAppendRecipe_clicked()
{
    fDockDish->show();
}

void RERestDish::on_btnRemoveRecipeRow_clicked()
{
    int row = ui->tblRecipe->currentRow();
    if (row < 0) {
        return;
    }
    if (message_confirm_tr("Confirm to delete selected row") != QDialog::Accepted) {
        return;
    }
    ui->tblRecipe->removeRow(row);
}

void RERestDish::on_btnClearRecipe_clicked()
{
    if (message_confirm_tr("Confirm to clear recipe") != QDialog::Accepted) {
        return;
    }
    ui->tblRecipe->clearContents();
    ui->tblRecipe->setRowCount(0);
}

void RERestDish::on_chContainItself_clicked(bool checked)
{
    for (int i = 0; i < ui->tblRecipe->rowCount(); i++) {
        if (ui->tblRecipe->toString(i, 0) == ui->leCode->text()) {
            if (checked) {
                return;
            } else {
                ui->tblRecipe->removeRow(i);
                return;
            }
        }
    }
    CI_Dish *c = CacheDish::instance()->get(ui->leCode->text());
    addDishRow(c->fCode, c->fName, 1);
}

void RERestDish::on_btnTrack_clicked()
{
    DlgTracking::showTracking(TRACK_DISH, ui->leCode->text());
}

void RERestDish::on_leReadScancode_returnPressed()
{
    if (ui->leReadScancode->isEmpty()) {
        return;
    }
    QListWidgetItem *item = new QListWidgetItem(ui->lstScancodes);
    item->setText(ui->leReadScancode->text());
    ui->lstScancodes->addItem(item);
    ui->leReadScancode->clear();
}

void RERestDish::on_btnRemove_clicked()
{
    QString err;
    fDbBind[":f_material"] = ui->leCode->asInt();
    DatabaseResult dr;
    dr.select("select * from r_body where f_material=:f_material", fDbBind);
    if (dr.rowCount() > 0) {
        err += tr("This code used in store") + "<BR>";
    }
    fDbBind[":f_dish"] = ui->leCode->asInt();
    dr.select("select * from o_dish where f_dish=:f_dish", fDbBind);
    if (dr.rowCount() > 0) {
        err += tr("This code used in sales") + "<BR>";
    }
    fDbBind[":f_goods"] = ui->leCode->asInt();
    dr.select("select * from st_body where f_goods=:f_goods", fDbBind);
    if (dr.rowCount() > 0) {
        err += tr("Այս կոդը օգտագործվել է պահեստի գույքագրման մեջ") + "<BR>";
    }
    fDbBind[":f_goods"] = ui->leCode->asInt();
    dr.select("select * from r_inventorization_qty where f_goods=:f_goods", fDbBind);
    if (dr.rowCount() > 0) {
        err += tr("Այս կոդը օգտագործվել է պահեստի գույքագրման մեջ") + "<BR>";
    }
    if (!err.isEmpty()) {
        message_error(err);
        return;
    }
    if (message_confirm(tr("Confirm to remove")) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] = ui->leCode->asInt();
    dr.select("delete from r_recipe where f_dish=:f_id or f_part=:f_id", fDbBind);
    fDbBind[":f_id"] = ui->leCode->asInt();
    dr.select("delete from r_dish where f_id=:f_id", fDbBind);
    message_info(tr("Deleted"));
    reject();
}
