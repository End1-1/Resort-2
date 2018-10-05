#ifndef FCITYLEDGERDETAILEDBALANCE_H
#define FCITYLEDGERDETAILEDBALANCE_H

#include "wfilterbase.h"
#include "dwselectorcityledger.h"

namespace Ui {
class FCityLedgerDetailedBalance;
}

class FCityLedgerDetailedBalance : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCityLedgerDetailedBalance(QWidget *parent = 0);
    ~FCityLedgerDetailedBalance();
    virtual void apply(WReportGrid *rg);
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual bool officialTitle();
    virtual bool handlePrint();
    void setData(const QDate &date, const QString &cl);
    static void open();
private:
    Ui::FCityLedgerDetailedBalance *ui;
    DWSelectorCityLedger *fDockCL;
private slots:
    void cityLedger(CI_CityLedger *c);
    void openInvoice();
    void on_btnNextDate_clicked();
    void on_btnPrevDate_clicked();
    void on_btnChangeCityLedger_clicked();
};

#endif // FCITYLEDGERDETAILEDBALANCE_H
