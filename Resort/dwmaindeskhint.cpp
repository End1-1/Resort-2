#include "dwmaindeskhint.h"
#include "ui_dwmaindeskhint.h"
#include "dlgfiltervalues.h"
#include "basewidget.h"
#include "wreservation.h"

DWMainDeskHint::DWMainDeskHint(QWidget *parent) :
    QDockWidget(parent, Qt::WindowStaysOnBottomHint),
    Base(),
    ui(new Ui::DWMainDeskHint)
{
    ui->setupUi(this);
    fCheckInFilter = false;
    connect(ui->tblDockHint->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(tblHeaderSectionClicked(int)));
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(thisVisibilityChanged(bool)));
    ui->btnCheckIn->setVisible(false);
}

DWMainDeskHint::~DWMainDeskHint()
{
    delete ui;
}

void DWMainDeskHint::commonFilter(const QString &filter, int col)
{
    if (fCheckInFilter) {
        return;
    }
    EQTableWidget *t = ui->tblDockHint;
    QList<int> columns;
    if (col == -1) {
        for (int i = 0, count = ui->tblDockHint->columnCount(); i < count; i++) {
            columns << i;
        }
    } else {
        columns << col;
    }
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        if (filter.length() == 0 && t->toInt(i, 0) != -1) {
            t->setRowHidden(i, false);
            continue;
        }
        t->setRowHidden(i, true);
        for (int j = 0, colCount = columns.count(); j < colCount; j++) {
            if (t->item(i, columns.at(j))->text().contains(filter, Qt::CaseInsensitive) && t->toInt(i, 0) != -1) {
                t->setRowHidden(i, false);
                break;
            }
        }
    }
}

void DWMainDeskHint::filterEqual(const QStringList &values, int col)
{
    if (fCheckInFilter) {
        return;
    }
    EQTableWidget *t = ui->tblDockHint;
    QList<int> columns;
    if (col == -1) {
        for (int i = 0, count = ui->tblDockHint->columnCount(); i < count; i++) {
            columns << i;
        }
    } else {
        columns << col;
    }
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        t->setRowHidden(i, true);
        if (t->toInt(i, 0) == -1) {
            goto mark;
        }
        for (int j = 0, colCount = columns.count(); j < colCount; j++) {
            for (QStringList::const_iterator it = values.begin(); it != values.end(); it++) {
                if (t->item(i, columns.at(j))->text().compare(*it, Qt::CaseInsensitive) == 0) {
                    t->setRowHidden(i, false);
                    goto mark;
                }
            }
        }
        mark:
        continue;
    }
}

void DWMainDeskHint::checkInFilter()
{
    setWindowTitle(tr("Ready for checkin"));
    ui->btnCheckIn->setVisible(true);
    fCheckInFilter = true;
    EQTableWidget *t = ui->tblDockHint;
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        t->setRowHidden(i, true);
        if (t->toInt(i, 0) == -1) {
            continue;
        }
        if (t->item(i, 0)->data(Qt::UserRole).toInt() == RESERVE_RESERVE &&
                WORKING_DATE.toString(def_date_format) == t->item(i, 4)->text()) {
            t->setRowHidden(i, false);
            continue;
        }
    }
}

void DWMainDeskHint::checkOutFilter()
{
    setWindowTitle(tr("Ready for checkout"));
    fCheckInFilter = true;
    EQTableWidget *t = ui->tblDockHint;
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        t->setRowHidden(i, true);
        if (t->toInt(i, 0) == -1) {
            continue;
        }
        if (t->item(i, 0)->data(Qt::UserRole).toInt() == RESERVE_CHECKIN &&
                WORKING_DATE.toString(def_date_format) == t->item(i, 5)->text()) {
            t->setRowHidden(i, false);
            continue;
        }
    }
}

void DWMainDeskHint::airFilter()
{
    setWindowTitle(tr("Air reservations"));
    fCheckInFilter = true;
    EQTableWidget *t = ui->tblDockHint;
    for (int i = 0, rowCount = t->rowCount(); i < rowCount; i++) {
        t->setRowHidden(i, true);
        if (t->toInt(i, 0) == -1) {
            continue;
        }
        if (t->item(i, 2)->data(Qt::DisplayRole).toInt() == 0) {
            t->setRowHidden(i, false);
            continue;
        }
    }
}

void DWMainDeskHint::hide()
{
    fCheckInFilter = false;
    QDockWidget::hide();
}

QTableWidget *DWMainDeskHint::tableWidget()
{
    return ui->tblDockHint;
}

void DWMainDeskHint::tblHeaderSectionClicked(int logicalIndex)
{
    if (fCheckInFilter) {
        return;
    }
    QSet<QString> values;
    for (int i = 0, count = ui->tblDockHint->rowCount(); i < count; i++) {
        values << ui->tblDockHint->item(i, logicalIndex)->text();
    }
    QStringList sortedValues = values.toList();
    std::sort(sortedValues.begin(), sortedValues.end());
    switch (logicalIndex) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7: {
        DlgFilterValues *d = new DlgFilterValues(sortedValues, this);
        if (d->exec() == QDialog::Accepted) {
            filterEqual(sortedValues, logicalIndex);
        }
        delete d;
    }
    }
}

void DWMainDeskHint::thisVisibilityChanged(bool v)
{
    if (!v) {
        fCheckInFilter = false;
        ui->btnCheckIn->setVisible(false);
    } else {
        setFocus();
    }
}

void DWMainDeskHint::on_btnCheckIn_clicked()
{
    hide();
    QList<CI_Room*> rooms;
    rooms.append(0);
    WReservation *w = addTab<WReservation>();
    w->setInitialParams(WORKING_DATE, WORKING_DATE, rooms);
}
