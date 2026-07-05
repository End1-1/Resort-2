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
        fQueries[idname_hall] = "select f_id, f_name from r_hall order by 2 ";
        fQueries[idname_store] = "select f_id, f_name from r_store where f_state=1 order by 2";
        fQueries[idname_dish] = "select f_id, f_en as f_name from r_dish order by 2";
        fQueries[idname_dish_defstore] = "select d.f_id, d.f_en as f_name, d.f_defstore, coalesce(s.f_name, '') as f_storename "
                                          "from r_dish d "
                                          "left join r_store s on s.f_id=d.f_defstore "
                                          "order by 2";
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

    if(fTable == idname_dish_defstore) {
        ui->tbl->setColumnCount(3);
        ui->tbl->setHorizontalHeaderLabels(QStringList() << tr("Code") << tr("Name") << tr("Store"));
        ui->tbl->setColumnWidth(0, 80);
        ui->tbl->setColumnWidth(1, 350);
        ui->tbl->setColumnWidth(2, 120);
    } else {
        ui->tbl->setColumnCount(2);
        ui->tbl->setHorizontalHeaderLabels(QStringList() << tr("Code") << tr("Name"));
        ui->tbl->setColumnWidth(0, 100);
        ui->tbl->setColumnWidth(1, 400);
    }

    int r = 0;
    while (db.next()) {
        ui->tbl->setItem(r, 0, new QTableWidgetItem(db.string("f_id")));
        ui->tbl->setItem(r, 1, new QTableWidgetItem(db.string("f_name")));

        if(fTable == idname_dish_defstore) {
            QString storeText = db.string("f_defstore");
            const QString storeName = db.string("f_storename");

            if(!storeName.isEmpty()) {
                storeText += " - " + storeName;
            }

            ui->tbl->setItem(r, 2, new QTableWidgetItem(storeText));
        }

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
        bool match = ui->tbl->item(i, 0)->text().contains(arg1, Qt::CaseInsensitive)
                     || ui->tbl->item(i, 1)->text().contains(arg1, Qt::CaseInsensitive);

        if(fTable == idname_dish_defstore && ui->tbl->columnCount() > 2 && ui->tbl->item(i, 2)) {
            match = match || ui->tbl->item(i, 2)->text().contains(arg1, Qt::CaseInsensitive);
        }

        ui->tbl->setRowHidden(i, !match);
    }
}

void DlgGetIDName::on_tbl_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    on_btnOk_clicked();
}
