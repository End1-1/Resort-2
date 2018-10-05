#include "rchangemenu.h"
#include "ui_rchangemenu.h"
#include "dishestable.h"

RChangeMenu::RChangeMenu(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RChangeMenu)
{
    ui->setupUi(this);
}

RChangeMenu::~RChangeMenu()
{
    delete ui;
}

bool RChangeMenu::changeMenu(int currentMenu, int &newMenu, QWidget *parent)
{
    bool result = false;
    RChangeMenu *rm = new RChangeMenu(parent);
    for (QMap<int, MenuStruct*>::const_iterator it = DishesTable::fMenu.begin(); it != DishesTable::fMenu.end(); it++) {
        QListWidgetItem *item = new QListWidgetItem(rm->ui->lstMenu);
        MenuStruct *m = it.value();
        item->setText(m->fName[def_lang]);
        item->setData(Qt::UserRole, m->fId);
        item->setSizeHint(QSize(0, 60));
        item->setTextAlignment(Qt::AlignHCenter);
        rm->ui->lstMenu->addItem(item);
        if (currentMenu == m->fId) {
            rm->ui->lstMenu->setCurrentRow(rm->ui->lstMenu->count() - 1);
        }
    }
    if (rm->exec() == QDialog::Accepted) {
        newMenu = rm->ui->lstMenu->currentItem()->data(Qt::UserRole).toInt();
        result = true;
    }
    delete rm;
    return result;

}

void RChangeMenu::on_btnCancel_clicked()
{
    reject();
}

void RChangeMenu::on_btnOk_clicked()
{
    if (ui->lstMenu->currentRow() < 0) {
        return;
    }
    accept();
}
