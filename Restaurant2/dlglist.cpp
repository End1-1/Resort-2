#include "dlglist.h"
#include "ui_dlglist.h"

DlgList::DlgList(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgList)
{
    ui->setupUi(this);
    connect(ui->wKbd, &RKeyboard::textChanged, [this](const QString &text) {
        for (int i = 0; i < ui->lst->count(); i++) {
            ui->lst->item(i)->setHidden(false);
            if (text.isEmpty()) {
                continue;
            }
            if (!ui->lst->item(i)->text().contains(text, Qt::CaseInsensitive)) {
                ui->lst->item(i)->setHidden(true);
            }
        }
    });

    connect(ui->wKbd, &RKeyboard::accepted, [this]() {
        ui->btnOk->click();
    });

    connect(ui->wKbd, &RKeyboard::rejected, [this](){
       reject();
    });
}

DlgList::~DlgList()
{
    delete ui;
}

bool DlgList::getValue(const QString &caption, QString &name, QVariant &result, const QString &sql)
{
    bool r = false;
    DlgList *d = new DlgList();
    d->ui->lbCaption->setText(caption);
    d->build(sql);
    if (d->exec() == QDialog::Accepted) {
        result = d->fResult;
        name = d->fName;
        r = true;
    }
    delete d;
    return r;
}

void DlgList::on_btnCancel_clicked()
{
    reject();
}

void DlgList::build(const QString &sql)
{
    fDb.select(sql, fDbBind, fDbRows);
    foreach_rows {
        QListWidgetItem *item = new QListWidgetItem(ui->lst);
        item->setData(Qt::UserRole, it->at(0));
        item->setText(it->at(1).toString());
        item->setSizeHint(QSize(50, 50));
        ui->lst->addItem(item);
    }
}

void DlgList::on_btnOk_clicked()
{
    QListWidgetItem *item = ui->lst->currentItem();
    if (!item) {
        return;
    }
    fName = item->text();
    fResult = item->data(Qt::UserRole);
    accept();
}
