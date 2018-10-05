#include "finhouseguest.h"
#include "ui_finhouseguest.h"
#include "wreportgrid.h"

FInHouseGuest::FInHouseGuest(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FInHouseGuest)
{
    ui->setupUi(this);
}

FInHouseGuest::~FInHouseGuest()
{
    delete ui;
}

void FInHouseGuest::apply(WReportGrid *rg)
{
    QString where = "where r.f_state=1 order by r.f_room ";
    buildQuery(rg, where);

    QMap<QString, int> total;
    total["B/O-Adult"] = 0;
    total["B/O-Childs"] = 0;
    total["B/B-Adult"] = 0;
    total["B/B-Childs"] = 0;

    int occupied = rg->fModel->rowCount();
    int adults = 0;
    int childs = 0;
    int arCol = rg->fModel->columnIndex(tr("Acc."));
    int adCol = rg->fModel->columnIndex(tr("Adults"));
    int chCol = rg->fModel->columnIndex(tr("Childs"));
    for (int i = 0, count = rg->fModel->rowCount(); i < count; i++) {
        adults += rg->fModel->data(i, adCol).toInt();
        childs += rg->fModel->data(i, chCol).toInt();

        total[rg->fModel->data(i, arCol).toString() + "-Adult"] =
                total[rg->fModel->data(i, arCol).toString() + "-Adult"]
                + rg->fModel->data(i, adCol).toInt();

        total[rg->fModel->data(i, arCol).toString() + "-Childs"] =
                total[rg->fModel->data(i, arCol).toString() + "-Childs"]
                + rg->fModel->data(i, chCol).toInt();
    }

    int colCount = rg->fModel->columnCount();
    QList<QVariant> emptyRow;
    for (int i = 0; i < colCount; i++) {
        emptyRow << "";
    }
    QList<QVariant> tmpRow = emptyRow;
    rg->fModel->appendRow(emptyRow);
    tmpRow[1] = tr("Occupied");
    tmpRow[2] = tr("Adults");
    tmpRow[3] = tr("Childs");
    rg->fModel->appendRow(tmpRow);
    tmpRow = emptyRow;
    tmpRow[1] = QString::number(occupied);
    tmpRow[2] = QString::number(adults);
    tmpRow[3] = QString::number(childs);
    rg->fModel->appendRow(tmpRow);
    tmpRow = emptyRow;
    rg->fModel->appendRow(tmpRow);
    tmpRow[1] = "B/O - Adult";
    tmpRow[2] = "B/O - Child";
    tmpRow[3] = "B/B - Adult";
    tmpRow[4] = "B/B - Child";
    rg->fModel->appendRow(tmpRow);
    tmpRow = emptyRow;
    tmpRow[1] = total["B/O-Adult"];
    tmpRow[2] = total["B/O-Childs"];
    tmpRow[3] = total["B/B-Adult"];
    tmpRow[4] = total["B/B-Childs"];
    rg->fModel->appendRow(tmpRow);
    rg->fTableView->setSpan(occupied, 0, 1, colCount);
    rg->fTableView->setSpan(occupied + 3, 0, 1, colCount);

    if (ui->chDisplayRate->isChecked()) {
        QList<int> cols;
        cols << 9;
        QList<double> val;
        rg->fModel->sumOfColumns(cols, val);
        rg->fModel->setData(rg->fModel->rowCount() - 5, 8, tr("TOTAL RATE"));
        rg->fModel->setData(rg->fModel->rowCount() - 5, 9, val.at(0));
    }
}

QWidget *FInHouseGuest::firstElement()
{
    return ui->leDate;
}

QString FInHouseGuest::reportTitle()
{
    return QString("%1, %2")
            .arg(tr("In house guests"))
            .arg(ui->leDate->text());
}
