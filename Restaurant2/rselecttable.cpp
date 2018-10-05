#include "rselecttable.h"
#include "ui_rselecttable.h"
#include "rchangehall.h"
#include <QItemDelegate>
#include <QPainter>

class TableItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, Qt::white);
        if (!index.isValid()) {
            return;
        }
        TableStruct *t = index.data(Qt::UserRole).value<TableStruct*>();
        if (!t) {
            return;
        }
        QTextOption o;
        o.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        QRect textRect = option.rect;
        textRect.adjust(2, 2, -2, -2);
        painter->drawText(textRect, t->fName, o);
    }
};

RSelectTable::RSelectTable(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RSelectTable)
{
    ui->setupUi(this);
    ui->tblTables->setItemDelegate(new TableItemDelegate());
    showFullScreen();
}

RSelectTable::~RSelectTable()
{
    delete ui;
}

void RSelectTable::setup(int hallId)
{
    QList<TableStruct*> tables;
    Hall::filterTables(hallId, 0, tables);
    Utils::setupTableFullColumnWidth(ui->tblTables, 100, tables.count());
    Utils::fillTableWithElements<TableStruct*>(ui->tblTables, tables);
}

TableStruct *RSelectTable::table()
{
    return fTable;
}

void RSelectTable::on_btnReject_clicked()
{
    reject();
}

void RSelectTable::on_btnChangeHall_clicked()
{
    RChangeHall *h = new RChangeHall(this);
    h->setup(Hall::fHallTable);
    if (h->exec() == QDialog::Accepted) {
        setup(h->hall());
    }
    delete h;
}

void RSelectTable::on_tblTables_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    TableStruct *t = index.data(Qt::UserRole).value<TableStruct*>();
    if (!t) {
        return;
    }
    fTable = t;
    accept();
}
