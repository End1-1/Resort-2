#include "dlgfiltervalues.h"
#include "ui_dlgfiltervalues.h"
#include <QListWidget>
#include <QCheckBox>

DlgFilterValues::DlgFilterValues(QStringList &values, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgFilterValues),
    fValues(values)
{
    ui->setupUi(this);
    for (QStringList::const_iterator it = values.begin(); it != values.end(); it++) {
        QListWidgetItem *item = new QListWidgetItem(ui->lst);
        QCheckBox *c = new QCheckBox(*it);
        ui->lst->addItem(item);
        ui->lst->setItemWidget(item, c);
    }
}

DlgFilterValues::~DlgFilterValues()
{
    delete ui;
}

void DlgFilterValues::on_buttonBox_accepted()
{
    for (int i = ui->lst->count() - 1; i > -1; i--) {
        QCheckBox *c = static_cast<QCheckBox*>(ui->lst->itemWidget(ui->lst->item(i)));
        if (c->checkState() != Qt::Checked)
            fValues.removeAt(i);
    }
}
