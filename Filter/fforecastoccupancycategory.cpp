#include "fforecastoccupancycategory.h"
#include "ui_fforecastoccupancycategory.h"
#include "wreportgrid.h"

FForecastOccupancyCategory::FForecastOccupancyCategory(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FForecastOccupancyCategory)
{
    ui->setupUi(this);
    setupTab();
}

FForecastOccupancyCategory::~FForecastOccupancyCategory()
{
    delete ui;
}

void FForecastOccupancyCategory::apply(WReportGrid *rg)
{
    if (ui->deStart->date() < WORKING_DATE || ui->deEnd->date() < WORKING_DATE) {
        message_error_tr("Invalid starting date");
        return;
    }
    rg->fModel->clearData();
    rg->fModel->clearColumns();
    QMap<QString, int> cats;
    QMap<QString, int> catsQty;
    fDb.select("select rc.f_short, count(rm.f_id) from f_room_classes rc "
               "left join f_room rm on rm.f_class=rc.f_id group by 1", fDbBind, fDbRows);
    int ind = 0;
    int totRooms = 0;
    foreach_rows {
        cats[it->at(0).toString()] = ind++;
        catsQty[it->at(0).toString()] = it->at(1).toInt();
        totRooms += it->at(1).toInt();
    }

    rg->fModel->setColumn(100, "", tr("Date"));
    rg->fModel->setColumn(40, "", tr("Tot"));
    for (QMap<QString, int>::const_iterator it = cats.begin(); it != cats.end(); it++) {
        rg->fModel->setColumn(40, "", it.key());
    }
    rg->fModel->setColumn(80, "", tr("Tot/occ"));
    int vacIndex = rg->fModel->columnCount();
    for (QMap<QString, int>::const_iterator it = cats.begin(); it != cats.end(); it++) {
        rg->fModel->setColumn(40, "", it.key());
    }
    rg->fModel->setColumn(80, "", tr("Tot/vac"));
    int totIndex = rg->fModel->columnCount();
    rg->fModel->setColumn(40, "", tr("O/O"))
            .setColumn(80, "", tr("Amount"))
            .setColumn(80, "", tr("Avg. rate"))
            .setColumn(50, "", tr("Per %"));

    QString query = QString("call forecast_occupancy(%1, %2)")
            .arg(ui->deStart->dateMySql())
            .arg(ui->deEnd->dateMySql());
    fDb.queryDirect(query);
    fDb.select("select * from rep", fDbBind, fDbRows);

    QList<QVariant> emptyRow;
    for (int i = 0; i < rg->fModel->columnCount(); i++) {
        emptyRow << QVariant();
    }

    ind = 0;
    QMap<QDate, int> dates;
    for (QDate d = ui->deStart->date(); d != ui->deEnd->date().addDays(1); d = d.addDays(1)) {
        dates[d] = ind++;
        emptyRow[0] = d;
        emptyRow[1] = totRooms;
        for (QMap<QString, int>::const_iterator it = cats.begin(); it != cats.end(); it++) {
            emptyRow[it.value() + cats.count() + 3] = catsQty[it.key()];
        }
        rg->fModel->appendRow(emptyRow);
    }

    int ooIndex = 1 + 1 + cats.count() + 1 + cats.count() + 1;
    int amountIndex = ooIndex + 1;
    int avgIndex = amountIndex + 1;
    int perIndex = avgIndex + 1;
    foreach_rows {
        int dateIndex = dates[it->at(0).toDate()];
        int catIndex = cats[it->at(1).toString()] + 2;
        //rg->fModel->setData(dateIndex, catIndex + cats.count() + 1, it->at())
        rg->fModel->setData(dateIndex, catIndex, it->at(4));
        rg->fModel->setData(dateIndex, catIndex + cats.count() + 1, catsQty[it->at(1).toString()] - it->at(4).toInt() - it->at(3).toInt());
        rg->fModel->setData(dateIndex, amountIndex, rg->fModel->data(dateIndex, amountIndex, Qt::EditRole).toDouble() + it->at(5).toDouble());
        rg->fModel->setData(dateIndex, avgIndex, it->at(6));
        rg->fModel->setData(dateIndex, perIndex, 0.1);
        rg->fModel->setData(dateIndex, ooIndex, it->at(3).toInt() + rg->fModel->data(dateIndex, ooIndex, Qt::EditRole).toInt());
    }

    for (int i = 0, count = rg->fModel->rowCount(); i < count; i++) {
        int totOcc = 0;
        int totVac = 0;
        for (int j = 2; j < vacIndex; j++) {
            totOcc += rg->fModel->data(i, j, Qt::EditRole).toInt();
            totVac += rg->fModel->data(i, j + cats.count() + 1, Qt::EditRole).toInt();
        }
        rg->fModel->setData(i, vacIndex - 1, totOcc);
        rg->fModel->setData(i, totIndex - 1, totVac);
        if (totOcc > 0) {
            rg->fModel->setData(i, avgIndex, rg->fModel->data(i, amountIndex, Qt::EditRole).toDouble() / totOcc);
        }
        double per = 0;
        if (rg->fModel->data(i, 1, Qt::EditRole).toInt() > 0) {
            per = (totOcc * 100) / rg->fModel->data(i, 1, Qt::EditRole).toInt();
        }
        rg->fModel->setData(i, perIndex, per);
    }

    rg->fModel->apply(0);

    for (int i = 0, count = rg->fModel->rowCount(); i < count; i++) {
        rg->fModel->setBackgroundColor(i, 1, COLOR_DARK_ROW);
        for (int j = vacIndex; j < totIndex; j++) {
            rg->fModel->setBackgroundColor(i, j, COLOR_DARK_ROW);
        }
    }

    QList<int> cols;
    QList<double> vals;
    for (int i = 1; i < avgIndex; i++) {
        cols << i;
    }
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
    if (rg->fTableTotal->toInt(0, 1) > 0) {
        rg->fTableTotal->setItemWithValue(0, perIndex, rg->fTableTotal->toInt(0, vacIndex) / rg->fTableTotal->toInt(0, 1));
    }
}

QWidget *FForecastOccupancyCategory::firstElement()
{
    return ui->deStart;
}

QString FForecastOccupancyCategory::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Forecast / Occupancy"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FForecastOccupancyCategory::setupTab()
{
    fReportGrid->setupTabTextAndIcon(QString("%1 %2-%3")
                                     .arg(tr("Forecast occupancy / category"))
                                     .arg(ui->deStart->date().toString(def_date_format))
                                     .arg(ui->deEnd->date().toString(def_date_format)),
                                     ":/images/checkin.png");
}

void FForecastOccupancyCategory::on_deStart_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
    setupTab();
}

void FForecastOccupancyCategory::on_deEnd_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
    setupTab();
}
