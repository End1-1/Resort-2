#include "dlggetidname.h"
#include "ui_dlggetidname.h"
#include "database2.h"

QHash<int, QString> DlgGetIDName::fQueries;
QHash<int, DlgGetIDName*> DlgGetIDName::fDialogs;

DlgGetIDName::DlgGetIDName(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgGetIDName)
{
    ui->setupUi(this);
    ui->tbl->setColumnWidth(0, 100);
    ui->tbl->setColumnWidth(1, 400);
    if (fQueries.isEmpty()) {
        fQueries[idname_branch] = "select f_id, f_name from r_branch order by 2";
    }
}

DlgGetIDName::~DlgGetIDName()
{
    delete ui;
}

bool DlgGetIDName::get(QString &id, QString &name, int table, QWidget *parent)
{

    DlgGetIDName *d;
    if (fDialogs.contains(table)) {
        d = fDialogs[table];
    } else {
        d = new DlgGetIDName(parent);
        d->fTable = table;
        d->getData();
    }
    if (d->exec() == QDialog::Accepted) {
        id = d->fId;
        name = d->fName;
        return true;
    }
    return false;
}


void DlgGetIDName::on_btnCancel_clicked()
{
    reject();
}

void DlgGetIDName::getData()
{
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.exec(fQueries[fTable]);
    ui->tbl->setRowCount(db.rowCount());
    ui->tbl->clearSelection();
    int r = 0;
    while (db.next()) {
        ui->tbl->setItem(r, 0, new QTableWidgetItem(db.string("f_id")));
        ui->tbl->setItem(r, 1, new QTableWidgetItem(db.string("f_name")));
        r++;
    }
}

void DlgGetIDName::on_btnOk_clicked()
{
    if (ui->tbl->currentRow() < 0) {
        return;
    }
    fId = ui->tbl->item(ui->tbl->currentRow(), 0)->text();
    fName = ui->tbl->item(ui->tbl->currentRow(), 1)->text();
    accept();
}

void DlgGetIDName::on_btnRefresh_clicked()
{
    getData();
}

void DlgGetIDName::on_leFilter_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        ui->tbl->setRowHidden(i, !ui->tbl->item(i, 0)->text().contains(arg1, Qt::CaseInsensitive) && !ui->tbl->item(i, 1)->text().contains(arg1, Qt::CaseInsensitive));
    }
}

void DlgGetIDName::on_tbl_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    on_btnOk_clicked();
}
