#include "dlgbanketitems.h"
#include "ui_dlgbanketitems.h"

DlgBanketItems::DlgBanketItems(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgBanketItems)
{
    ui->setupUi(this);
    fDbBind[":f_rest"] = "1";
    fDb.select("select f_id, f_en from f_invoice_item where f_rest=:f_rest order by 2", fDbBind, fDbRows);
    foreach_rows {
        QListWidgetItem *item = new QListWidgetItem(ui->lst);
        item->setSizeHint(QSize(0, 50));
        item->setData(Qt::UserRole, it->at(0));
        item->setText(it->at(1).toString());
        ui->lst->addItem(item);
    }
}

DlgBanketItems::~DlgBanketItems()
{
    delete ui;
}

bool DlgBanketItems::getItem(int &code, QString &name)
{
    bool result = false;
    DlgBanketItems *d = new DlgBanketItems();
    if (d->exec() == QDialog::Accepted) {
        result = true;
        code = d->ui->lst->currentItem()->data(Qt::UserRole).toInt();
        name = d->ui->lst->currentItem()->text();
    }
    delete d;
    return result;
}

void DlgBanketItems::on_btnCancel_clicked()
{
    reject();
}

void DlgBanketItems::on_btnOk_clicked()
{
    if (ui->lst->currentRow() < 0) {
        return;
    }
    accept();
}
