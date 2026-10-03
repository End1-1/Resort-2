#include "rerestbranch.h"
#include "ui_rerestbranch.h"
#include "defines.h"
#include "utils.h"
#include <QCheckBox>
#include <QHeaderView>
#include <QSet>
#include <QTableWidgetItem>

RERestBranch::RERestBranch(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_BRANCH, parent),
    ui(new Ui::RERestBranch)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "r_branch";
    loadMenuTable();
    connect(ui->btnOk, &QPushButton::clicked, this, &RERestBranch::on_btnOk_clicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &RERestBranch::on_btnCancel_clicked);
}

RERestBranch::~RERestBranch()
{
    delete ui;
}

void RERestBranch::loadMenuTable()
{
    ui->tblMenus->clearContents();
    ui->tblMenus->setRowCount(0);
    Utils::tableSetColumnWidths(ui->tblMenus, 2, 40, 320);
    ui->tblMenus->horizontalHeader()->setStretchLastSection(true);

    fDb.select(QString("select f_id, f_%1 from r_menu_names where f_enabled=1 order by f_id").arg(def_lang),
               fDbBind, fDbRows);
    int row = 0;
    foreach_rows {
        ui->tblMenus->insertRow(row);
        auto *check = new QCheckBox(this);
        ui->tblMenus->setCellWidget(row, 0, check);
        auto *item = new QTableWidgetItem(it->at(1).toString());
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setData(Qt::UserRole, it->at(0));
        ui->tblMenus->setItem(row, 1, item);
        row++;
    }
}

void RERestBranch::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    if (isNew || ui->leCode->asInt() <= 0) {
        return;
    }

    QSet<int> selected;
    fDbBind[":f_branch"] = ui->leCode->asInt();
    fDb.select("select f_menu from r_branch_menu where f_branch=:f_branch", fDbBind, fDbRows);
    foreach_rows {
        selected.insert(it->at(0).toInt());
    }

    for (int i = 0; i < ui->tblMenus->rowCount(); i++) {
        const int menuId = ui->tblMenus->item(i, 1)->data(Qt::UserRole).toInt();
        if (auto *check = qobject_cast<QCheckBox *>(ui->tblMenus->cellWidget(i, 0))) {
            check->setChecked(selected.contains(menuId));
        }
    }
}

void RERestBranch::saveBranchMenus(int branchId)
{
    fDbBind[":f_branch"] = branchId;
    fDb.select("delete from r_branch_menu where f_branch=:f_branch", fDbBind, fDbRows);
    for (int i = 0; i < ui->tblMenus->rowCount(); i++) {
        auto *check = qobject_cast<QCheckBox *>(ui->tblMenus->cellWidget(i, 0));
        if (!check || !check->isChecked()) {
            continue;
        }
        const int menuId = ui->tblMenus->item(i, 1)->data(Qt::UserRole).toInt();
        fDbBind[":f_branch"] = branchId;
        fDbBind[":f_menu"] = menuId;
        fDb.insert("r_branch_menu", fDbBind);
    }
}

void RERestBranch::save()
{
    if (!saveOnly()) {
        return;
    }
    saveBranchMenus(ui->leCode->asInt());
    accept();
}

void RERestBranch::on_btnCancel_clicked()
{
    reject();
}

void RERestBranch::on_btnOk_clicked()
{
    save();
}
