#include "dwreservationitemsize.h"
#include "ui_dwreservationitemsize.h"
#include <QCheckBox>

DWReservationItemSize::DWReservationItemSize(QList<int> &values, QWidget *parent) :
    QDockWidget(parent),
    Base(),
    ui(new Ui::DWReservationItemSize),
    fValues(values)
{
    ui->setupUi(this);
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(thisVisibilityChanged(bool)));
    fDb.select("select f_id, f_" + def_lang + " from f_invoice_item order by f_id", fDbBind, fDbRows);
    int row = 0; int col = 0;
    Utils::setupTableFullColumnWidth(ui->tblItem, 200, fDbRows.count());
    foreach_rows {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, it->at(0).toInt());
        ui->tblItem->setItem(row, col, item);
        QCheckBox *check = new QCheckBox(it->at(1).toString(), this);
        check->setCheckState(values.contains(it->at(0).toInt()) ? Qt::Checked : Qt::Unchecked);
        ui->tblItem->setCellWidget(row, col, check);
        col ++;
        if (col == 3) {
            col = 0;
            row ++;
        }
    }
}

DWReservationItemSize::~DWReservationItemSize()
{
    delete ui;
}

void DWReservationItemSize::thisVisibilityChanged(bool v)
{
    if (v) {
        for (int i = 0, rowCount = ui->tblItem->rowCount(); i < rowCount; i++) {
            for (int j = 0, colCount = ui->tblItem->columnCount(); j < colCount; j++) {
                QTableWidgetItem *item = ui->tblItem->item(i, j);
                if (item) {
                    QCheckBox *check = static_cast<QCheckBox*>(ui->tblItem->cellWidget(i, j));
                    check->setCheckState(fValues.contains(item->data(Qt::UserRole).toInt()) ? Qt::Checked : Qt::Unchecked);
                }
            }
        }
    } else {
        fValues.clear();
        for (int i = 0, rowCount = ui->tblItem->rowCount(); i < rowCount; i++) {
            for (int j = 0, colCount = ui->tblItem->columnCount(); j < colCount; j++) {
                QTableWidgetItem *item = ui->tblItem->item(i, j);
                if (item) {
                    QCheckBox *check = static_cast<QCheckBox*>(ui->tblItem->cellWidget(i, j));
                    if (check->checkState() == Qt::Checked) {
                        fValues.append(item->data(Qt::UserRole).toInt());
                    }
                }
            }
        }
    }
}
