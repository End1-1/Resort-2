#include "dlgdeptholder.h"
#include "ui_dlgdeptholder.h"
#include "rmessage.h"
#include "cacherestdebtholder.h"

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
            ui->leName->setText(text.toUpper());
            for (int i = 0; i < ui->lstHolders->count(); i++) {
                ui->lstHolders->item(i)->setHidden(true);
                if (ui->lstHolders->item(i)->text().contains(text, Qt::CaseInsensitive)) {
                    ui->lstHolders->item(i)->setHidden(false);
                }
            }
        } else {
            ui->leInfo->setText(text.toUpper());
        }
    });
    QMapIterator<QString, CI_RestDebtHolder*> it (CacheRestDebtHolder::instance()->it());
    while (it.hasNext()) {
        it.next();
        QListWidgetItem *item = new QListWidgetItem(ui->lstHolders);
        item->setText(it.value()->fName + "/" + it.value()->fInfo);
        item->setData(Qt::UserRole, it.value()->fCode);
        ui->lstHolders->addItem(item);
    }
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
