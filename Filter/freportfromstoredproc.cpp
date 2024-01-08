#include "freportfromstoredproc.h"
#include "ui_freportfromstoredproc.h"

FReportFromStoredProc::FReportFromStoredProc(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::FReportFromStoredProc)
{
    ui->setupUi(this);

}

FReportFromStoredProc::~FReportFromStoredProc()
{
    delete ui;
}

EQTableWidget *FReportFromStoredProc::tbl(Database2 &db2)
{
    int row = 0;
    QList<int> sumColumns;
    QList<double> sums;
    while (db2.next()) {
        if (ui->tbl->columnCount() == 0) {
            ui->tbl->setColumnCount(db2.columnCount());
            QStringList headers;
            for (int i = 0; i < db2.columnCount(); i++) {
                headers.append(db2.columnName(i));
                if (db2.value(i).type() == QVariant::Double) {
                    sumColumns.append(i);
                }
            }
            ui->tbl->setHorizontalHeaderLabels(headers);
            ui->tbl->setRowCount(db2.rowCount());
        }
        for (int i = 0; i < db2.columnCount(); i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::EditRole, db2.value(i));
            ui->tbl->setItem(row, i, item);
        }
        row++;
    }
    QStringList rowLables;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        rowLables.append(QString::number(i + 1));
    }
    ui->tbl->setVerticalHeaderLabels(rowLables);
    ui->totals->setVerticalHeaderLabels({QString::number(rowLables.count())});
    ui->totals->setColumnCount(ui->tbl->columnCount());
    ui->totals->setVisible(sumColumns.count() > 0);
    for (int c: sumColumns) {
        double v = 0;
        for (int i = 0; i < ui->tbl->rowCount(); i++) {
            v += ui->tbl->item(i, c)->data(Qt::EditRole).toDouble();
        }
        auto *item = new QTableWidgetItem();
        item->setData(Qt::EditRole, v);
        ui->totals->setItem(0, c, item);
    }
    ui->tbl->resizeColumnsToContents();
    ui->totals->resizeColumnsToContents();
    for (int i = 0; i < ui->tbl->columnCount(); i++) {
        if (ui->tbl->columnWidth(i) < ui->totals->columnWidth(i)) {
            ui->tbl->setColumnWidth(i, ui->totals->columnWidth(i));
        }
        if (ui->totals->columnWidth(i) < ui->tbl->columnWidth(i)) {
            ui->totals->setColumnWidth(i, ui->tbl->columnWidth(i));
        }
    }
    return ui->tbl;
}

void FReportFromStoredProc::setupTab()
{
    setupTabTextAndIcon(fTitle, ":/images/refund.png");
}
