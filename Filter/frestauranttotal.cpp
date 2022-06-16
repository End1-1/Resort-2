#include "frestauranttotal.h"
#include "ui_frestauranttotal.h"
#include "wreportgrid.h"
#include "dwselectordish.h"
#include "dwselectordishstate.h"
#include "dlgperemovereason.h"
#include "dlggposorderinfo.h"
#include "baseorder.h"
#include "storeoutput.h"
#include "paymentmode.h"
#include "cacheresthall.h"
#include "recalculatestoreoutputs.h"
#include "cacheusers.h"
#include "cacheresttable.h"
#include "cachepaymentmode.h"
#include <QPrinter>

#define sn_order_state 1
#define sn_table 2
#define sn_dish 3
#define sn_dish_state 4

FRestaurantTotal::FRestaurantTotal(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FRestaurantTotal)
{
    ui->setupUi(this);
    if (RIGHT(WORKING_USERGROUP, cr__o_cancelation)) {
        fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removeOrder()), this)->setFocusPolicy(Qt::ClickFocus);
    }
    if (r__(cr__remove_restaurant)) {
        fReportGrid->addToolBarButton(":/images/biohazard.png", tr("Eliminate!"), SLOT(removePermanently()), this)->setFocusPolicy(Qt::ClickFocus);
    }
    fReportGrid->addToolBarButton(":/images/puzzle.png", tr("Recalculate store"), SLOT(recalculateStore()), this)->setFocusPolicy(Qt::ClickFocus);
    fReportGrid->addToolBarButton(":/images/printer.png", tr("Print receipts"), SLOT(printReceipt()), this)->setFocusPolicy(Qt::ClickFocus);
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClick(QList<QVariant>)));
    fDockHall = new DWSelectorHall(this);
    fDockHall->configure();
    fDockHall->setSelector(ui->leHall);
    connect(fDockHall, SIGNAL(hall(CI_RestHall*)), this, SLOT(hall(CI_RestHall*)));
    fDockOrderState = new DWSelectorOrderState(this);
    fDockOrderState->configure();
    fDockOrderState->setSelector(ui->leState);
    fDockOrderState->setDialog(this, sn_order_state);
    DWSelectorRestTable *dwTable = new DWSelectorRestTable(this);
    dwTable->configure();
    dwTable->setSelector(ui->leTable);
    dwTable->setDialog(this, sn_table);
    fDockUsers = new DWSelectorUsers(this);
    fDockUsers->configure();
    fDockUsers->setSelector(ui->leStaff);
    connect(fDockUsers, SIGNAL(user(CI_User*)), this, SLOT(user(CI_User*)));
    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, SIGNAL(store(CI_RestStore*)), this, SLOT(store(CI_RestStore*)));
    fDockDishType = new DWSelectorDishType(this);
    fDockDishType->configure();
    fDockDishType->setSelector(ui->leDishType);
    connect(fDockDishType, SIGNAL(dishType(CI_RestDishType*)), this, SLOT(dishType(CI_RestDishType*)));
    DWSelectorDish *dwDish = new DWSelectorDish(this);
    dwDish->configure();
    dwDish->setSelector(ui->leDish);
    dwDish->setDialog(this, sn_dish);
    DWSelectorDishState *dwDishState = new DWSelectorDishState(this);
    dwDishState->configure();
    dwDishState->setSelector(ui->leDishState);
    dwDishState->setDialog(this, sn_dish_state);

    CI_OrderState *os = CacheOrderState::instance()->get(ORDER_STATE_CLOSED);
    if (os) {
        selector(sn_order_state, qVariantFromValue(os));
    }
    CI_DishState *ds = CacheDishState::instance()->get(DISH_STATE_READY);
    if (ds) {
        selector(sn_dish_state, qVariantFromValue(ds));
    }


    fReportGrid->fIncludes.clear();
    fReportGrid->fIncludes["oh.f_id"] = false;
    fReportGrid->fIncludes["oh.f_state"] = false;
    fReportGrid->fIncludes["os.f_" + def_lang] = false;
    fReportGrid->fIncludes["oh.f_datecash"] = false;
    fReportGrid->fIncludes["oh.f_dateopen"] = false;
    fReportGrid->fIncludes["oh.f_dateclose"] = false;
    fReportGrid->fIncludes["oh.f_hall"] = false;
    fReportGrid->fIncludes["h.f_name"] = false;
    fReportGrid->fIncludes["oh.f_table"] = false;
    fReportGrid->fIncludes["t.f_name"] = false;
    fReportGrid->fIncludes["oh.f_staff"] = false;
    fReportGrid->fIncludes["concat(u.f_firstname,' ',u.f_lastname)"] = false;
    fReportGrid->fIncludes["oh.f_cityLedger"] = false;
    fReportGrid->fIncludes["cl.f_name"] = false;
    fReportGrid->fIncludes["od.f_store"] = false;
    fReportGrid->fIncludes["s.f_name"] = false;
    fReportGrid->fIncludes["od.f_state"] = false;
    fReportGrid->fIncludes["ds.f_en"] = false;
    fReportGrid->fIncludes["d.f_type"] = false;
    fReportGrid->fIncludes["dt.f_" + def_lang] = false;
    fReportGrid->fIncludes["d.f_as"] = false;
    fReportGrid->fIncludes["od.f_dish"] = false;
    fReportGrid->fIncludes["d.f_" + def_lang] = false;
    fReportGrid->fIncludes["od.f_price"] = false;
    fReportGrid->fIncludes["oh.f_tax"] = false;
    fReportGrid->fIncludes["oh.f_paymentMode"] = false;
    fReportGrid->fIncludes["pm.f_" + def_lang] = false;
    fReportGrid->fIncludes["oh.f_paymentModeComment"] = false;
    fReportGrid->fIncludes["sum(od.f_qty)"] = true;
    fReportGrid->fIncludes["sum(od.f_total)"] = true;
    fReportGrid->fIncludes["count(oh.f_id)"] = false;
    fReportGrid->fIncludes["sum(oh.f_total)"] = false;
    fReportGrid->fIncludes["op.f_discountcard"] = false;
    fReportGrid->fIncludes["sum(op.f_cash)"] = false;
    fReportGrid->fIncludes["sum(op.f_card)"] = false;
    fReportGrid->fIncludes["sum(op.f_discount)"] = false;
    fReportGrid->fIncludes["sum(op.f_debt)"] = false;
    fReportGrid->fIncludes["sum(op.f_coupon)"] = false;
}

FRestaurantTotal::~FRestaurantTotal()
{
    delete ui;
}

void FRestaurantTotal::apply(WReportGrid *rg)
{
    QString order;
    bool countAmount = !ui->chDish->isChecked()
            && !ui->rb500No->isChecked()
            && !ui->rb500Yes->isChecked()
            && !ui->chStore->isChecked()
            && !ui->chDishType->isChecked()
            && !ui->chDishState->isChecked()
            && ui->leDish->fHiddenText.isEmpty()
            && ui->leStore->text().isEmpty()
            && ui->leDishType->text().isEmpty()
            && ui->leArmSoft->text().isEmpty()
            && ui->chArmSoft->isChecked();
    if (ui->chPaymentMode->isChecked()) {
        countAmount = true;
    }
    fReportGrid->fIncludes["sum(od.f_total)"] = !countAmount;
    fReportGrid->fIncludes["sum(od.f_qty)"] = !countAmount;
    fReportGrid->fIncludes["count(oh.f_id)"] = countAmount;
    fReportGrid->fIncludes["sum(oh.f_total)"] = countAmount;
    if (countAmount && ui->chPaymentMode->isChecked()) {
        fReportGrid->fIncludes["sum(oh.f_total)"] = false;
        fReportGrid->fIncludes["sum(op.f_cash)"] = true;
        fReportGrid->fIncludes["sum(op.f_card)"] = true;
        fReportGrid->fIncludes["sum(op.f_discount)"] = true;
        fReportGrid->fIncludes["sum(op.f_debt)"] = true;
        fReportGrid->fIncludes["sum(op.f_coupon)"] = true;
    }
    rg->fFieldsWidths.clear();

    rg->fFieldsWidths[tr("Order #")] = 100;
    rg->fFieldsWidths[tr("State code")] = 0;
    rg->fFieldsWidths[tr("State")] = 80;
    rg->fFieldsWidths[tr("Date")] = 100;
    rg->fFieldsWidths[tr("Opened")] = 120;
    rg->fFieldsWidths[tr("Closed")] = 120;
    rg->fFieldsWidths[tr("Hall code")] = 0;
    rg->fFieldsWidths[tr("Hall")] = 100;
    rg->fFieldsWidths[tr("Table code")] = 0;
    rg->fFieldsWidths[tr("Table")] = 50;
    rg->fFieldsWidths[tr("Staff code")] = 0;
    rg->fFieldsWidths[tr("Staff")] = 150;
    rg->fFieldsWidths[tr("City ledger code")] = 0;
    rg->fFieldsWidths[tr("City ledger")] = 150;
    rg->fFieldsWidths[tr("Store code")] = 0;
    rg->fFieldsWidths[tr("Store")] = 100;
    rg->fFieldsWidths[tr("Dish state code")] = 0;
    rg->fFieldsWidths[tr("Dish state")] = 100;
    rg->fFieldsWidths[tr("Dish type code")] = 0;
    rg->fFieldsWidths[tr("Dish type")] = 150;
    rg->fFieldsWidths[tr("Dish code")] = 80;
    rg->fFieldsWidths[tr("Dish")] = 200;
    rg->fFieldsWidths[tr("Price")] = 80;
    rg->fFieldsWidths[tr("ArmSoft")] = 50;
    rg->fFieldsWidths[tr("Tax")] = 80;
    rg->fFieldsWidths[tr("Payment mode code")] = 0;
    rg->fFieldsWidths[tr("P/M")] = 150;
    rg->fFieldsWidths[tr("P/M comment")] = 100;
    rg->fFieldsWidths[tr("Discount card")] = 100;
    rg->fFieldsWidths[tr("Discount amount")] = 80;
    rg->fFieldsWidths[tr("Qty")] = 80;
    rg->fFieldsWidths[tr("Total")] = 80;
    rg->fFieldsWidths[tr("Cash")] = 80;
    rg->fFieldsWidths[tr("Card")] = 80;
    rg->fFieldsWidths[tr("Debt")] = 80;
    rg->fFieldsWidths[tr("Discount")] = 80;
    rg->fFieldsWidths[tr("Coupon")] = 80;
              ;

    rg->fFields.clear();
    rg->fFields << "oh.f_id"
           << "oh.f_state"
           << "os.f_" + def_lang
           << "oh.f_datecash"
           << "oh.f_dateopen"
           << "oh.f_dateclose"
           << "oh.f_hall"
           << "h.f_name"
           << "oh.f_table"
           << "t.f_name"
           << "oh.f_staff"
           << "concat(u.f_firstName,' ',u.f_lastName)"
           << "oh.f_cityledger"
           << "cl.f_name"
           << "od.f_store"
           << "od.f_state"
           << "s.f_name"
           << "ds.f_en"
           << "d.f_type"
           << "dt.f_" + def_lang
           << "od.f_dish"
           << "d.f_" + def_lang
           << "od.f_price"
           << "d.f_as"
           << "oh.f_tax"
           << "oh.f_paymentMode"
           << "pm.f_" + def_lang
           << "oh.f_paymentModeComment"
           << "op.f_discountcard"
              ;
    if (countAmount) {
        if (!ui->chPaymentMode->isChecked()) {
            rg->fFields
                    << "count(oh.f_id)"
                    << "sum(oh.f_total)";
        } else {
            rg->fFields << "count(oh.f_id)"
                        << "sum(op.f_cash)"
                        << "sum(op.f_card)"
                        << "sum(op.f_debt)"
                        << "sum(op.f_discount)"
                        << "sum(op.f_coupon)";
        }
    } else {
        rg->fFields
           << "sum(od.f_qty)"
           << "sum(od.f_total)";
    }
    rg->fFieldTitles.clear();
    rg->fFieldTitles["oh.f_id"] = tr("Order #");
    rg->fFieldTitles["oh.f_state"] = tr("State code");
    rg->fFieldTitles["os.f_" + def_lang] = tr("State");
    rg->fFieldTitles["oh.f_datecash"] = tr("Date");
    rg->fFieldTitles["oh.f_dateopen"] = tr("Opened");
    rg->fFieldTitles["oh.f_dateclose"] = tr("Closed");
    rg->fFieldTitles["oh.f_hall"] = tr("Hall code");
    rg->fFieldTitles["h.f_name"] = tr("Hall");
    rg->fFieldTitles["oh.f_table"] = tr("Table code");
    rg->fFieldTitles["t.f_name"] = tr("Table");
    rg->fFieldTitles["oh.f_staff"] = tr("Staff code");
    rg->fFieldTitles["concat(u.f_firstname,' ',u.f_lastname)"] = tr("Staff");
    rg->fFieldTitles["oh.f_cityledger"] = tr("City ledger code");
    rg->fFieldTitles["cl.f_name"] = tr("City ledger");
    rg->fFieldTitles["od.f_store"] = tr("Store code");
    rg->fFieldTitles["s.f_name"] = tr("Store");
    rg->fFieldTitles["od.f_state"] = tr("Dish state code");
    rg->fFieldTitles["ds.f_en"] = tr("Dish state");
    rg->fFieldTitles["d.f_type"] = tr("Dish type code");
    rg->fFieldTitles["dt.f_" + def_lang] = tr("Dish type");
    rg->fFieldTitles["od.f_dish"] = tr("Dish code");
    rg->fFieldTitles["d.f_" + def_lang] = tr("Dish");
    rg->fFieldTitles["od.f_price"] = tr("Price");
    rg->fFieldTitles["d.f_as"] = tr("ArmSoft");
    rg->fFieldTitles["oh.f_tax"] = tr("Tax");
    rg->fFieldTitles["oh.f_paymentMode"] = tr("Payment mode code");
    rg->fFieldTitles["pm.f_" + def_lang] = tr("P/M");
    rg->fFieldTitles["oh.f_paymentModeComment"] = tr("P/M comment");
    rg->fFieldTitles["op.f_discountcard"] = tr("Discount card");
    rg->fFieldTitles["sum(op.f_discount)"] = tr("Discount amount");
    rg->fFieldTitles["count(oh.f_id)"] = tr("Qty");
    rg->fFieldTitles["sum(oh.f_total)"] = tr("Total");
    rg->fFieldTitles["sum(od.f_qty)"] = tr("Qty");
    rg->fFieldTitles["sum(od.f_total)"] = tr("Total");
    rg->fFieldTitles["sum(op.f_cash)"] = tr("Cash");
    rg->fFieldTitles["sum(op.f_card)"] = tr("Card");
    rg->fFieldTitles["sum(op.f_debt)"] = tr("Debt");
    rg->fFieldTitles["sum(op.f_discount)"] = tr("Discount");
    rg->fFieldTitles["sum(op.f_coupon)"] = tr("Coupon");

    rg->fTables.clear();
    rg->fTables << "o_header oh"
           << "o_dish od"
           << "o_state os"
           << "r_hall h"
           << "r_table t"
           << "r_store s"
           << "r_dish_type dt"
           << "r_dish d"
           << "users u"
           << "f_city_ledger cl"
           << "f_payment_type pm"
           << "o_dish_state ds"
           << "o_header_payment op";
    rg->fJoins.clear();
    rg->fJoins << "from" //od
          << "inner" //oh
          << "inner" //os
          << "inner" //h
          << "inner" //t
          << "inner" //s
          << "inner" //dt
          << "inner" // d
          << "inner" //u
          << "left" //cl
          << "inner" //pm
          << "left" //ds
          << "left" //op
             ;
    rg->fJoinConds.clear();
    rg->fJoinConds << ""
              << "od.f_header=oh.f_id"
              << "os.f_id=oh.f_state"
              << "h.f_id=oh.f_hall"
              << "t.f_id=oh.f_table"
              << "s.f_id=od.f_store"
              << "dt.f_id=d.f_type"
              << "d.f_id=od.f_dish"
              << "u.f_id=oh.f_staff"
              << "cl.f_id=oh.f_cityLedger"
              << "pm.f_id=oh.f_paymentMode"
              << "ds.f_id=od.f_state"
              << "op.f_id=oh.f_id"
                 ;

    QString where = "where (oh.f_dateCash between '" + ui->deStart->date().toString(def_mysql_date_format) + "' "
            + " and '" + ui->deEnd->date().toString(def_mysql_date_format) + "') ";
    if (!ui->leOrder->text().isEmpty()) {
        where += " and oh.f_id in (" + ui->leOrder->text() + ") ";
    }
    if (!ui->leState->text().isEmpty()) {
        where += " and oh.f_state in (" + ui->leState->fHiddenText + ") ";
    }
    if (!ui->leHall->text().isEmpty()) {
        where += " and oh.f_hall in (" + ui->leHall->fHiddenText + ") ";
    }
    if (!ui->leTable->text().isEmpty()) {
        where += " and oh.f_table in (" + ui->leTable->fHiddenText + ") ";
    }
    if (!ui->leStaff->text().isEmpty()) {
        where += " and oh.f_staff in (" + ui->leStaff->fHiddenText + ") ";
    }
    if (!countAmount) {
        where += " and od.f_dish<>558 ";
        if (!ui->leDishState->text().isEmpty()) {
            where += " and od.f_state in (" + ui->leDishState->fHiddenText + ") ";
        }
    }
    if (!countAmount) {
        if (ui->chOnlyZeroes->isChecked()) {
            where += " and od.f_total=0 ";
        }
    } else {
        if (ui->chOnlyZeroes->isChecked()) {
            where += " and oh.f_total=0 ";
        }
    }
    if (ui->rb500Yes->isChecked()) {
        where += " and od.f_dish in (159,171,158,169,153,165,386,387,388,389,390,391) ";
    } else if (ui->rb500No->isChecked()) {
        where += " and od.f_dish not in (159,171,158,169,153,165,386,387,388,389,390,391) ";
    }
    if (!ui->leStore->text().isEmpty()) {
        where += " and od.f_store in (" + ui->leStore->fHiddenText + ") ";
    }
    if (!ui->leDishType->text().isEmpty()) {
        where += " and d.f_type in (" + ui->leDishType->fHiddenText + ") ";
    }
    if (!ui->leDish->text().isEmpty()) {
        where += " and od.f_dish in (" + ui->leDish->fHiddenText + ") ";
    }
    if (!ui->leArmSoft->text().isEmpty()) {
        where += " and d.f_as in(" + ui->leArmSoft->text() + ") ";
    }

    if (!ui->lePMComment->text().isEmpty()) {
        where += " and upper(oh.f_paymentModeComment) like '" + ui->lePMComment->text() + "%' ";
    }
    if (!ui->leTax->text().isEmpty()) {
        where += " and oh.f_tax in (" + ui->leTax->text() + ") ";
    }
    if (ui->rbNoShowComplex->isChecked() && !countAmount) {
        where += " and (od.f_complex=0 or (od.f_complex>0 and od.f_complexId>0)) ";
    }
    if (ui->rbShowComplex->isChecked()) {

    }
    if (ui->rbOnlycomplex->isChecked() && !countAmount) {
        where += " and (od.f_complex>0 and od.f_complexId=0) ";
    }

    if (ui->chOrderNum->isChecked()) {
        order += "oh.f_id,";
    }
    if (ui->rbWithCard->isChecked()) {
        where += " and length(op.f_discountcard)>0 ";
    }
    if (ui->rbNoCard->isChecked()) {
        where += " and length(op.f_discountcard)=0 ";
    }
    QString group;
    QObjectList ol(children());
    bool first = true;
    foreach (QObject *o, ol) {
        if (isCheckBox(static_cast<QWidget*>(o))) {
            EQCheckBox *check = static_cast<EQCheckBox*>(o);
            if (check->isChecked()) {
                if (first) {
                    first = false;
                } else {
                    group += ",";
                }
                group += check->getField().replace(";", ",");
                if (check->getRequireLang()) {
                    group += def_lang;
                }
            }
        }
    }
    group = group.replace("op.f_cash,op.f_card,op.f_discount,op.f_debt,op.f_coupon,", "");
    if (group.length() > 0) {
        if (group.at(group.length() - 1) == ",") {
            group.remove(group.length() - 1, 1);
        }
    }
    if (!group.isEmpty()) {
        group = " group by " + group;
    }
    if (!order.isEmpty()) {
        order = " order by " + order;
    }
    where += group;
    order.remove(order.length() - 1, 1);
    where += order;
    buildQuery(rg, where);
    QList<int> colsTotal;
    if (!ui->chPaymentMode->isChecked()) {
        colsTotal << rg->fModel->columnIndex(tr("Qty"))
              << rg->fModel->columnIndex(tr("Total"))
                 ;
    } else {
        colsTotal << rg->fModel->columnIndex(tr("Qty"))
              << rg->fModel->columnIndex(tr("Cash"))
              << rg->fModel->columnIndex(tr("Card"))
              << rg->fModel->columnIndex(tr("Debt"))
              << rg->fModel->columnIndex(tr("Discount"))
              << rg->fModel->columnIndex(tr("Coupon"));
    }
    QList<double> valsTotal;
    rg->fModel->sumOfColumns(colsTotal, valsTotal);
    rg->setTblTotalData(colsTotal, valsTotal);
    if (ui->chOrderNum->isChecked()) {
        QColor dark = COLOR_DARK_ROW;
        QColor currColor = Qt::white;
        QString currId ;
        for (int i = 0, count = rg->fModel->rowCount(); i < count; i++) {
            if (rg->fModel->data(i, 0).toString() != currId) {
                currId = rg->fModel->data(i, 0).toString();
                currColor = (currColor == Qt::white ? dark : Qt::white);
            }
            rg->fModel->setBackgroundColor(i, currColor);
        }
    }
}

QWidget *FRestaurantTotal::firstElement()
{
    return ui->deStart;
}

QString FRestaurantTotal::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Earnings"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FRestaurantTotal::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    rg->setupTabTextAndIcon(tr("Earnings"), ":/images/cutlery.png");
    FRestaurantTotal *fr = new FRestaurantTotal(rg);
    rg->addFilterWidget(fr);
    fr->apply(rg);
}

void FRestaurantTotal::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case sn_order_state:
        dockResponse<CI_OrderState, CacheOrderState>(ui->leState, value.value<CI_OrderState*>());
        break;
    case sn_table:
        dockResponse<CI_RestTable, CacheRestTable>(ui->leTable, value.value<CI_RestTable*>());
        break;
    case sn_dish:
        dockResponse<CI_Dish, CacheDish>(ui->leDish, value.value<CI_Dish*>());
        break;
    case sn_dish_state:
        dockResponse<CI_DishState, CacheDishState>(ui->leDishState, value.value<CI_DishState*>());
        break;
    }
}

void FRestaurantTotal::printNewPage(int &top, int &left, int &page, PPrintPreview *pp, PPrintScene *&ps, int nextHeight)
{
    int footerTop = sizePortrait.height() - 200;
    QBrush b(Qt::white, Qt::SolidPattern);
    PTextRect trFooter;
    trFooter.setBrush(b);
    QFont ffooter(qApp->font().family(), 20);
    trFooter.setFont(ffooter);
    trFooter.setBorders(false, false, false, false);
    trFooter.setTextAlignment(Qt::AlignLeft);

    if (top + nextHeight > sizePortrait.height() - 300) {
        if (left == 20) {
            left = 1100;
        } else {
            left = 20;
            ps->addTextRect(20, footerTop, 1800, 60, QString("%1: %2 %3")
                            .arg(tr("Printed"))
                            .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                            .arg(WORKING_USERNAME), &trFooter);
            trFooter.setTextAlignment(Qt::AlignRight);
            ps->addTextRect(1800, footerTop, 200, 60, QString("%1 %2")
                            .arg(tr("Page"))
                            .arg(page), &trFooter);
            trFooter.setTextAlignment(Qt::AlignLeft);
            ps = pp->addScene(0, Portrait);
            page++;
        }
        top = 20;
    }
}

void FRestaurantTotal::printReceipt()
{
    if (!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }
    QModelIndexList sel = fReportGrid->fTableView->selectionModel()->selectedIndexes();
    if (sel.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    QSet<QString> orders;
    foreach (QModelIndex m, sel) {
        orders.insert(fReportGrid->fModel->data(m.row(), 0, Qt::EditRole).toString());
    }
    if (orders.count() == 0) {
        return;
    }

    int left = 20;
    int top = 20;
    int page = 1;

    PTextRect prTempl;
    prTempl.setWrapMode(QTextOption::NoWrap);
    prTempl.setFont(QFont(qApp->font().family(), 17));
    prTempl.setBorders(true, true, true, true);
    QPen pen;
    pen.setWidth(2);
    prTempl.setRectPen(pen);
    PTextRect prHead(prTempl, "");
    prHead.setFont(QFont(qApp->font().family(), 20));
    prHead.setBrush(QBrush(QColor::fromRgb(215, 215, 215), Qt::SolidPattern));
    prHead.setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    PPrintPreview pp(this);
    PPrintScene *ps = pp.addScene(0, Portrait);
    PTextRect th;
    QFont f("Arial", 30);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    th.setTextAlignment(Qt::AlignLeft);
    int rowHeight = 60;

    CacheRestHall *hall = CacheRestHall::instance();
    CacheRestTable *table = CacheRestTable::instance();
    CacheUsers *users = CacheUsers::instance();
    //CachePaymentMode *pm = CachePaymentMode::instance();
    foreach (QString s, orders) {
        printNewPage(top, left, page, &pp, ps, 250);
        PImage *logo = new PImage("logo_print.png");
        ps->addItem(logo);
        logo->setRect(QRectF(left + 150, top, 400, 250));
        top += 260;
        printNewPage(top, left, page, &pp, ps);

        DatabaseResult dh;
        fDbBind[":f_id"] = s;
        dh.select(fDb, "select * from o_header where f_id=:f_id", fDbBind);
        top += ps->addTextRect(left + 10, top, 680, rowHeight, hall->get(dh.value("f_hall").toString())->fName, &prHead)
                ->textHeight();
        QString receiptNum = QString("%1 %2").arg(tr("Receipt S/N")).arg(s);
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(left + 10, top, 680, rowHeight, receiptNum, &prHead)->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Table"), &th, f));
        ps->addTextRect(new PTextRect(left + 160, top, 200, rowHeight, table->get(dh.value("f_table").toString())->fName, &th, f));
        ps->addTextRect(new PTextRect(left + 340, top, 230, rowHeight, tr("Date"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(450, top, 250, rowHeight, dh.value("f_datecash").toDate().toString(def_date_format), &th, f))
                ->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Time"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 200, rowHeight, QTime::currentTime().toString(def_time_format), &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Waiter"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 500, rowHeight, users->get(dh.value("f_staff").toString())->fFull, &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Opened"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 350, rowHeight, dh.value("f_dateOpen").toDateTime().toString(def_date_time_format), &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Closed"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 350, rowHeight, dh.value("f_dateClose").toDateTime().toString(def_date_time_format), &th, f))->textHeight();

        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += 2;
        ps->addLine(left + 10, top, left + 680, top);
        top ++;
        ps->addTextRect(new PTextRect(left + 10, top, 100, rowHeight, tr("Qty"), &th, f));
        ps->addTextRect(new PTextRect(left + 110, top, 390, rowHeight, tr("Description"), &th, f));
        top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, tr("Amount"), &th, f))->textHeight();
        ps->addLine(left + 10, top, left + 680, top);
        top ++;


        DatabaseResult dcomplex;
        fDbBind[":f_header"] = s;
        dcomplex.select(fDb, "select od.f_id, od.f_dish, dc.f_en, dc.f_ru, dc.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
                             "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
                             "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
                             "dc.f_adgt "
                             "from o_dish od "
                             "left join r_dish_complex dc on dc.f_id=od.f_complexId "
                             "where od.f_header=:f_header and f_complex>0 and f_complexId>0 and f_state=1 "
                             "order by od.f_id ", fDbBind);
        f.setPointSize(18);
        f.setBold(true);
        th.setFont(f);
        for (int i = 0; i < dcomplex.rowCount(); i++) {
            ps->addTextRect(new PTextRect(left + 10, top, 100, rowHeight, float_str(dcomplex.value(i, "f_qty").toDouble(), 1), &th, f));
            ps->addTextRect(new PTextRect(left + 110, top, 390, rowHeight, dcomplex.value(i, "f_en").toString(), &th, f));
            top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, float_str(dcomplex.value(i, "f_price").toDouble() * dcomplex.value(i, "f_qty").toDouble(), 2), &th, f))->textHeight();
            printNewPage(top, left, page, &pp, ps);
        }
        DatabaseResult ddish;
        fDbBind[":f_header"] = s;
        ddish.select(fDb, "select od.f_id, od.f_dish, d.f_en, d.f_ru, d.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
                          "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
                          "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
                          "od.f_adgt, od.f_complexRec "
                          "from o_dish od "
                          "left join r_dish d on d.f_id=od.f_dish "
                          "where od.f_header=:f_header and (f_complex=0 or (f_complex>0 and f_complexId=0)) and f_state=1 "
                          "order by od.f_id ", fDbBind);

        for (int i = 0; i < ddish.rowCount(); i++) {
            ps->addTextRect(new PTextRect(left + 10, top, 100, rowHeight, float_str(ddish.value(i, "f_qty").toDouble(), 1), &th, f));
            ps->addTextRect(new PTextRect(left + 110, top, 390, rowHeight, ddish.value(i, "f_en").toString(), &th, f));
            top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, float_str(ddish.value(i, "f_total").toDouble(), 2), &th, f))->textHeight();
            printNewPage(top, left, page, &pp, ps);
        }
        ps->addLine(left + 10, top, left + 680, top);
        top += 2;
        f.setPointSize(24);
        th.setFont(f);
        ps->addTextRect(new PTextRect(left + 10, top, 400, rowHeight, tr("Total, AMD"), &th, f));
        top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, float_str(dh.value("f_total").toDouble(), 0), &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 400, rowHeight, tr("Total, USD"), &th, f));
        top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, float_str(dh.value("f_total").toDouble() / def_usd, 2), &th, f))->textHeight();

        top += rowHeight;
        f.setPointSize(28);
        th.setFont(f);
        th.setTextAlignment(Qt::AlignHCenter);
        printNewPage(top, left, page, &pp, ps);
        if (!dh.value("f_roomComment").toString().isEmpty()) {
           top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, dh.value("f_roomComment").toString(), &th, f))->textHeight();
           top += rowHeight;
           top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("Signature"), &th, f))->textHeight();
           top += rowHeight + 2;
           ps->addLine(left + 150, top, left + 680, top);
        }

        printNewPage(top, left, page, &pp, ps);
        if (dh.value("f_paymentMode").toInt() == PAYMENT_COMPLIMENTARY) {
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("COMPLIMENTARY"), &th, f))->textHeight();
        } else {
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("SALES"), &th, f))->textHeight();
        }
        if (dh.value("f_paymentMode").toInt()) {
            top += rowHeight;
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("Mode Of Payment"), &th, f))->textHeight();
            switch (dh.value("f_paymentMode").toInt()) {
            case PAYMENT_CASH:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("CASH"), &th, f))->textHeight();
                break;
            case PAYMENT_CARD:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("CARD") + "/" + dh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
                break;
            case PAYMENT_ROOM:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, dh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
                break;
            case PAYMENT_CL:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, "CL/" + dh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
                break;
            case PAYMENT_COMPLIMENTARY:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, dh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
                break;
            }
        } else {
            th.setTextAlignment(Qt::AlignLeft);
            top += rowHeight;
            ps->addTextRect(left + 10, top, 400, rowHeight, tr("Room number:"), &th);
            top += 2;
            ps->addLine(left + 300, top + rowHeight, left + 600, top + rowHeight);
            top += rowHeight + 2;
            ps->addTextRect(left + 10, top, 200, rowHeight, tr("Signature"), &th);
            top += 2;
            ps->addLine(left + 300, top + rowHeight, left + 600, top + rowHeight);
        }
        top++;
        ps->addLine(left + 10, top, left + 680, top, QPen(QBrush(Qt::SolidPattern), 5));
        top += 20;

        printNewPage(top, left, page, &pp, ps);
    }

    int footerTop = sizePortrait.height() - 200;
    QBrush b(Qt::white, Qt::SolidPattern);
    PTextRect trFooter;
    trFooter.setBrush(b);
    QFont ffooter(qApp->font().family(), 20);
    trFooter.setFont(ffooter);
    trFooter.setBorders(false, false, false, false);
    trFooter.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(20, footerTop, 1800, 60, QString("%1: %2 %3")
                    .arg(tr("Printed"))
                    .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                    .arg(WORKING_USERNAME), &trFooter);
    trFooter.setTextAlignment(Qt::AlignRight);
    ps->addTextRect(1800, footerTop, 200, 60, QString("%1 %2")
                    .arg(tr("Page"))
                    .arg(page), &trFooter);
    trFooter.setTextAlignment(Qt::AlignLeft);

    pp.exec();

}

void FRestaurantTotal::recalculateStore()
{
    if (!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }
    if (message_yesnocancel(tr("Confirm to recalculate store outputs")) != RESULT_YES) {
        return;
    }
    QSet<int> ids;
    for (int i = 0; i < fReportGrid->fModel->rowCount(); i++) {
        ids.insert(fReportGrid->fModel->data(i, 0, Qt::EditRole).toInt());
    }
    RecalculateStoreOutputs *r = new RecalculateStoreOutputs(ids, this);
    r->exec();
    delete r;
}

void FRestaurantTotal::removeOrder()
{
    if (!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }
    QList<QVariant> val;
    if (fReportGrid->fillRowValuesOut(val) < 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    if (message_yesnocancel(tr("Confirm to remove the order")) != RESULT_YES) {
        return;
    }
    DlgPERemoveReason *d = new DlgPERemoveReason(this);
    int state = d->exec();
    if (state == QDialog::Rejected) {
        return;
    }
    fDb.fDb.transaction();
    fDbBind[":f_id"] = val.at(0);
    fDb.select("select f_discountcard, f_discount from o_header_payment where f_id=:f_id", fDbBind, fDbRows);
    double discamount = 0.0;
    QString disccard = fDbRows.at(0).at(0).toString();
    if (disccard.isEmpty() == false){
        discamount = fDbRows.at(0).at(1).toDouble();
        fDbBind[":f_card"] = disccard;
        fDb.select("select f_mode from d_car_client where f_card=:f_card", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            QStringList params = fDbRows.at(0).at(0).toString().split(";", Qt::SkipEmptyParts);
            if (params.count() == 4) {
                params[2] = QString::number(params[2].toDouble() + discamount, 'f', 0);
                fDbBind[":f_card"] = disccard;
                fDbBind[":f_mode"] = params.join(";") + ";";
                fDb.select("update d_car_client set f_mode=:f_mode where f_card=:f_card", fDbBind, fDbRows);
            }
        }
    }

    fDbBind[":f_state"] = ORDER_STATE_REMOVED;
    fDbBind[":f_comment"] = "Canceled by " + WORKING_USERNAME;
    fDb.update("o_header", fDbBind, where_id(val.at(0).toInt()));
    fDbBind[":f_state"] = state;
    fDbBind[":f_state_cond"] = DISH_STATE_READY;
    fDbBind[":f_header"] = val.at(0);
    fDbBind[":f_comment"] = "Canceled by " + WORKING_USERNAME;
    fDb.select("update o_dish set f_state=:f_state, f_comment=:f_comment "
                   "where f_header=:f_header and f_state=:f_state_cond", fDbBind, fDbRows);
    fDbBind[":f_id"] = val.at(0);
    fDbBind[":f_cancelReason"] = "Canceled by " + WORKING_USERNAME;
    fDb.select("update m_register set f_canceled=1, f_cancelReason=:f_cancelReason where f_id=:f_id",
               fDbBind, fDbRows);
    StoreOutput so(fDb, val.at(0).toInt());
    so.rollbackSale(fDb, val.at(0).toInt());
    fDb.fDb.commit();
    message_info_tr("Please, refresh report to view the changes");
}

void FRestaurantTotal::removePermanently()
{
    if (!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }
    QList<QVariant> val;
    if (fReportGrid->fillRowValuesOut(val) < 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    if (message_yesnocancel(tr("<h1><b>Confirm to remove the order<br>PERMANENTLY</b></h1>")) != RESULT_YES) {
        return;
    }
    fDb.fDb.transaction();
    fDbBind[":f_header"] = val.at(0);
    fDb.select("delete from o_dish where f_header=:f_header", fDbBind, fDbRows);
    fDbBind[":f_id"] = val.at(0);
    fDb.select("delete from o_header where f_id=:f_id", fDbBind, fDbRows);
    fDbBind[":f_id"] = val.at(0);
    fDb.select("delete from m_register where f_id=:f_id", fDbBind, fDbRows);
    StoreOutput so(fDb, val.at(0).toInt());
    so.rollbackSale(fDb, val.at(0).toInt());
    fDb.fDb.commit();
    message_info_tr("Please, refresh report to view the changes");
}

void FRestaurantTotal::hall(CI_RestHall *c)
{
    dockResponse<CI_RestHall, CacheRestHall>(ui->leHall, c);
}

void FRestaurantTotal::user(CI_User *c)
{
    dockResponse<CI_User, CacheUsers>(ui->leStaff, c);
}

void FRestaurantTotal::store(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);
}

void FRestaurantTotal::dishType(CI_RestDishType *c)
{
    dockResponse<CI_RestDishType, CacheRestDishType>(ui->leDishType, c);
}

void FRestaurantTotal::doubleClick(const QList<QVariant> &row)
{
    if (!ui->chOrderNum->isChecked()) {
        message_error(tr("Order number checkbox must be checked"));
        return;
    }
    if (row.count() == 0) {
        message_error(tr("Nothing is selected"));
        return;
    }
    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(row.at(0).toString());
    d->exec();
    delete d;
}

void FRestaurantTotal::on_btnPrevDate_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FRestaurantTotal::on_btnNextDate_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}

void FRestaurantTotal::on_chShowDiscount_clicked(bool checked)
{
    fReportGrid->fIncludes["op.f_discountcard"] = checked;
    fReportGrid->fIncludes["op.f_discount"] = checked;
    if (checked) {
        ui->chOrderNum->setChecked(true);
    }
}
