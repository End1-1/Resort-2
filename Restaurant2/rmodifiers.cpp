#include "rmodifiers.h"
#include "ui_rmodifiers.h"

RModifiers::RModifiers(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RModifiers)
{
    ui->setupUi(this);
    show();
    qApp->processEvents();
}

RModifiers::~RModifiers()
{
    delete ui;
}

void RModifiers::setModifiers(const QStringList &modifiers)
{
    QList<QString> l(modifiers);
    Utils::setupTableFullColumnWidth(ui->tblMod, 200, modifiers.count());
    Utils::fillTableWithElements<QString>(ui->tblMod, l, Qt::DisplayRole);
}

QString RModifiers::mod()
{
    return fMod;
}

void RModifiers::on_btnCancel_clicked()
{
    reject();
}

void RModifiers::on_btnOk_clicked()
{
    fMod.clear();
    QModelIndexList l = ui->tblMod->selectionModel()->selectedIndexes();
    if (l.count() == 0) {
        return;
    }
    bool first = true;
    for (QModelIndexList::const_iterator it = l.begin(); it != l.end(); it++) {
        if (!it->isValid()) {
            continue;
        }
        if (first) {
            first = false;
        } else {
            fMod += ",";
        }
        fMod += it->data(Qt::DisplayRole).toString();
    }
    if (fMod.length() == 0) {
        return;
    }
    accept();
}
