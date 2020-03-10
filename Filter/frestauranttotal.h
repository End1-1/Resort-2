#ifndef FRESTAURANTTOTAL_H
#define FRESTAURANTTOTAL_H

#include "wfilterbase.h"
#include "dwselectorhall.h"
#include "dwselectororderstate.h"
#include "dwselectorresttable.h"
#include "dwselectorusers.h"
#include "dwselectorcityledger.h"
#include "dwselectorreststore.h"
#include "dwselectordishtype.h"
#include "dwselectordish.h"
#include "dwselectorpaymentmode.h"
#include "ptextrect.h"
#include "pimage.h"
#include "pprintpreview.h"

namespace Ui {
class FRestaurantTotal;
}

class WReportGrid;

class FRestaurantTotal : public WFilterBase
{
    Q_OBJECT

public:
    explicit FRestaurantTotal(QWidget *parent = 0);
    ~FRestaurantTotal();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    QString reportTitle();
    static void open();

protected:
    virtual void selector(int selectorNumber, const QVariant &value);

private:
    Ui::FRestaurantTotal *ui;
    DWSelectorHall *fDockHall;
    DWSelectorOrderState *fDockOrderState;
    DWSelectorUsers *fDockUsers;
    DWSelectorRestStore *fDockStore;
    DWSelectorDishType *fDockDishType;
    DWSelectorPaymentMode *fDockPay;
    void printNewPage(int &top, int &left, int &page, PPrintPreview *pp, PPrintScene *&ps, int nextHeight = 0);

private slots:
    void printReceipt();
    void removeOrder();
    void removePermanently();
    void hall(CI_RestHall *c);
    void user(CI_User *c);
    void cityLedger(CI_CityLedger *c);
    void store(CI_RestStore *c);
    void dishType(CI_RestDishType *c);
    void on_btnOrdersSubtotal_clicked();
    void doubleClick(const QList<QVariant> &row);
    void on_btnPrevDate_clicked();
    void on_btnNextDate_clicked();
    void on_chShowDiscount_clicked(bool checked);
};

#endif // FRESTAURANTTOTAL_H
