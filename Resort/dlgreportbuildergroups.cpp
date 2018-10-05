#include "dlgreportbuildergroups.h"
#include "ui_dlgreportbuildergroups.h"
#include <QInputDialog>

DlgReportBuilderGroups::DlgReportBuilderGroups(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReportBuilderGroups)
{
    ui->setupUi(this);
    fDb.select("select f_id, f_name from serv_reports_group order by f_name", fDbBind, fDbRows);
    foreach_rows {
        QListWidgetItem *item = new QListWidgetItem(ui->lwList);
        item->setText(it->at(1).toString());
        item->setData(Qt::UserRole, it->at(0));
        ui->lwList->addItem(item);
    }
}

DlgReportBuilderGroups::~DlgReportBuilderGroups()
{
    delete ui;
}

void DlgReportBuilderGroups::on_btnClose_clicked()
{
    accept();
}

void DlgReportBuilderGroups::on_btnNew_clicked()
{
    QString name = QInputDialog::getText(this, tr("New name"), tr("Name"));
    if (name.isEmpty()) {
        return;
    }
    fDbBind[":f_name"] = name;
    int id = fDb.insert("serv_reports_group", fDbBind);
    QListWidgetItem *item = new QListWidgetItem(ui->lwList);
    item->setText(name);
    item->setData(Qt::UserRole, id);
    ui->lwList->addItem(item);
}

void DlgReportBuilderGroups::on_btnEdit_clicked()
{
    QListWidgetItem *item = ui->lwList->currentItem();
    if (!item) {
        message_error_tr("Nothing is selected");
        return;
    }
    QString name = QInputDialog::getText(this, tr("New name"), tr("Name"), QLineEdit::Normal, item->text());
    if (name.isEmpty()) {
        return;
    }
    fDbBind[":f_name"] = name;
    fDb.update("serv_reports_group", fDbBind, where_id(item->data(Qt::UserRole).toInt()));
    item->setText(name);
    message_info_tr("Group name successfuly updated");
}

void DlgReportBuilderGroups::on_btnRemove_clicked()
{
    QListWidgetItem *item = ui->lwList->currentItem();
    if (!item) {
        message_error_tr("Nothing is selected");
        return;
    }
    if (message_confirm_tr("Confirm to delete selected group.<br><b>ALL REPORTS THATS ASSIGNED TO THIS GROUP WILL BE REMOVE TOO!</b>") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_group"] = item->data(Qt::UserRole);
    fDb.select("delete from serv_reports where f_group=:f_group", fDbBind, fDbRows);
    fDbBind[":f_id"] = item->data(Qt::UserRole);
    fDb.select("delete from serv_reports_group where f_id=:f_id", fDbBind, fDbRows);
    ui->lwList->removeItemWidget(item);
    message_info_tr("Group successfuly removed :(");
}
