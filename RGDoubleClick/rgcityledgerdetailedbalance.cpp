#include "rgcityledgerdetailedbalance.h"
#include "wreportgrid.h"
#include "fcityledgerdetailedbalance.h"

RGCityLedgerDetailedBalance::RGCityLedgerDetailedBalance(QObject *parent) :
    RGDoubleClickDelegate(parent)
{

}

void RGCityLedgerDetailedBalance::doubleClick(TableModel *t, int row)
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCityLedgerDetailedBalance *clb = new FCityLedgerDetailedBalance(rg);
    QDate date = QDate::currentDate();
    date = date.addDays(-1 * (date.day() - 1));
    QString cityLedger = t->data(row, 0, Qt::DisplayRole).toString();
    clb->setData(date, cityLedger);
    clb->apply(rg);
}
