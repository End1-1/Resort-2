#include "dlgdeptholder.h"
#include <QMessageBox>
#include "database2.h"
#include "rmessage.h"
#include "ui_dlgdeptholder.h"

DlgDeptHolder::DlgDeptHolder(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgDeptHolder)
{
    ui->setupUi(this);
    fEditName = true;
    connect(ui->leName, &EQLineEdit::focusIn, [this](){
        fEditName = true;
        ui->lstHolders->clearSelection();
        ui->wKbd->clearText();
        for (int i = 0; i < ui->lstHolders->count(); i++) {
            ui->lstHolders->item(i)->setHidden(!ui->lstHolders->item(i)->text().contains(ui->leName->text(), Qt::CaseInsensitive));
        }
    });
    connect(ui->leInfo, &EQLineEdit::focusIn, [this]() {
       fEditName = false;
       ui->lstHolders->clearSelection();
       ui->wKbd->clearText();
    });
    connect(ui->wKbd, &RKeyboard::textChanged, [this](const QString &text) {
        if (fEditName) {
            ui->leName->setText(text);
            Db b = Preferences().getDatabase(Base::fDbName);
            Database2 db2;

            if (!db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
                QMessageBox::critical(0, "Db error", db2.lastDbError());
                exit(0);
            }
            db2.exec(QString("select  d.f_id, d.f_name, d.f_info from o_debt_holder d where d.f_name like '%1%' limit 10 ").arg(text));
            ui->lstHolders->clear();
            while (db2.next()) {
                QListWidgetItem *item = new QListWidgetItem(ui->lstHolders);
                item->setText(db2.string("f_name"));
                item->setData(Qt::UserRole, db2.integer("f_id"));
                ui->lstHolders->addItem(item);
            }
        } else {
            ui->leInfo->setText(text.toUpper());
        }
    });

    fHolderId = 0;
    connect(ui->wKbd, &RKeyboard::accepted, [this](){
        on_btnSave_clicked();
    });
    connect(ui->wKbd, &RKeyboard::rejected, [this](){
       on_btnCancel_clicked();
    });
}

DlgDeptHolder::~DlgDeptHolder()
{
    delete ui;
}

bool DlgDeptHolder::getHolder(int &id, QString &name)
{
    DlgDeptHolder *d = new DlgDeptHolder();
    bool result = d->exec() == QDialog::Accepted;
    id = d->fHolderId;
    name = d->fHolderName;
    delete d;
    return result;
}

void DlgDeptHolder::on_btnCancel_clicked()
{
    reject();
}

void DlgDeptHolder::on_btnSave_clicked()
{
    if (fHolderId == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    fDbBind[":f_info"] = ui->leInfo->text();
    fDb.update("o_debt_holder", fDbBind, where_id(fHolderId));
    accept();
}

void DlgDeptHolder::on_btnCreateNew_clicked()
{
    if (ui->leName->isEmpty()) {
        message_error(tr("Name cannot be empty"));
        return;
    }
    fDbBind[":f_name"] = ui->leName->text();
    fDbBind[":f_info"] = ui->leInfo->text();
    fHolderId = fDb.insert("o_debt_holder", fDbBind);
    if (fHolderId < 1) {
        message_error(fDb.fLastError);
        return;
    }
    fHolderName = ui->leName->text();
    accept();
}

void DlgDeptHolder::on_lstHolders_itemClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    fHolderId = item->data(Qt::UserRole).toInt();
    fHolderName = item->text();
    ui->leName->setText(item->text());
}

void DlgDeptHolder::on_btnUnknown_clicked()
{
    ui->wKbd->setText(tr("Unknown"));
}
