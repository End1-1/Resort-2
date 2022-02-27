#include "rdesk.h"
#include "ui_rdesk.h"
#include "paymentmode.h"
#include "rchangelanguage.h"
#include "dlgdeptholder.h"
#include "rchangemenu.h"
#include "rmessage.h"
#include "rpaymenttype.h"
#include "cachecar.h"
#include "rselecttable.h"
#include "rdishcomment.h"
#include "dlgsmile.h"
#include "rtools.h"
#include "baseuid.h"
#include "dlgsalary.h"
#include "rlogin.h"
#include "rnumbers.h"
#include "cacherights.h"
#include "rmodifiers.h"
#include "dlgreservation.h"
#include "pprintreceipt.h"
#include "ptextrect.h"
#include "pimage.h"
#include "c5printing.h"
#include "pprintscene.h"
#include "dlgcomplexdish.h"
#include "cacheinvoiceitem.h"
#include "dlgcheckcard.h"
#include "logging.h"
#include "cacherights.h"
#include "dlgpayment.h"
#include "cacheusers.h"
#include "dlggettext.h"
#include "reportprint.h"
#include "printtax.h"
#include "dlgselecttaxcashmode.h"
#include "dlgvoidback.h"
#include "dlgcarselection.h"
#include "dlglist.h"
#include "rlogin.h"
#include "databaseresult.h"
#include "dlgdate.h"
#include "vauchers.h"
#include "dlgorders.h"
#include <QItemDelegate>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QInputDialog>
#include <QDir>

QMap<int, DishStruct*> RDesk::fQuickDish;

class PartItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, QBrush());
        if (!index.isValid()) {
            return;
        }
        DishPartStruct *p = index.data(Qt::UserRole).value<DishPartStruct*>();
        if (!p) {
            return;
        }
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(190, 240, 254, 255));
        }
        QFont f(qApp->font());
        f.setBold(true);
        painter->setFont(f);
        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QRect textRect = option.rect;
        textRect.adjust(3, 3, -3, -3);
        painter->drawText(textRect, p->fName[def_lang], o);
    }
};

class TypeItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, Qt::white);
        TypeStruct *t = index.data(Qt::UserRole).value<TypeStruct*>();
        if (!t) {
            return;
        }
        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        o.setWrapMode(QTextOption::WordWrap);
        painter->fillRect(option.rect, QColor::fromRgb(t->fBgColor));
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(42,42, 42));
            painter->setPen(Qt::white);
        } else {
            painter->setPen(QColor::fromRgb(t->fTextColor));
        }
        QRect textRect = option.rect;
        textRect.adjust(3, 3, -3, -3);
        QFont f(qApp->font());
        f.setPointSize(14);
        painter->setFont(f);
        painter->drawText(textRect, t->fName[def_lang], o);
    }
};

class DishItemDelegate : public QItemDelegate {
public:
    DishItemDelegate(RDesk *desk) {
        fDesk = desk;
    }
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, Qt::white);
        if (!index.isValid()) {
            return;
        }
        DishStruct *d = index.data(Qt::UserRole).value<DishStruct*>();
        if (!d) {
            return;
        }
        QTextOption o;
        o.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        o.setWrapMode(QTextOption::WordWrap);
        painter->fillRect(option.rect, QColor::fromRgb(d->fBgColor));
        QRect textRect = option.rect;
        textRect.adjust(3, 3, -3, -3);
        painter->setPen(QColor::fromRgb(d->fTextColor));
        QFont f(qApp->font());
        f.setPointSize(12);
        f.setBold(true);
        painter->setFont(f);
        QString text = QString("%1 [%2]")
                .arg(d->fName[def_lang])
                .arg(QString::number(d->fPrice, 'f', 0));
        painter->drawText(textRect, text, o);
    }
private:
    RDesk *fDesk;
};

class OrderDishDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->save();
        painter->fillRect(option.rect, Qt::white);
        if (!index.isValid()) {
            return;
        }
        OrderDishStruct *od = index.data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            return;
        }
        QFont f(qApp->font());
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(42, 42, 42));
            painter->setPen(Qt::white);
            f.setBold(true);
            painter->setFont(f);
        } else {
            painter->setPen(QColor::fromRgb(42, 42, 42));
        }
        if (od->fState != DISH_STATE_READY) {
            f.setStrikeOut(true);
            painter->setFont(f);
        }
        QTextOption o;
        o.setWrapMode(QTextOption::WordWrap);
        switch (index.column()) {
        case 0: {
            QRect textRect = option.rect;
            textRect.adjust(2, 2, -2, -2);
            painter->drawText(textRect, od->fName[def_lang], o);
            f.setPointSize(8);
            painter->setFont(f);
            int h = QFontMetrics(f).height();
            QRect commentRect = textRect;
            commentRect.adjust(0, commentRect.height() - h - 1, 0, 0);
            painter->drawText(commentRect, od->fComment, o);
            if (od->fComplex > 0) {
                QRect imageRect = option.rect;
                imageRect.adjust(imageRect.width() - 20, imageRect.height() - 20, -5, -5);
                QImage img(":/images/dinner.png");
                painter->drawImage(imageRect, img);
            }
            break;
        }
        case 1: {
            o.setAlignment(Qt::AlignHCenter);
            QRect qtyRect = option.rect;
            qtyRect.adjust(2, 2, -2, (qtyRect.height() / 2) * -1);
            f.setBold(true);
            painter->setFont(f);
            painter->drawText(qtyRect, float_str(od->fQty, 1), o);

            QRect printRect = option.rect;
            printRect.adjust(2, (printRect.height() / 2), -2, -2);
            f.setBold(false);
            painter->setFont(f);
            painter->drawText(printRect, float_str(od->fQtyPrint, 1), o);

            painter->drawLine(qtyRect.left(), qtyRect.bottom(), qtyRect.right(), qtyRect.bottom());
            break;
        }
        case 2: {
            o.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            QRect totalRect = option.rect;
            totalRect.adjust(2, 2, -2, -2);
            painter->drawText(totalRect, float_str(od->fTotal, 2), o);
            break;
        }
        }
        painter->restore();
    }
};

class TotalItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->setPen(Qt::white);
        QItemDelegate::paint(painter, option, index);

    }
};

class TablesItemDeletgate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QItemDelegate::paint(painter, option, index);
        TableStruct *t = index.data(Qt::UserRole).value<TableStruct*>();
        if (!t) {
            return;
        }
        painter->fillRect(option.rect, Qt::black);
        painter->setPen(Qt::white);
        if (t->fAmount.toDouble() > 0.1) {
            painter->fillRect(option.rect, Qt::yellow);
            painter->setPen(Qt::black);
        }
        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QRect r = option.rect;
        r.adjust(0, 0, 0, -20);
        painter->drawText(r, t->fName, o);
        r = option.rect;
        r.adjust(0, r.height() / 2, 0, 0);
        painter->drawText(r, t->fAmount, o);
    }
};

RDesk::RDesk(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RDesk)
{
    ui->setupUi(this);
    ui->tblComplex->setVisible(false);
    fCanClose = false;
    fShowRemoved = false;
    fTable  = 0;
    fCloseTimeout = 0;
    ui->tblType->setItemDelegate(new TypeItemDelegate());
    ui->tblDish->setItemDelegate(new DishItemDelegate(this));
    ui->tblOrder->setItemDelegate(new OrderDishDelegate());
    ui->tblTotal->setItemDelegate(new TotalItemDelegate());
    ui->tblTables->setItemDelegate(new TablesItemDeletgate());
    if (fQuickDish.count() == 0) {
//        fDbBind[":f_hall"] = def_default_hall
    }
    fTrackControl = new TrackControl(TRACK_REST_ORDER);
    fHall = Hall::getHallById(fPreferences.getDb(def_default_hall).toInt());
    /*
    fMenu = fHall->fDefaultMenu;
    setBtnMenuText();
    setupType(0);
    */
    ui->tblTables->setMaximumHeight((ui->tblTables->rowCount() * ui->tblTables->verticalHeader()->defaultSectionSize()) + 10);
    fPreferences.setDb(def_working_day, QDate::currentDate().toString(def_date_format));
    DatabaseResult dr;
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    dr.select(fDb, "select f_key, f_value from r_config where f_comp=:f_comp", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fPreferences.setDb(dr.value(i, "f_key").toString(), dr.value("f_value").toString());
    }

    fCostumerId = 0;
    fCarId = 0;
}

RDesk::~RDesk()
{
    delete ui;
    delete fStaff;
}

void RDesk::prepareToShow()
{
    showFullScreen();
    qApp->processEvents();
}

bool RDesk::setup(TableStruct *t)
{
    if (!t) {
        t = ui->tblTables->item(0, 0)->data(Qt::UserRole).value<TableStruct*>();
    }

    if (t) {
        fMenu = Hall::fHallMap[t->fHall]->fDefaultMenu;
        setBtnMenuText();
        setupType(0);
    }


    int colWidth = ui->tblDish->horizontalHeader()->defaultSectionSize();
    int colCount = ui->tblDish->width() / colWidth;
    int delta = ui->tblDish->width() - (colCount * colWidth);
    colWidth += (delta / colCount);
    ui->tblDish->horizontalHeader()->setDefaultSectionSize(colWidth);
    ui->tblDish->setColumnCount(colCount);

    ui->tblOrder->setColumnWidth(1, 40);
    ui->tblOrder->setColumnWidth(2, 60);
    ui->tblOrder->setColumnWidth(0, ui->tblOrder->width() - 104 - ui->tblOrder->verticalHeader()->width());

    ui->tblTotal->setColumnWidth(1, 70);
    ui->tblTotal->setColumnWidth(0, ui->tblTotal->width() - 72);

    bool result = false;
//    if (t) {
//        result = setTable(t);
//    }
    result = true;
    changeBtnState();
    return result;
}

void RDesk::setStaff(User *user)
{
    fStaff = user;
    ui->lbStaff->setText(user->fName);
}

void RDesk::showHideRemovedItems()
{
    fShowRemoved = !fShowRemoved;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        setOrderRowHidden(i, od);
    }
}

void RDesk::setOrderComment()
{
    QString comment;
    if (RDishComment::getComment(comment, this)) {
        fTable->fComment = comment;
        fDbBind[":f_comment"] = comment;
        fDb.update("o_header", fDbBind, QString("where f_id='%1'").arg(fTable->fOrder));
        fTrackControl->insert("Set order comment", comment, "");
    }
}

void RDesk::removeOrder()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_cancelation, trackUser)) {
        return;
    }
    if (message_question(tr("Confirm remove whole order")) != QDialog::Accepted) {
        return;
    }
    QString name, sql = "select f_id, f_en from o_dish_state where f_id in (2, 3)";
    QVariant result;
    if (!DlgList::getValue(tr("STORE OPTION"), name, result, sql)) {
        return;
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (od->fState == DISH_STATE_READY && od->fQtyPrint > 0.1) {
            fDbBind[":f_state"] = result.toInt();
            fDbBind[":f_cancelUser"] = trackUser;
            fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
            fDb.update("o_dish", fDbBind, where_id(ap(od->fRecId)));
            printRemovedDish(od, od->fQty - od->fQtyPrint, trackUser);
        } else if (od->fState == DISH_STATE_READY) {
            fDbBind[":f_state"] = DISH_STATE_EMPTY;
            fDbBind[":f_cancelUser"] = trackUser;
            fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
            fDb.update("o_dish", fDbBind, where_id(ap(od->fRecId)));
        }
        if (od->fState == DISH_STATE_READY) {
            QString dish = QString("%1, %2/%3")
                    .arg(od->fName["en"])
                    .arg(od->fQty)
                    .arg(od->fQtyPrint);
            fTrackControl->insert("Dish removed with order", dish, "");
        }
    }
    fTrackControl->insert("Order canceled", "", "");
    closeOrder(ORDER_STATE_REMOVED);
}

void RDesk::showTableOrders()
{

}

void RDesk::showMyTotal()
{
    fDbBind[":f_dateCash"] = fPreferences.getLocalDate(def_working_day);
    fDbBind[":f_staff"] = fStaff->fId;
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDb.select("select i.f_" + def_lang + ", sum(d.f_total) "
               "from o_dish d "
               "inner join o_header h on h.f_id=d.f_header "
               "inner join f_invoice_item i on i.f_id=h.f_paymentMode "
               "where h.f_dateCash=:f_dateCash and h.f_staff=:f_staff and h.f_state=:f_state "
               "group by 1", fDbBind, fDbRows);
    QString msg;
    foreach_rows {
        msg += it->at(0).toString() + " - " + it->at(1).toString() + "<br> ";
    }
    message_info(msg);

}

void RDesk::initialCash()
{
    float num;
    if (RNumbers::getNumber(num, 100000, this)) {
        fDb.fDb.transaction();
        fDbBind[":f_date"] = fPreferences.getLocalDate(def_working_day);
        fDbBind[":f_staff"] = fStaff->fId;
        fDb.select("delete from o_initial_cash where f_date=:f_date and f_staff=:f_staff", fDbBind, fDbRows);
        fDbBind[":f_date"] = fPreferences.getLocalDate(def_working_day);
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_amount"] = num;
        fDb.insert("o_initial_cash", fDbBind);
        fDb.fDb.commit();
    }
}

void RDesk::setComplexMode()
{
    if (fTable) {
        if (fTable->fHall != 1) {
            message_error(tr("This action is prohabited on this hall"));
            return;
        }
    } else {
        message_info(tr("Please, select table"));
        return;
    }
    DishComplexStruct *dc = DlgComplexDish::complex(this);
    if (dc) {
        checkOrderHeader(fTable);
        fDbBind[":f_header"] = fTable->fOrder;
        fDbBind[":f_state"] = DISH_STATE_READY;
        fDbBind[":f_dish"] = dc->fId;
        fDbBind[":f_qty"] = dc->fQty;
        fDbBind[":f_qtyPrint"] = dc->fQty;
        fDbBind[":f_price"] = dc->fPrice;
        fDbBind[":f_svcValue"] = 0;
        fDbBind[":f_svcAmount"] = 0;
        fDbBind[":f_dctValue"] = 0;
        fDbBind[":f_dctAmount"] = 0;
        fDbBind[":f_total"] = dc->fPrice;
        fDbBind[":f_totalUSD"] = dc->fPrice;
        fDbBind[":f_print1"] = "";
        fDbBind[":f_print2"] = "";
        fDbBind[":f_store"] = dc->fDishes.at(0)->fStore;
        fDbBind[":f_comment"] = "";
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_complex"] = dc->fId;
        fDbBind[":f_complexId"] = dc->fId;
        fDbBind[":f_adgt"] = dc->fAdgt;
        dc->fRecId = fDb.insert("o_dish", fDbBind);
        if (dc->fRecId == 0) {
            message_error("Application will quit due an program error.");
            qApp->quit();
            return;
        }
        fTrackControl->insert("New complex begin", dc->fName["en"], "----");
        for (int i = 0; i < dc->fDishes.count(); i++) {
            dc->fDishes[i]->fComplexRec = dc->fRecId;
            dc->fDishes[i]->fComplex = dc->fId;
            //dc->fDishes[i]->fPrice = 0;
            addDishToOrder(dc->fDishes[i], true);
        }
        ui->tblComplex->setRowCount(ui->tblComplex->rowCount() + 1);
        ui->tblComplex->setItem(ui->tblComplex->rowCount() - 1, 0, new QTableWidgetItem());
        ui->tblComplex->item(ui->tblComplex->rowCount() - 1, 0)->setData(Qt::UserRole, qVariantFromValue(dc));
        countTotal();
        fTrackControl->insert("New complex end", dc->fName["en"], "----");
    }
}

void RDesk::closeOrder(int state)
{
    BaseOrder bo(fTable->fOrder);
    bo.calculateOutput(fDb);
    fDbBind[":f_state"] = state;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    fDbBind[":f_dateClose"] = QDateTime::currentDateTime();
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fDbBind[":f_order"] = 0;
    fDb.update("r_table", fDbBind, where_id(ap(fTable->fId)));

    if (fHall->fServiceItem > 0) {
        DatabaseResult dr;
        fDbBind[":f_header"] = fTable->fOrder;
        fDbBind[":f_state"] = DISH_STATE_READY;
        dr.select(fDb, "select sum(f_total) as f_total from o_dish where f_header=:f_header and f_state=:f_state", fDbBind);
        double t = dr.value("f_total").toDouble();

        OrderDishStruct *od = new OrderDishStruct();
        od->fDishId = fHall->fServiceItem;
        od->fState = DISH_STATE_READY;
        od->fPrint1 = "";
        od->fPrint2 = "";
        od->fStore = 3;
        od->fName["en"] = fHall->fServiceName;
        od->fPrice = t * (fHall->fServiceValue / 100);
        od->fTotal = od->fPrice;
        od->fSvcValue = 0;
        od->fSvcAmount = 0;
        od->fDctValue = 0;
        od->fDctAmount = 0;
        od->fQty = 1;
        od->fQtyPrint = 1;
        od->fComplex = 0;
        od->fComplexRecId = "";
        od->fAdgt = "";

        fDbBind[":f_header"] = fTable->fOrder;
        fDbBind[":f_state"] = DISH_STATE_READY;
        fDbBind[":f_dish"] = od->fDishId;
        fDbBind[":f_qty"] = od->fQty;
        fDbBind[":f_qtyPrint"] = od->fQtyPrint;
        fDbBind[":f_price"] = od->fPrice;
        fDbBind[":f_svcValue"] = od->fSvcValue;
        fDbBind[":f_svcAmount"] = od->fSvcAmount;
        fDbBind[":f_dctValue"] = od->fDctValue;
        fDbBind[":f_dctAmount"] = od->fDctAmount;
        fDbBind[":f_total"] = od->fTotal;
        fDbBind[":f_totalUSD"] = od->fTotal;
        fDbBind[":f_print1"] = od->fPrint1;
        fDbBind[":f_print2"] = od->fPrint2;
        fDbBind[":f_store"] = od->fStore;
        fDbBind[":f_comment"] = od->fComment;
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_complex"] = od->fComplex;
        fDbBind[":f_complexId"] = 0;
        fDbBind[":f_adgt"] = od->fAdgt;
        fDbBind[":f_complexRec"] = od->fComplexRecId;
        od->fRecId = fDb.insert("o_dish", fDbBind);
        fTrackControl->insert("New dish", od->fName["en"], "");
    }
    clearOrder();
}

void RDesk::printTotalShort()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__print_reports_any_day, trackUser)) {
        return;
    }
    QDate date;
    if (!DlgDate::getDate(date)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotalShort(date, userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalToday()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__o_print_reports, trackUser)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotal(WORKING_DATE, userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalYesterday()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__o_print_reports, trackUser)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotal(WORKING_DATE.addDays(-1), userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalAnyDay()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__print_reports_any_day, trackUser)) {
        return;
    }
    QDate date;
    if (!DlgDate::getDate(date)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotal(date, userName, fHall->fReceiptPrinter);
}

void RDesk::printTax(int cashMode)
{

}

void RDesk::printTaxDialog()
{
    int trackUser = fStaff->fId;
    if (!right(cr__print_any_tax, trackUser)) {
        return;
    }
    DlgOrders *d = new DlgOrders(fStaff->fId, this);
    d->exec();
    delete d;
}

void RDesk::printReceiptByNumber()
{
    int trackUser = fStaff->fId;
    if (!right(cr__print_receipt_by_umber, trackUser)) {
        return;
    }
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    } else {
        userName = "#Username Error";
    }
    QString ordNum;
    if (!DlgGetText::getText(ordNum, "")) {
        return;
    }
    if (ordNum.trimmed().isEmpty()) {
        return;
    }
    PPrintReceipt::printOrder(fHall->fReceiptPrinter, ordNum, trackUser);

}

void RDesk::voidBack()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_cancelation, trackUser)) {
        return;
    }
    DlgVoidBack::recover();
    if (fTable) {
        loadOrder();
    }
}

void RDesk::printVoidReport()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_print_reports, trackUser)) {
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    dr.select(fDb, "select oh.f_id, h.f_name as hname, t.f_name as tname, concat(u1.f_firstName, ' ' , u1.f_lastName) as staff, \
              ds.f_en as state, d.f_en as dish, od.f_qty, od.f_total, \
              concat(u2.f_firstName, ' ', u2.f_lastName) as staffcancel \
              from o_dish od \
              left join o_header oh on oh.f_id=od.f_header \
              left join r_hall h on h.f_id=oh.f_hall \
              left join o_dish_state ds on ds.f_id=od.f_state \
              left join r_table t on t.f_id=oh.f_table \
              left join users u1 on u1.f_id=oh.f_staff \
              left join r_dish d on d.f_id=od.f_dish \
              left join users u2 on u2.f_id=od.f_cancelUser \
              where od.f_state in (2, 3) and oh.f_dateCash=:f_dateCash \
              order by 1", fDbBind);

    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 30);
    th.setTextAlignment(Qt::AlignHCenter);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(200, top, 400, 250));
    top += 250;
    top += ps->addTextRect(new PTextRect(10, top, 750, rowHeight, QString("%1").arg(tr("VOID REPORT")), &th, f))->textHeight();
    top += ps->addTextRect(10, top, 680, rowHeight, WORKING_DATE.toString(def_date_format), &th)->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Printed by ") + CacheUsers::instance()->get(trackUser)->fFull, &th)->textHeight();
    top += ps->addTextRect(0, top, 680, rowHeight, tr("Date") + ": " + WORKING_DATE.toString(def_date_format), &th)->textHeight();
    ps->addLine(10, top, 750, top);

    th.setTextAlignment(Qt::AlignLeft);

    QPen dotPen(Qt::DotLine);
    for (int i = 0; i < dr.rowCount(); i++) {
        QString row = QString("%1 / %2 / #%3")
                .arg(dr.value(i, "hname").toString())
                .arg(dr.value(i, "tname").toString())
                .arg(dr.value(i, "f_id").toString());
        top += ps->addTextRect(10, top, 680, rowHeight, row, &th)->textHeight();
        ps->addLine(10, top, 680, top, dotPen);
        row = QString ("%1 / %2 / %3")
                .arg(dr.value(i, "dish").toString())
                .arg(dr.value(i, "f_qty").toString())
                .arg(dr.value(i, "f_total").toString());
        top += ps->addTextRect(10, top, 680, rowHeight, row, &th)->textHeight();
        row = QString("%1").arg(dr.value(i, "staff").toString());
        top += ps->addTextRect(10, top, 680, rowHeight, row, &th)->textHeight();
        top += ps->addTextRect(10, top, 680, rowHeight, tr("Type: ") + dr.value(i, "state").toString(), &th)->textHeight();
        top += ps->addTextRect(10, top, 680, rowHeight, tr("Manager"), &th)->textHeight();
        top += ps->addTextRect(10, top, 680, rowHeight, dr.value(i, "staffcancel").toString(), &th)->textHeight();
        top ++;
        ps->addLine(10, top, 680, top, dotPen);
        top += 2;
        ps->addLine(10, top, 680, top, dotPen);
        top += rowHeight;
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }

    ps->addTextRect(10, top, 300, rowHeight, "_", &th);


    QPrinter printer;
    printer.setPrinterName(fHall->fReceiptPrinter);
    QMatrix m;
    m.scale(3, 3);
    QPainter painter(&printer);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }
    qDeleteAll(lps);
}

void RDesk::complimentary()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_complimentary, trackUser)) {
        return;
    }
    QString comment, sql = "select f_id, f_name from r_complimentary_comment";
    QVariant result;
    if (!DlgList::getValue(tr("Complimentary comment"), comment, result, sql)) {
        return;
    }
    fDbBind[":f_paymentMode"] = PAYMENT_COMPLIMENTARY;
    fDbBind[":f_paymentModeComment"] =  comment;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fTable->fPaymentComment = comment;
    fTable->fPaymentMode = PAYMENT_COMPLIMENTARY;
    printReceipt(true);
    closeOrder();
}

void RDesk::changePassword()
{
    QString login;
    if (RLogin::getLogin(login, tr("Login"), this)) {
        QString query = QString("update users set f_altPassword='' where f_altPassword=md5('%1')")
                .arg(login);
        fDb.queryDirect(query);
        query = QString("update users set f_altPassword=md5('%1') where f_id=%2")
                .arg(login)
                .arg(fStaff->fId);
        fDb.queryDirect(query);
        message_info(tr("Your password was changed"));
    }
}

void RDesk::openTools()
{
    RTools *t = new RTools(this);
    t->setNoTable();
    if (t->exec() == QDialog::Accepted) {

    }
    delete t;
}

void RDesk::closeDay()
{
    fDbBind[":f_date"] = WORKING_DATE;
    fDbBind[":f_docType"] = 2;
    fDb.select("delete from c_cash where f_date=:f_date and f_docType=:f_docType", fDbBind, fDbRows);

    DatabaseResult dtotal;
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    dtotal.select(fDb, "select sum(h.f_cash+h.f_card) as f_total, sum(f_card) as f_card from o_header_payment h "
                       "left join o_Header o on o.f_id=h.f_id "
                       "where o.f_state=:f_state and o.f_dateCash=:f_dateCash", fDbBind);
    if (dtotal.rowCount() > 0) {
        fDbBind[":f_date"] = WORKING_DATE;
        fDbBind[":f_docType"] = 2;
        fDbBind[":f_debit"] = 1;
        fDbBind[":f_credit"] = 1;
        fDbBind[":f_amount"] = dtotal.value("f_total");
        fDb.insert("c_cash", fDbBind);
        fDbBind[":f_date"] = WORKING_DATE;
        fDbBind[":f_docType"] = 2;
        fDbBind[":f_debit"] = 1;
        fDbBind[":f_credit"] = 4;
        fDbBind[":f_amount"] = dtotal.value("f_card").toDouble() * -1;
        fDb.insert("c_cash", fDbBind);
    }
    printTotalToday();
}

void RDesk::salary()
{
    DlgSalary::salary();
}

void RDesk::visitStat()
{
    int id;
    QString name;
    if (!DlgDeptHolder::getHolder(id, name)) {
        return;
    }
    fDbBind[":f_debtHolder"] = id;
    DatabaseResult dr;
    dr.select(fDb, "select count(o.f_id) as qty, sum(o.f_total)  as amount \
              from o_header o \
              left join o_car c on c.f_order=o.f_id \
              where c.f_costumer=:f_debtHolder and o.f_state=2 ", fDbBind);
    QString msg;
    if (dr.rowCount() > 0) {
        msg = name + "<br>" + tr("Total visits") + "<br>" + QString("%1: %2<br>%3: %4")
            .arg(tr("Visits"))
            .arg(dr.value("qty").toInt())
            .arg(tr("Amount"))
            .arg(float_str(dr.value("amount").toDouble(), 2));
    }
    fDbBind[":f_debtHolder"] = id;
    dr.select(fDb, "select h.f_dateCash, h.f_id, d.f_name, v.f_debt "
                   "from o_header_payment v "
                   "left join o_header h on h.f_id=v.f_id "
                   "left join o_debt_holder d on d.f_id=v.f_debtHolder "
                   "where v.f_debt>0 and v.f_debtHolder=:f_debtHolder "
                   "union "
                   "select p.f_date, p.f_id, d.f_name, p.f_amount * -1 as f_debt "
                   "from o_debt_pay p "
                   "left join o_debt_holder d on d.f_id=p.f_holder "
                   "where p.f_holder=:f_debtHolder", fDbBind);
    if (dr.rowCount() > 0) {
        double debt = 0;
        for (int i = 0; i < dr.rowCount(); i++) {
            debt += dr.value(i, "f_debt").toDouble();
        }
        if (debt > 1 || debt < -1) {
            msg += "<br>" + tr("Debt") + ": " + float_str(debt, 2);
        }
    }

    message_info(msg);
}

void RDesk::checkCardAmount()
{
    QString name;
    QVariant result;
    if (!DlgList::getValue(tr("Card holder"), name, result, "select f_card, right(f_card, 5) from d_car_client where f_model=3")) {
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_card"] = result;
    dr.select(fDb, "select f_mode from d_car_client where f_card=:f_card", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Invalid card code"));
        return;
    }
    QStringList l = dr.value(0, 0).toString().split(";");
    if (l.count() < 0) {
        message_error(tr("Card error"));
        return;
    }
    message_info(tr("Balance") + "<br>" + l.at(2));
}

void RDesk::cardStat()
{
    QString name;
    QVariant result;
    if (!DlgList::getValue(tr("Card holder"), name, result, "select f_id, f_name from d_car_client where length(f_name) >0 order by 2")) {
        return;
    }
    QString msg;
    DatabaseResult dr;

    fDbBind[":f_costumer"] = result;
    dr.select(fDb, "select right(f_discountcard, 4) card, count(hp.f_id) as qty, "
                   "sum(f_finalAmount) as amount from o_header_payment hp "
                   "left join o_header h on h.f_id=hp.f_id "
                   "where hp.f_costumer = :f_costumer and h.f_state=2  "
                   "group by 1", fDbBind);
    msg += "<br>" + tr("Cards") + "<br>";
    msg += QString("%1 / %2 / %3 / %4<br>").arg(tr("Card")).arg(tr("Total qty")).arg(tr("Total amount")).arg(tr("Current visits"));
    for (int i = 0; i < dr.rowCount(); i++) {
        msg += dr.value(i, "card").toString() + ": " + dr.value(i, "qty").toString() + "/" + float_str(dr.value(i, "amount").toDouble(), 2) + "/";
        msg += QString("%1<br>").arg(dr.value(i, "qty").toInt() % 11);
    }
    message_info(msg);
}

void RDesk::saledItem()
{
    int trackUser;
    if (!right(cr__print_reports_any_day, trackUser)) {
        return;
    }
    QDate date;
    if (!DlgDate::getDate(date)) {
        return;
    }
    C5Printing p;
    p.setSceneParams(700, 2800, QPrinter::Portrait);
    p.setFont(qApp->font());
    p.setFontSize(20);

    p.ctext(tr("Daily sale"));
    p.br();
    p.ctext(tr("Goods"));
    p.br();
    p.ctext(date.toString("dd/MM/yyyy"));
    p.br();

    fDbBind[":f_datecash"] = date;
    fDbBind[":f_ostate"] = ORDER_STATE_CLOSED;
    fDbBind[":f_dstate"] = DISH_STATE_READY;
    DatabaseResult dr;
    dr.select(fDb, "select d.f_en, od.f_store, sum(od.f_qty) as f_qty, sum(od.f_total) as f_total "
              "from o_dish od "
              "inner join o_header oh on oh.f_Id=od.f_header "
              "inner join r_dish d on d.f_id=od.f_dish "
              "where oh.f_state=:f_ostate and od.f_state=:f_dstate "
              "and oh.f_datecash=:f_datecash "
              "group by 1, 2 "
              "order by od.f_store ", fDbBind);
    if (dr.rowCount() == 0) {
        return;
    }
    double total = 0.0;
    int store = 0;
    for (int i = 0; i < dr.rowCount(); i++) {
        if (store != dr.value(i, "f_store").toInt()) {
            if (total > 0.01) {
                p.br();
                p.ltext(tr("Total"), 0);
                p.rtext(float_str(total, 2));
                p.br();
                p.br();
            }
            store = dr.value(i, "f_store").toInt();
            DatabaseResult dr2;
            fDbBind[":f_id"] = store;
            dr2.select(fDb, "select f_name from r_store where f_id=:f_id", fDbBind);
            if (dr2.rowCount() > 0) {
                p.br();
                p.br();
                p.ctext(dr2.value("f_name").toString());
                p.br();
                p.line();
                p.br(2);
            }
        }
        total += dr.value(i, "f_total").toDouble();
        p.br(2);
        p.ltext(dr.value(i, "f_en").toString(), 0);
        p.br();
        p.ltext(dr.value(i, "f_qty").toString(), 0);
        p.ltext(dr.value(i, "f_total").toString(), 150);
        p.br();
        p.line();
        p.br(2);
    }
    if (total > 0.01) {
        p.br();
        p.ltext(tr("Total"), 0);
        p.rtext(float_str(total, 2));
    }
    p.print("p1", QPrinter::Custom);
}

void RDesk::checkCardRegistration()
{
    DlgCheckCard *d = new DlgCheckCard(this);
    d->exec();
    delete d;
}

void RDesk::employesOfDay()
{
    DlgSalary::salary2();
}

void RDesk::closeEvent(QCloseEvent *e)
{
    if (!fCanClose) {
        e->ignore();
        return;
    }
    checkEmpty();
    BaseExtendedDialog::closeEvent(e);
}

void RDesk::onBtnQtyClicked()
{
    fCloseTimeout = 0;
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    QPushButton *b = static_cast<QPushButton*>(sender());
    float qty;
    if (b->text() == "-0.5") {
        qty = -0.5;
    } else if (b->text() == "+0.5") {
        qty = 0.5;
    } else {
        qty = b->text().toFloat();
    }
    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
    if (!od) {
        return;
    }
    if (od->fState != DISH_STATE_READY) {
        message_error(tr("You cannot edit the quantity of selected item"));
        return;
    }
    if (od->fQty + qty < 0.1) {
        return;
    }
    if (od->fComplex == 0) {
        QString oldQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                .arg(od->fQty)
                .arg(od->fQtyPrint);
        if (od->fQtyPrint < 0.01) {
            if (qty > 0.9) {
                if (qty < 10.0) {
                    od->fQty = qty;
                } else {
                    od->fQty += qty;
                }
            } else {
                od->fQty += qty;
            }
        } else {
            od->fQty += qty;
        }
        QString newQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                .arg(od->fQty)
                .arg(od->fQtyPrint);
        countDish(od);
        fTrackControl->insert("Dish qty", oldQty, newQty);
        updateDish(od);
    } else {
        double complexQty = 0;
        DishComplexStruct *dc = 0;
        for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
            DishComplexStruct *dt = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
            if (dt->fRecId == od->fComplexRecId) {
                dc = dt;
                break;
            }
        }
        if (!dc) {
            message_error("Application error. Contact to developer, message: dc ==0, dish qty");
            return;
        }
        QString oldQty = QString("%1, %2").arg(dc->fName[def_lang])
                .arg(dc->fQty);
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (od->fComplexRecId != dc->fRecId) {
                continue;
            }
            QString odOldQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                    .arg(od->fQty)
                    .arg(od->fQtyPrint);
            if (od->fQtyPrint < 0.01) {
                if (qty > 0.9){
                    od->fQty = qty;
                } else {
                    od->fQty += qty;
                }
            } else {
                od->fQty += qty;
            }
            complexQty = od->fQty;
            QString odNewQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                    .arg(od->fQty)
                    .arg(od->fQtyPrint);
            fTrackControl->insert("Dish qty", odOldQty, odNewQty);
            countDish(od);
            updateDish(od);
        }
        dc->fQty = complexQty;
        fDbBind[":f_qty"] = dc->fQty;
        fDbBind[":f_total"] = dc->fPrice * dc->fQty;
        fDbBind[":f_totalUSD"] = dc->fPrice * dc->fQty;
        fDb.update("o_dish", fDbBind, where_id(ap(dc->fRecId)));
        QString newQty = QString("%1, %2").arg(dc->fName[def_lang])
                .arg(dc->fQty);
        fTrackControl->insert("Complex qty", oldQty, newQty);
    }
    resetPrintQty();
    ui->tblOrder->viewport()->update();
    countTotal();
    changeBtnState();
    repaintTables();
}

void RDesk::on_btnExit_clicked()
{
    if (message_question(tr("Confirm to close application")) != QDialog::Accepted) {
        return;
    }
    if (fTable) {
//        fDb.fDb.transaction();
//        fDbBind[":f_id"] = fTable->fId;
//        if (fDb.select("select f_id from r_table where f_id=:f_id for update", fDbBind, fDbRows) == -1) {
//            message_error(tr("Cannot close current table, try later"));
//            return;
//        }
//        fDbBind[":f_lockTime"] = 0;
//        fDbBind[":f_lockHost"] = "";
//        fDb.update("r_table", fDbBind, QString("where f_id=%1").arg(fTable->fId));
//        fDb.fDb.commit();
    }
    fCanClose = true;
    close();
}

void RDesk::on_btnLanguage_clicked()
{
    if (RChangeLanguage::changeLanguage(this)) {
        ui->tblType->viewport()->update();
        ui->tblDish->viewport()->update();
        ui->tblOrder->viewport()->update();
    }
}

void RDesk::on_btnMenu_clicked()
{
    int newMenu;
    if (RChangeMenu::changeMenu(fMenu, newMenu, this)) {
        fMenu = newMenu;
        setBtnMenuText();
        setupType(0);
    }
}

void RDesk::setBtnMenuText()
{
    if (fMenu == 0) {
        message_error(tr("Default menu is not set"));
        return;
    }
}

void RDesk::setupType(int partId)
{
    writelog("RDesk::setupType start.");
    QMap<int, TypeStruct*> type;
    fDishTable.filterType(fMenu, partId, type);
    ui->tblDish->clear();
    ui->tblDish->setRowCount(0);
    ui->tblType->clear();
    ui->tblType->setColumnCount(2);
    ui->tblType->horizontalHeader()->setDefaultSectionSize((ui->tblType->width() - 10)  /2);
    int row = -1, col = 3;
    for (QMap<int, TypeStruct*>::const_iterator it = type.begin(); it != type.end(); it++) {
        if (col > ui->tblType->columnCount() - 1) {
            col = 0;
            row ++;
            ui->tblType->setRowCount(row + 1);
        }
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(it.value()));
        ui->tblType->setItem(row, col++, item);
    }
    writelog("RDesk::setupType end.");
}

void RDesk::setupDish(int typeId)
{
    ui->tblDish->clear();
    QMap<int, DishStruct*> dish;
    fDishTable.filterDish(fMenu, typeId, dish);
    int rowCount = (dish.count() / ui->tblDish->columnCount());
    if (dish.count() % ui->tblDish->columnCount() > 0) {
        rowCount++;
    }
    ui->tblDish->setRowCount(rowCount);
    int col = 0, row = 0;
    for (QMap<int, DishStruct*>::const_iterator it = dish.begin(); it != dish.end(); it++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(*it));
        ui->tblDish->setItem(row, col++, item);
        if (col == ui->tblDish->columnCount()) {
            row++;
            col = 0;
        }
    }
}

void RDesk::addDishToOrder(DishStruct *d, bool counttotal)
{
    checkOrderHeader(fTable);
    OrderDishStruct *od = new OrderDishStruct();
    if (d->fMod.count() > 0) {
        QStringList mods;
        for (QList<QMap<QString, QString> >::const_iterator it = d->fMod.begin(); it != d->fMod.end(); it++) {
            mods.append(it->value(def_lang));
        }
        RModifiers *m = new RModifiers(this);
        m->setModifiers(mods);
        if (m->exec() == QDialog::Accepted) {
            od->fComment = m->mod();
        }
        delete m;
    }
    od->fDishId = d->fId;
    od->fState = DISH_STATE_READY;
    od->fPrint1 = d->fPrint1;
    od->fPrint2 = d->fPrint2;
    od->fStore = d->fStore;
    od->fName = d->fName;
    od->fPrice = d->fPrice;
    od->fSvcValue = d->fId == 487 ? 0 : fHall->fServiceValue / 100;
    od->fSvcAmount = d->fId == 487 ? 0 : od->fSvcValue * od->fTotal;
    od->fDctValue = 0;
    od->fDctAmount = 0;
    od->fQty = 1;
    od->fQtyPrint = 0;
    od->fComplex = d->fComplex;
    od->fComplexRecId = d->fComplexRec;
    od->fAdgt = d->fAdgt;
    od->fTax = d->fTax;

    countDish(od);
    fDbBind[":f_header"] = fTable->fOrder;
    fDbBind[":f_state"] = DISH_STATE_READY;
    fDbBind[":f_dish"] = od->fDishId;
    fDbBind[":f_qty"] = od->fQty;
    fDbBind[":f_qtyPrint"] = od->fQtyPrint;
    fDbBind[":f_price"] = od->fPrice;
    fDbBind[":f_svcValue"] = od->fSvcValue;
    fDbBind[":f_svcAmount"] = od->fSvcAmount;
    fDbBind[":f_dctValue"] = od->fDctValue;
    fDbBind[":f_dctAmount"] = od->fDctAmount;
    fDbBind[":f_total"] = od->fTotal;
    fDbBind[":f_totalUSD"] = od->fTotal;
    fDbBind[":f_print1"] = od->fPrint1;
    fDbBind[":f_print2"] = od->fPrint2;
    fDbBind[":f_store"] = od->fStore;
    fDbBind[":f_comment"] = od->fComment;
    fDbBind[":f_staff"] = fStaff->fId;
    fDbBind[":f_complex"] = od->fComplex;
    fDbBind[":f_complexId"] = 0;
    fDbBind[":f_adgt"] = od->fAdgt;
    fDbBind[":f_complexRec"] = od->fComplexRecId;
    od->fRecId = fDb.insert("o_dish", fDbBind);
    updateDishQtyHistory(od);
    addDishToTable(od, counttotal);
    resetPrintQty();
    fTrackControl->insert("New dish", od->fName["en"], "");
}

void RDesk::addDishToTable(OrderDishStruct *od, bool counttotal)
{
    int row = ui->tblOrder->rowCount();
    ui->tblOrder->setRowCount(row + 1);
    for (int i = 0; i < 3; i++) {
        ui->tblOrder->setItem(row, i, new QTableWidgetItem());
    }
    ui->tblOrder->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(od));
    ui->tblOrder->item(row, 1)->setData(Qt::UserRole, qVariantFromValue(od));
    ui->tblOrder->item(row, 2)->setData(Qt::UserRole, qVariantFromValue(od));
    ui->tblOrder->setCurrentCell(row, 0);
    setOrderRowHidden(row, od);
    if (counttotal) {
        countTotal();
        changeBtnState();
    }
}

void RDesk::updateDish(OrderDishStruct *od)
{
    fDbBind[":f_state"] = od->fState;
    fDbBind[":f_qty"] = od->fQty;
    fDbBind[":f_qtyPrint"] = od->fQtyPrint;
    fDbBind[":f_price"] = od->fPrice;
    fDbBind[":f_svcValue"] = od->fSvcValue;
    fDbBind[":f_svcAmount"] = od->fSvcAmount;
    fDbBind[":f_dctValue"] = od->fDctValue;
    fDbBind[":f_dctAmount"] = od->fDctAmount;
    fDbBind[":f_total"] = od->fTotal;
    fDbBind[":f_totalUSD"] = od->fTotal;
    fDbBind[":f_comment"] = od->fComment;
    fDbBind[":f_cancelUser"] = od->fCancelUser;
    fDbBind[":f_cancelDate"] = od->fCancelDate;
    fDb.update("o_dish", fDbBind, QString("where f_id=%1").arg(od->fRecId));
//    if (!od->fComplexRecId.isEmpty()) {
//        fDbBind[":f_id"] = od->fComplexRecId;
//        fDb.query("update o_dish set f_totalUSD=f_total where f_id=:f_id", fDbBind);
//    }
    updateDishQtyHistory(od);
}

double RDesk::countTotal()
{
    double total = 0;
    double serviceValue = 0;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (!od->fComplexRecId.isEmpty() ) {
            continue;
        }
        total += od->fTotal;
        serviceValue += od->fSvcAmount;
    }
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc == 0, countTotal");
            return -1;
        }
        total += dc->fPrice * dc->fQty;
    }

    double grandTotal = total ;

    fHall = Hall::getHallById(fTable->fHall);
    ui->tblTotal->setItem(2, 0, new QTableWidgetItem(fHall->fServiceName));
    if (fHall->fServiceItem > 0) {
        ui->tblTotal->setItem(2, 1, new QTableWidgetItem(float_str(serviceValue, 2)));
    }

//    if ((int)grandTotal % 10 > 0) {
//        double r = (int)grandTotal % 10;
//        double serviceValue += 10 - r;
//        grandTotal += 10 - r;
//        ui->tblTotal->setItem(2, 1, new QTableWidgetItem(float_str(serviceValue, 2)));
//    }

    ui->tblTotal->item(1, 1)->setData(Qt::EditRole, float_str(grandTotal, 2));
    fDbBind[":f_total"] = grandTotal;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fDbBind[":f_cash"] = grandTotal;
    fDbBind[":f_card"] = 0;
    fDbBind[":f_debt"] = 0;
    fDbBind[":f_coupon"] = 0;
    fDb.update("o_header_payment", fDbBind, where_id(ap(fTable->fOrder)));
    fTable->fAmount = float_str(grandTotal, 2);

    updateTableInfo();
    return grandTotal;
}

void RDesk::countDish(OrderDishStruct *d)
{
    d->fTotal = d->fQty * d->fPrice;
    d->fSvcAmount = (d->fTotal * d->fSvcValue);
    d->fTotal += d->fSvcAmount;
    d->fDctAmount = d->fTotal * d->fDctValue;
    d->fTotal -= d->fDctAmount;
}

bool RDesk::setTable(TableStruct *t)
{
    if (fTable) {
        if (t == fTable) {
            DlgSmile *ds = new DlgSmile(this);
            ds->exec();
            delete ds;
            return true;
        }
    }
    if (fTable) {
        //unlock previous table
        checkEmpty();
        QString query = QString("update r_table set f_lockTime=0, f_lockHost='' where f_id=%1")
                .arg(fTable->fId);
        fDb.queryDirect(query);
    }
    clearOrder();
    if (t == 0) {
        return false;
    }
    fCarModel.clear();
    fCarGovNum.clear();
    ui->lbCar->clear();
    Splash s(this);
    s.show();
    fTable = t;
    s.setText(tr("Opening table ") + t->fName);
    ui->tblTotal->item(0, 1)->setText(t->fName);
    ui->tblTotal->item(1, 1)->setText("0");
    fDbBind[":f_id"] = t->fId;
    if (fDb.select("select  f_order from r_table where f_id=:f_id ", fDbBind, fDbRows) == -1) {
//        if (fDb.fLastError.contains("Lock wait timeout exceeded")) {
//            fDb.fDb.commit();
//            s.close();
//            message_error(tr("Table in editing mode"));
//            fCanClose = true;
//            close();
//            return false;
//        }
    }
//    QString lockHost = fDbRows.at(0).at(1).toString();
//    int lockTime = fDbRows.at(0).at(0).toInt();
//    int currTime = QDateTime::currentDateTime().toTime_t();
//    if (lockTime > 0) {
//        if (currTime - lockTime < 1 * 60 * 3) {
//            if (lockHost != Utils::hostName()) {
//                s.close();
//                message_error(tr("Table in editing mode"));
//                fCanClose = true;
//                close();
//                return false;
//            }
//        }
//    }

    if (fTable) {
        fTable->fOrder = fDbRows.at(0).at(0).toInt();
        if (fTable->fOrder > 0) {
            loadOrder();
        }
        fHall = Hall::getHallById(fTable->fHall);
        ui->lbCar->setText(fCarModel + " " + fCarGovNum);
        s.close();

        if (fTable->fHall == 3) {
            OrderDishStruct *od = nullptr;
            for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
                od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
                if (!od) {
                    continue;
                }
                if (od->fDishId == 487) {
                    break;
                }
            }
            if (od == nullptr) {
                if (message_question(tr("Open new VIP Table?")) == QDialog::Accepted) {
                    DishStruct *d = nullptr;
                    for (int i = 0; i < fDishTable.fDish.count(); i++) {
                        if (fDishTable.fDish.at(i)->fId == 487) {
                            d = fDishTable.fDish.at(i);
                        }
                    }
                    if (!d) {
                        setTable(nullptr);
                        return false;
                    }
                    addDishToOrder(d, false);
                } else {
                    setTable(nullptr);
                    return false;
                }
            }
        }
    }
    countTotal();
    changeBtnState();
    return true;
}

void RDesk::checkOrderHeader(TableStruct *t)
{
    if (t->fOrder == 0) {
        fDb.fDb.transaction();
        QString query = QString("select f_id from r_table where f_id='%1' for update")
                .arg(t->fId);
        fDb.queryDirect(query);

        fDbBind[":f_state"] = ORDER_STATE_OPENED;
        fDbBind[":f_table"] = t->fId;
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_dateOpen"] = QDateTime::currentDateTime();
        fDbBind[":f_dateCash"] = WORKING_DATE;
        fDbBind[":f_tax"] = 0;
        fDbBind[":f_paymentMode"] = PAYMENT_CASH;
        fDbBind[":f_hall"] = t->fHall;
        fDbBind[":f_servicevalue"] = fHall->fServiceValue;
        t->fOpened = fDbBind[":f_dateOpen"].toDateTime();
        t->fOrder = fDb.insert("o_header", fDbBind);

        fDbBind[":f_order"] = t->fOrder;
        fDb.update("r_table", fDbBind, QString("where f_id=%1").arg(t->fId));
        fDb.fDb.commit();

        ui->tblTables->viewport()->update();
        if (fTable->fHall == 1) {
            on_btnSetCar_clicked();
        }
    }
}

void RDesk::clearOrder()
{
    if (!fTable) {
        return;
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        delete ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
    }
    ui->tblOrder->clear();
    ui->tblOrder->setRowCount(0);
    fTable->fPrint = 0;
    fTable->fOrder = 0;
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        for (int j = 0; j < dc->fDishes.count(); j++) {
            delete dc->fDishes[j];
        }
        delete dc;
    }
    ui->tblComplex->setRowCount(0);
    ui->lbCar->clear();
    fCarModel = "";
    fCarGovNum = "";
    fCostumerId = 0;
    fCarId = 0;
    ui->tblTotal->item(1, 1)->setData(Qt::EditRole, "0");
    if (ui->tblTotal->item(2, 1)) {
        ui->tblTotal->item(2, 1)->setData(Qt::EditRole, "0");
    }
    fTable = nullptr;
}

void RDesk::loadOrder()
{
    QElapsedTimer et;
    et.start();
    ui->tblComplex->clear();
    ui->tblComplex->setRowCount(0);
    //ui->tblOrder->clear();
    ui->tblOrder->setRowCount(0);
    User u;
    QString query = QString("select h.f_staff, h.f_comment, concat(u.f_firstName, ' ', u.f_lastName), h.f_print, "
                            "h.f_paymentMode, h.f_paymentModeComment, h.f_cityLedger, h.f_roomComment, h.f_dateOpen, "
                            "h.f_tax "
                             "from o_header h "
                             "left join users u on u.f_id=h.f_staff "
                             "where h.f_id='%1'")
            .arg(fTable->fOrder);
    fDb.select(query, fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        return;
    }

    u.fId = fDbRows.at(0).at(0).toInt();
    u.fName = fDbRows.at(0).at(2).toString();
    fTable->fComment = fDbRows.at(0).at(1).toString();
    fTable->fPrint = fDbRows.at(0).at(3).toInt();
    fTable->fPaymentMode = fDbRows.at(0).at(4).toInt();
    fTable->fPaymentComment = fDbRows.at(0).at(5).toString();
    fTable->fCitiLedger = fDbRows.at(0).at(6).toInt();
    fTable->fRoomComment = fDbRows.at(0).at(7).toString();
    fTable->fOpened = fDbRows.at(0).at(8).toDateTime();
    fTable->fTaxPrint = fDbRows.at(0).at(9).toInt();
    logtime("LOAD ORDER HEADER " , et.elapsed());
    et.restart();
    if (u.fId != fStaff->fId) {
        message_info(QString("%1<br>%2")
                     .arg(tr("Table opened by"))
                     .arg(u.fName));
    }

    //Complex
    query = "select od.f_id, od.f_dish, dc.f_en, dc.f_ru, dc.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
            "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
            "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
            "dc.f_adgt "
            "from o_dish od "
            "left join r_dish_complex dc on dc.f_id=od.f_complexId "
            "where od.f_header=:f_header and f_complex>0 and f_complexId>0 and f_state=1 "
            "order by od.f_id ";
    fDbBind[":f_header"] = fTable->fOrder;
    fDb.select(query, fDbBind, fDbRows);
    logtime("LOAD ORDER COMPLEX QUERY", et.elapsed());
    et.restart();
    foreach_rows {
        DishComplexStruct *dc = new DishComplexStruct();
        dc->fRecId = it->at(0).toInt();
        dc->fName["en"] = it->at(2).toString();
        dc->fName["ru"] = it->at(3).toString();
        dc->fName["am"] = it->at(4).toString();
        dc->fAdgt = it->at(20).toString();
        dc->fId = it->at(19).toInt();
        dc->fQty = it->at(5).toDouble();
        dc->fPrice = it->at(7).toDouble();
        int row = ui->tblComplex->rowCount();
        ui->tblComplex->setRowCount(row + 1);
        ui->tblComplex->setItem(row, 0, new QTableWidgetItem());
        ui->tblComplex->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(dc));
    }
    logtime("LOAD ORDER COMPLEX ", et.elapsed());
    et.restart();

    query = "select od.f_id, od.f_dish, d.f_en, d.f_ru, d.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
            "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
            "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
            "od.f_adgt, od.f_complexRec "
            "from o_dish od "
            "left join r_dish d on d.f_id=od.f_dish "
            "where od.f_header=:f_header and (f_complex=0 or (f_complex>0 and f_complexId=0)) and f_state=1 "
            "order by od.f_id ";
    fDbBind[":f_header"] = fTable->fOrder;
    QList<QList<QVariant> > dbr;
    fDb.select(query, fDbBind, dbr);
    for (QList<QList<QVariant> >::const_iterator it = dbr.begin(); it != dbr.end(); it++) {
        OrderDishStruct *d = new OrderDishStruct();
        int c = 0;
        d->fRecId = it->at(c++).toInt();
        d->fDishId = it->at(c++).toInt();
        d->fName["en"] = it->at(c++).toString();
        d->fName["ru"] = it->at(c++).toString();
        d->fName["am"] = it->at(c++).toString();
        d->fQty = it->at(c++).toFloat();
        d->fQtyPrint = it->at(c++).toFloat();
        d->fPrice = it->at(c++).toFloat();
        d->fSvcValue = it->at(c++).toFloat();
        d->fSvcAmount = it->at(c++).toFloat();
        d->fDctValue = it->at(c++).toFloat();
        d->fDctAmount = it->at(c++).toFloat();
        d->fTotal = it->at(c++).toFloat();
        d->fPrint1 = it->at(c++).toString();
        d->fPrint2 = it->at(c++).toString();
        d->fComment = it ->at(c++).toString();
        d->fStaff = it->at(c++).toInt();
        d->fState = it->at(c++).toInt();
        d->fComplex = it->at(c++).toInt();
        c++; //complexId
        d->fAdgt = it->at(c++).toString();
        d->fComplexRecId = it->at(c++).toString();
        if (!d->fComplexRecId.isEmpty()) {
            for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
                DishComplexStruct *ds = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
                if (ds) {
                    if (ds->fRecId == d->fComplexRecId) {
                        DishStruct *dss = new DishStruct();
                        dss->fId = d->fDishId;
                        dss->fName = d->fName;
                        dss->fPrice = d->fPrice;
                        ds->fDishes.append(dss);
                    }
                }
            }
        }
        countDish(d);
        addDishToTable(d, false);
        setOrderRowHidden(ui->tblOrder->rowCount() - 1, d);
    }
    logtime("LOAD ORDER DISHES ", et.elapsed());
    et.restart();
    changeBtnState();
    countTotal();
    DatabaseResult dr;
    fDbBind[":f_order"] = fTable->fOrder;
    dr.select(fDb, "select f_model, f_govNumber,f_costumer from o_car where f_order=:f_order", fDbBind);
    if (dr.rowCount() > 0) {
        fCarId = dr.value("f_model").toInt();
        CI_Car *car = CacheCar::instance()->get(dr.value("f_model").toString());
        if (car) {
            fCarModel = car->fName;
        }
        fCarGovNum = dr.value("f_govNumber").toString();
        fCostumerId = dr.value("f_costumer").toInt();
    }
    ui->lbCar->setText(fCarModel + " " + fCarGovNum);
}

void RDesk::setOrderRowHidden(int row, OrderDishStruct *od)
{
    switch(od->fState) {
    case DISH_STATE_READY:
        ui->tblOrder->setRowHidden(row, false);
        break;
    case DISH_STATE_REMOVED_NOSTORE:
    case DISH_STATE_REMOVED_STORE:
    case DISH_STATE_MOVED:
        ui->tblOrder->setRowHidden(row, !fShowRemoved);
        break;
    case DISH_STATE_EMPTY:
        ui->tblOrder->setRowHidden(row, true);
        break;
    }
}

void RDesk::printServiceCheck(const QString &prn, int side)
{
    QStringList printers = QPrinterInfo::availablePrinterNames();
    foreach (QString s, printers) {
        writelog("printer - " + s);
    }

    if (!printers.contains(prn, Qt::CaseInsensitive)) {
        message_error(tr("The printer with name not exists on the system") + "\r\n" + prn);
        return;
    }
    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 24);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    PTextRect *r = 0;
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    f.setPointSize(34);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, fHall->fName, &th)->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    r = ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("Service check, order #"))
                                     .arg(fTable->fOrder),
                                     &th, f));
    top += r->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Table")).arg(fTable->fName), &th, f))->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Time"))
                                        .arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th, f))->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Waiter"))
                                        .arg(fStaff->fName), &th, f))->textHeight();
    ps->addLine(10, top, 680, top);
    top += 2;
    ps->addTextRect(new PTextRect(10, top, 80, rowHeight, tr("Qty"), &th, f));
    top += ps->addTextRect(new PTextRect(90, top, 680, rowHeight, tr("Description"), &th, f))->textHeight();
    ps->addLine(10, top, 680, top);
    top += 2;
    th.setTextAlignment(Qt::AlignLeft);
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (side == 1) {
            if (od->fPrint1 != prn) {
                continue;
            }
        }
        if (side == 2) {
            if (od->fPrint2 != prn) {
                continue;
            }
        }
        float qty = od->fQty - od->fQtyPrint;
        if (qty < 0.1) {
            continue;
        }
        ps->addTextRect(new PTextRect(10, top, 80, rowHeight, float_str(qty, 1), &th, f));
        top += ps->addTextRect(new PTextRect(90, top, 680, rowHeight, od->fName[def_lang], &th, f))->textHeight();
        if (!od->fComment.isEmpty()) {
            f.setPointSize(18);
            f.setBold(true);
            th.setFont(f);
            top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, od->fComment, &th, f))->textHeight();
            f.setPointSize(24);
            f.setBold(false);
            th.setFont(f);
        }
        ps->addLine(10, top, 680, top);
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    top += 25;
    th.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(10, top, 680, rowHeight, tr("Printer: ") + prn, &th);
    top += 5;
    ps->addTextRect(10, top, 680, rowHeight, "_", &th);
    QPrinter printer;
    //writelog("before set " + prn.toUpper());
    printer.setPrinterName(prn.toUpper());
    //writelog("after set " + printer.printerName());
    int scale = 3; //600 / printer.resolution();
    //printer.setPrinterName("\\\\10.2.1.37\\HP LaserJet Professional P1102");
    QPainter painter(&printer);
    QPrintDialog pd(&printer, this);
    QMatrix m;
    m.scale(scale, scale);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }
}

void RDesk::printRemovedDish(OrderDishStruct *od, double removed, int user)
{
    CI_User *u = CacheUsers::instance()->get(user);
    QString userName = fStaff->fName;
    if (u) {
        userName =  u->fName;
    }
    PPrintScene ps(Portrait);
    PTextRect th;
    QFont f("Arial", 34);
    f.setBold(true);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    PTextRect *r = 0;
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    r = ps.addTextRect(10, top, 680, rowHeight, QString("%1").arg(tr("Cancelation check")), &th);
    top += r->textHeight();
    top += ps.addTextRect(10, top, 680, rowHeight, QString("Order #%1")
                                     .arg(fTable->fOrder),
                                     &th)->textHeight();
    f.setPointSize(25);
    th.setFont(f);
    top += r->textHeight();
    top += ps.addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Table")).arg(fTable->fName), &th, f))->textHeight();
    top += ps.addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Time"))
                                        .arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th, f))->textHeight();
    top += ps.addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("User"))
                                        .arg(userName), &th, f))->textHeight();
    ps.addLine(10, top, 680, top);
    th.setTextAlignment(Qt::AlignLeft);
    ps.addTextRect(new PTextRect(10, top, 80, rowHeight, float_str(removed, 1), &th, f));
    top += ps.addTextRect(new PTextRect(90, top, 680, rowHeight, od->fName[def_lang], &th, f))->textHeight();

    QPrinterInfo pi;
    if (!od->fPrint1.isEmpty()) {
        if (!pi.availablePrinterNames().contains(od->fPrint1, Qt::CaseInsensitive)) {
            message_error(QString("Printer %1 is not exists").arg(od->fPrint1));
        } else {
            QPrinter printer;
            QPrintDialog pd(&printer, this);
            printer.setPrinterName(od->fPrint1.toUpper());
            int scale = 3; //600 / printer.resolution();
            QMatrix m;
            m.scale(scale, scale);
            QPainter painter(&printer);
            painter.setMatrix(m);
            ps.render(&painter);
            //message_info("print on " + od->fPrint1);
        }
    }
    if (!od->fPrint2.isEmpty()) {
        if (!pi.availablePrinterNames().contains(od->fPrint2, Qt::CaseInsensitive)) {
            message_error(QString("Printer %1 is not exists").arg(od->fPrint2));
        } else {
            QPrinter printer;
            QPrintDialog pd(&printer, this);
            printer.setPrinterName(od->fPrint2.toUpper());
            int scale = 3;//600 / printer.resolution();
            QMatrix m;
            m.scale(scale, scale);
            QPainter painter(&printer);
            painter.setMatrix(m);
            ps.render(&painter);
            //message_info("print on " + od->fPrint2);
        }
    }
}

void RDesk::printReceipt(bool printModePayment)
{
    int trackUser = fStaff->fId;
    if (!printModePayment) {
        if (fTable->fPrint > 0) {
            if (!right(cr__o_print_unlimited_receipt, trackUser)) {
                return;
            }
        }
    }
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    } else {
        userName = "#Username Error";
    }

    for (int i = 0, count = ui->tblOrder->rowCount(); i < count; i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (od->fComplex) {
            od->fPrice = 0;
            od->fTotal = 0;
            updateDish(od);
        }
    }
    ui->tblOrder->viewport()->update();

    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 30);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    PTextRect thdc;
    f.setPointSize(14);
    thdc.setFont(f);
    thdc.setBorders(false, false, false, false);
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(150, top, 400, 250));
    top += 250;
    f.setPointSize(36);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight + 10, fHall->fName, &th, f))->textHeight();
    top += 20;
    f.setPointSize(20);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("Receipt S/N "))
                                     .arg(fTable->fOrder),
                                     &th, f))->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Table"), &th, f));
    top += ps->addTextRect(new PTextRect(210, top, 200, rowHeight, fTable->fName, &th, f))->textHeight();
    if (!fCarModel.isEmpty()) {
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Car"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 400, rowHeight, fCarModel + ": " + fCarGovNum, &th, f))->textHeight();
    }
    ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Date"), &th, f));
    top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, WORKING_DATE.toString(def_date_format), &th, f))
            ->textHeight();
    if (!printModePayment) {
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Time"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 200, rowHeight, QTime::currentTime().toString(def_time_format), &th, f))->textHeight();
    }
    ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Waiter"), &th, f));
    top += ps->addTextRect(new PTextRect(210, top, 500, rowHeight, userName, &th, f))->textHeight();
    if (printModePayment) {
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Opened"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 350, rowHeight, fTable->fOpened.toString(def_date_time_format), &th, f))->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Closed"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 350, rowHeight, QDateTime::currentDateTime().toString(def_date_time_format), &th, f))->textHeight();
    }
    top += 2;
    ps->addLine(10, top, 680, top);
    top += 2;
    ps->addTextRect(new PTextRect(10, top, 100, rowHeight, tr("Qty"), &th, f));
    ps->addTextRect(new PTextRect(110, top, 390, rowHeight, tr("Description"), &th, f));
    top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, tr("Amount"), &th, f))->textHeight();
    top ++;
    ps->addLine(10, top, 680, top);
    top += 2;
    f.setPointSize(18);
    f.setBold(true);
    th.setFont(f);
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i,0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc==0, print receipt");
            return;
        }
        ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(dc->fQty, 1), &th, f));
        ps->addTextRect(new PTextRect(110, top, 390, rowHeight, dc->fName[def_lang], &th, f));
        top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(dc->fPrice * dc->fQty, 2), &th, f))->textHeight();
        if (top > sizePortrait.height()  - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
        foreach (DishStruct *ds, dc->fDishes) {
            //ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(dc->fQty, 1), &thdc, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, "**" + ds->fName[def_lang], &thdc, f));
            QString total = float_str(ds->fPrice * dc->fQty, 2);
            if (ds->fPrice < 0.1) {
                total = tr("Present");
            }
            top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, total, &thdc, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (od->fComplex > 0) {
            continue; //handle complex
        }
        ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(od->fQty, 1), &th, f));
        th.setWrapMode(QTextOption::WordWrap);
        int t = ps->addTextRect(new PTextRect(110, top, 390, rowHeight, od->fName[def_lang], &th, f))->textHeight();
        int k = ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(od->fTotal, 2), &th, f))->textHeight();
        top += (t > k ? t : k);
        if (top > sizePortrait.height()  - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    if (!ui->tblTotal->item(2, 0)->data(Qt::DisplayRole).toString().isEmpty()){
        ps->addTextRect(new PTextRect(10, top, 400, rowHeight, ui->tblTotal->item(2, 0)->data(Qt::DisplayRole).toString(), &th, f));
        top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, ui->tblTotal->item(2, 1)->data(Qt::DisplayRole).toString(), &th, f))->textHeight();
    }
    top += 2;
    ps->addLine(10, top, 680, top);
    top += 2;
    f.setPointSize(24);
    th.setFont(f);
    ps->addTextRect(new PTextRect(10, top, 400, rowHeight, tr("Total, AMD"), &th, f));
    top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, ui->tblTotal->item(1, 1)->data(Qt::EditRole).toString(), &th, f))->textHeight();


    top += rowHeight;
    f.setPointSize(28);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignHCenter);

    if (top > sizePortrait.height()  - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    if (!fTable->fRoomComment.isEmpty()) {
       top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, fTable->fRoomComment, &th, f))->textHeight();
       top += rowHeight;
       top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("Signature"), &th, f))->textHeight();
       top += rowHeight + 2;
       ps->addLine(150, top, 680, top);
    }

    if (top > sizePortrait.height()  - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    if (fTable->fPaymentMode == PAYMENT_COMPLIMENTARY) {
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("COMPLIMENTARY"), &th, f))->textHeight();
    } else {
        //top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("SALES"), &th, f))->textHeight();
    }
    if (printModePayment) {
        th.setTextAlignment(Qt::AlignLeft);
        PTextRect thr(th, "");
        thr.setTextAlignment(Qt::AlignRight);
        DatabaseResult dr;
        fDbBind[":f_id"] = fTable->fOrder;
        dr.select(fDb, "select * from o_header_payment where f_id=:f_id", fDbBind);
        if (dr.rowCount() == 0) {
            message_error(tr("Error while printing receipt"));
            return;
        }
        if (dr.value("f_cash").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Cash"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_cash").toDouble(), 2), &thr)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_card").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Card"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_card").toDouble(), 2), &thr)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_coupon").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Coupon"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_coupon").toDouble(), 2), &thr)->textHeight();
            top += ps->addTextRect(10, top, 600, rowHeight, dr.value("f_couponSeria").toString() + "/" + dr.value("f_couponNumber").toString(), &thr)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_debt").toDouble() > 0.1) {
            DatabaseResult drh;
            fDbBind[":f_debtHolder"];
            fDbBind[":f_id"] = fTable->fOrder;
            drh.select(fDb, "select f_name from o_debt_holder h "
                       "left join o_header_payment p on p.f_debtHolder=h.f_id "
                       "where p.f_id=:f_id", fDbBind);
            ps->addTextRect(10, top, 600, rowHeight, tr("Debt"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_debt").toDouble(), 2), &thr)->textHeight();
            if (drh.rowCount() > 0) {
                top += ps->addTextRect(10, top, 680, rowHeight, drh.value("f_name").toString(), &th)->textHeight();
            }
            top += 20;
            top += ps->addTextRect(10, top, 600, rowHeight, tr("Signature"), &th)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_discount").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Discount"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_discount").toDouble(), 2), &thr)->textHeight();
            fDbBind[":f_card"] = dr.value("f_costumer");
            dr.select(fDb, "select f_name from d_car_client where f_id=:f_card", fDbBind);
            if (dr.rowCount() > 0) {
                top += ps->addTextRect(10, top, 600, rowHeight, dr.value("f_name").toString(), &thr)->textHeight();
            } else {
                top += ps->addTextRect(10, top, 600, rowHeight, tr("Unknown costumer"), &thr)->textHeight();
            }
            ps->addLine(10, top, 680, top);
            top++;
        }
    }

    if (fShowRemoved) {
        top += (rowHeight * 3);
        top += ps->addTextRect(10, top, 600, rowHeight, tr("****VOID****"), &th)->textHeight();
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (!od) {
                continue;
            }
            if (od->fState != DISH_STATE_REMOVED_STORE) {
                continue;
            }
            if (od->fComplex > 0) {
                continue; //handle complex
            }
            ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(od->fQty, 1), &th, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, od->fName[def_lang], &th, f));
            top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(od->fTotal, 2), &th, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
        top += rowHeight;
        top += ps->addTextRect(10, top, 600, rowHeight, tr("****MISTAKE****"), &th)->textHeight();
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (!od) {
                continue;
            }
            if (od->fState != DISH_STATE_REMOVED_NOSTORE) {
                continue;
            }
            if (od->fComplex > 0) {
                continue; //handle complex
            }
            ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(od->fQty, 1), &th, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, od->fName[def_lang], &th, f));
            top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(od->fTotal, 2), &th, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
    }

    //Finish
    top += rowHeight;
    ps->addTextRect(new PTextRect(10, top, 680, rowHeight, "_", &th, f));

    QPrinter printer;
    qDebug() << defrest(dr_second_receipt_printer);
    qDebug() << defrest(dr_first_receipt_printer);
    printer.setPrinterName("local");
    QMatrix m;
#ifdef QT_DEBUG
    m.scale(1, 1);
#else
    m.scale(3, 3);
#endif
    QPainter painter(&printer);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }

    fTable->fPrint = abs(fTable->fPrint) + 1;
    fDbBind[":f_print"] = fTable->fPrint;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    changeBtnState();
    QString v = printModePayment ? "v2" : "v1";
    fTrackControl->insert("Print receipt " + v, fTable->fPaymentComment + " " + fTable->fRoomComment, "");
}

void RDesk::changeBtnState()
{
    bool emptyReceipt = true;
    bool btnPrintService = false;
    bool btnPrintReceipt = false;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        float qty = od->fQty - od->fQtyPrint;
        emptyReceipt = false;
        if (qty > 0.1) {
            btnPrintService = true;
            break;
        }
    }
    btnPrintReceipt = !btnPrintService;
    btnPrintReceipt = btnPrintReceipt;
    ui->btnPayment_2->setEnabled(btnPrintReceipt);
    ui->btnPrint->setEnabled(btnPrintService);
    ui->btnPayment->setEnabled(btnPrintReceipt && !emptyReceipt && fTable->fPrint > 0);

    updateTableInfo();
}

void RDesk::checkEmpty()
{
    if (!fTable) {
        return;
    }
    if (fTable->fOrder == 0) {
        return;
    }

    bool orderEmpty = true;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState == DISH_STATE_READY) {
            orderEmpty = false;
        }
    }
    if (orderEmpty) {
        fDbBind[":f_state"] = ORDER_STATE_EMPTY;
        fDbBind[":f_dateClose"] = QDateTime::currentDateTime();
        fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
        fDbBind[":f_order"] = "";
        fDb.update("r_table", fDbBind, where_id(fTable->fId));
    }
}

void RDesk::resetPrintQty()
{
    if (fTable->fPrint > 0) {
        fTable->fPrint = (fTable->fPrint) * -1;
        fDbBind[":f_print"] = fTable->fPrint;
        fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    }
}

void RDesk::updateDishQtyHistory(OrderDishStruct *od)
{
    fDbBind[":f_rec"] = od->fRecId;
    fDbBind[":f_user"] = fStaff->fName;
    fDbBind[":f_date"] = QDateTime::currentDateTime();
    fDbBind[":f_info"] = QString("%1/%2").arg(od->fQty).arg(od->fQtyPrint);
    fDb.insertWithoutId("o_dish_qty", fDbBind);
}

void RDesk::updateTableInfo()
{

    ui->lbCar->setText(fCarModel + " " + fCarGovNum);
}

void RDesk::manualdisc(double val, int costumer)
{
    DatabaseResult dr;
    fDbBind[":f_id"] = fTable->fOrder;
    dr.select(fDb, "select * from o_temp_disc where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        message_error(tr("Discount already used"));
        return;
    }
    if (!message_question(QString("%1 %2")
                         .arg(tr("Confirm to discount"))
                          .arg(val * 100)) ) {
        return;
    }
    double totalDisc = 0;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        double disc = (od->fPrice * val);
        totalDisc += disc;
        od->fPrice = od->fPrice - disc;
        od->fTotal = od->fQty * od->fPrice;
        updateDish(od);
    }
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc == 0, countTotal");
            return;
        }
        dc->fPrice -= dc->fPrice * val;
        fDbBind[":f_price"] = dc->fPrice;
        fDbBind[":f_total"] = dc->fPrice * dc->fQty;
        fDbBind[":f_totalusd"] = dc->fPrice * dc->fQty;
        fDb.update("o_dish", fDbBind, where_id(dc->fRecId));

    }
    fDbBind[":f_id"] = fTable->fOrder;
    fDbBind[":f_costumer"] = costumer;
    fDbBind[":f_val"] = val;
    fDbBind[":f_amount"] = totalDisc;
    fDb.insertWithoutId("o_temp_disc", fDbBind);
    countTotal();
    ui->tblOrder->viewport()->update();
    fTrackControl->insert(QString("Discount %1%").arg(val), "", "");

    changeBtnState();
}

void RDesk::logtime(const QString &msg, int elapsed)
{
//    QDir d;
//    QFile f(d.homePath() + "/SmartHotel/logtime.log");
//    f.open(QIODevice::Append);
//    f.write(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss ").toUtf8());
//    f.write(msg.toUtf8());
//    f.write(": ");
//    f.write(QString::number(elapsed).toUtf8());
//    f.write("\r\n");
//    f.close();
}

TableStruct *RDesk::loadHall(int hall)
{
    fCurrentHall = hall;
    int row = 0, col = 0;
    for (int i = 0; i < Hall::fTables.count(); i++) {
        if (row > 2) {
            break;
        }
        TableStruct *t = Hall::fTables.at(i);
        if (t->fHall != hall) {
            continue;
        }
        ui->tblTables->item(row, col)->setText(t->fName);
        ui->tblTables->item(row, col)->setData(Qt::UserRole, qVariantFromValue(t));
        col ++;
        if (col > 2) {
            col = 0;
            row ++;
        }
    }

    HallStruct *hs = Hall::getHallById(hall);
    if (hs == nullptr) {
        message_error(tr("Hall is empty"));
        return nullptr;
    }
    fMenu = hs->fDefaultMenu;
    setupType(0);
    TableStruct *ts = nullptr; //ui->tblTables->item(0, 0)->data(Qt::UserRole).value<TableStruct*>();
    setTable(ts);
    ui->tblTables->viewport()->update();
    return ts;
}

void RDesk::on_tblPart_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    DishPartStruct *p = index.data(Qt::UserRole).value<DishPartStruct*>();
    if (!p) {
        return;
    }
    setupType(p->fId);
}

void RDesk::on_tblType_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    TypeStruct *t = index.data(Qt::UserRole).value<TypeStruct*>();
    if (!t) {
        return;
    }
    setupDish(t->fId);
}

void RDesk::on_tblDish_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    if (fTable == nullptr) {
        message_error(tr("Please, select table"));
        return;
    }
    DishStruct *d = index.data(Qt::UserRole).value<DishStruct*>();
    if (!d) {
        return;
    }
    fCloseTimeout = 0;
    addDishToOrder(d, true);
    repaintTables();
}

void RDesk::on_btnTrash_clicked()
{
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    ui->tblOrder->clearSelection();
    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
    if (!od) {
        return;
    }
    if (!message_question(tr("Confirm to delete the selected item"))) {
        return;
    }
    QString oldQty = QString("%1, %2/%3").arg(od->fName[def_lang]).arg(od->fQty).arg(od->fQtyPrint);
    int trackUser = fStaff->fId;
    if (od->fComplex == 0) {
        if (od->fQtyPrint < 0.01) {
            od->fState = DISH_STATE_EMPTY;
            ui->tblOrder->setRowHidden(sel.at(0).row(), true);
        } else {
            float num = 0;
            float qtyToPrint = 0;
            QVariant reason;
            if (RNumbers::getNumber(num, od->fQty, this)) {
                qtyToPrint = od->fQtyPrint - (od->fQty - num);
                if (qtyToPrint > 0.01) {
                    if (!right(cr__o_cancelation, trackUser)) {
                        message_error(tr("Access denied"));
                        return;
                    }
                    QString name, sql = "select f_id, f_en from o_dish_state where f_id in (2, 3)";
                    if (!DlgList::getValue(tr("REMOVE OPTION"), name, reason, sql)) {
                        return;
                    }
                } else {
                    reason = DISH_STATE_REMOVED_NOSTORE;
                }
                if (float_equal(od->fQty, num)) {
                    od->fState = reason.toInt();
                    od->fCancelUser = trackUser;
                    od->fCancelDate = QDateTime::currentDateTime();
                    setOrderRowHidden(sel.at(0).row(), od);
                } else {
                    float qtyEmpty = od->fQty - od->fQtyPrint;
                    if (float_greaterOrEqual(qtyEmpty, num)) {
                        od->fQty -= num;
                    } else {
                        od->fQty -= qtyEmpty;
                        num -= qtyEmpty;
                        //Log empty
                        if (float_equal(od->fQty, num)) {
                            od->fState = reason.toInt();
                            od->fCancelUser = trackUser;
                            od->fCancelDate = QDateTime::currentDateTime();
                        } else {
                            od->fQty -= num;
                            od->fQtyPrint = od->fQty;
                            OrderDishStruct *r = new OrderDishStruct();
                            *r = *od;
                            r->fState = reason.toInt();
                            r->fQty = num;
                            r->fQtyPrint = num;
                            fDbBind[":f_header"] = fTable->fOrder;
                            fDbBind[":f_state"] = r->fState;
                            fDbBind[":f_dish"] = r->fDishId;
                            fDbBind[":f_qty"] = r->fQty;
                            fDbBind[":f_qtyPrint"] = r->fQtyPrint;
                            fDbBind[":f_price"] = r->fPrice;
                            fDbBind[":f_svcValue"] = r->fSvcValue;
                            fDbBind[":f_svcAmount"] = r->fSvcAmount;
                            fDbBind[":f_dctValue"] = r->fDctValue;
                            fDbBind[":f_dctAmount"] = r->fDctAmount;
                            fDbBind[":f_total"] = r->fTotal;
                            fDbBind[":f_print1"] = r->fPrint1;
                            fDbBind[":f_print2"] = r->fPrint2;
                            fDbBind[":f_store"] = r->fStore;
                            fDbBind[":f_comment"] = r->fComment;
                            fDbBind[":f_staff"] = fStaff->fId;
                            fDbBind[":f_cancelUser"] = trackUser;
                            fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
                            r->fRecId = fDb.insert("o_dish", fDbBind);
                            addDishToTable(r, true);
                        }
                    }
                }
                if (qtyToPrint > 0.01) {
                    printRemovedDish(od, qtyToPrint, trackUser);
                }
            }
        }
        QString newQty = QString("%1, %2/%3").arg(od->fName[def_lang]).arg(od->fQty).arg(od->fQtyPrint);
        fTrackControl->insert("Dish qty", oldQty, newQty);
        countDish(od);
        updateDish(od);
        resetPrintQty();
    } else {
        //if od.f_complex > 0
        if (od->fQtyPrint > 0.1) {
            if (!right(cr__o_cancelation, trackUser)) {
                message_error(tr("Access denied"));
                return;
            }
        }
        if (message_yesnocancel(tr("Are you sure remove whole complex?")) != RESULT_YES) {
            return;
        }
        QVariant reason;
        QString name, sql = "select f_id, f_en from o_dish_state where f_id in (2, 3)";
        if (!DlgList::getValue(tr("REMOVE OPTION"), name, reason, sql)) {
            return;
        }
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *o = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (o->fComplexRecId == od->fComplexRecId) {
                o->fState = reason.toInt();
                ui->tblOrder->setRowHidden(i, !fShowRemoved);
                updateDish(o);
                fTrackControl->insert("Remove complex component", o->fName["en"], "");
            }
        }
        for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
            DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
            if (dc->fRecId == od->fComplexRecId) {
                fDbBind[":f_state"] = reason.toInt();
                fDb.update("o_dish", fDbBind, where_id(ap(dc->fRecId)));
                ui->tblComplex->removeRow(i);
                fTrackControl->insert("Remove complex", dc->fName["en"], "");
                delete dc;
                break;
            }
        }
        fDbBind[":f_state"] = reason;
        fDb.update("o_dish", fDbBind, where_id(od->fComplexRecId));
    }
    countTotal();
    changeBtnState();
    repaintTables();
}

void RDesk::on_btnPayment_clicked()
{
    fCloseTimeout = 0;

    if (fTable->fHall == 1) {
        if (fCarModel.isEmpty()) {
            message_error(tr("Car model must be selected"));
            return;
        }
        if (fCostumerId == 0) {
            message_error(tr("Costumer must be selected"));
            return;
        }
    }
    if (!DlgPayment::payment(fTable->fOrder)) {
        return;
    }
    printReceipt(true);
    closeOrder();
    repaintTables();
    if (fTable) {
        fTable->fAmount = "";
    }
    changeBtnState();
}

void RDesk::on_btnPrint_clicked()
{
    QSet<QString> prn1, prn2;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (od->fQty - od->fQtyPrint > 0.1) {
            if (!od->fPrint1.isEmpty()) {
                prn1 << od->fPrint1;
            }
            if (!od->fPrint2.isEmpty()) {
                prn2 << od->fPrint2;
            }
        }
    }
    bool printed = false;
    if (prn1.count() > 0) {
        for (QSet<QString>::const_iterator prn = prn1.begin(); prn != prn1.end(); prn++) {
            printServiceCheck(*prn, 1);
        }
        printed = true;
    }
    if (prn2.count() > 0) {
        for (QSet<QString>::const_iterator prn = prn2.begin(); prn != prn2.end(); prn++) {
            printServiceCheck(*prn, 2);
        }
        printed = true;
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        od->fQtyPrint = od->fQty;
        updateDish(od);
    }
    ui->tblOrder->viewport()->update();
    if (printed) {
        fTrackControl->insert("Printed service check", "", "");
    }
    changeBtnState();
    repaintTables();
}

void RDesk::on_btnTable_clicked()
{
    RSelectTable *t = new RSelectTable(this);
    t->setup(fTable->fHall);
    if (t->exec() == QDialog::Accepted) {
        if (setTable(t->table())) {
            fHall = Hall::getHallById(fTable->fHall);
            if (fMenu != fHall->fDefaultMenu) {
                fMenu = fHall->fDefaultMenu;
                setBtnMenuText();
                setupType(0);
            }
        }
    }
    delete t;
    fCloseTimeout = 0;
    changeBtnState();
}

void RDesk::on_btnComment_clicked()
{
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
    if (!od) {
        return;
    }
    if (!od->fComment.isEmpty()) {
        if (od->fQtyPrint > 0.01) {
            message_error(tr("You cannot edit comment for this item"));
            return;
        }
    }
    if (od->fQtyPrint > 0.01) {
        message_error(tr("You cannot edit comment for this item"));
        return;
    }
    QString comment;
    if (RDishComment::getComment(comment, this)) {
        od->fComment = comment;
        updateDish(od);
        ui->tblOrder->viewport()->update();
    }
}

void RDesk::on_btnTools_clicked()
{
    RTools *t = new RTools(this);
    if (t->exec() == QDialog::Accepted) {

    }
    delete t;
}

void RDesk::on_btnCheckout_clicked()
{
    printReceipt(false);
    fCloseTimeout = 0;
}

int RDesk::right(int right, int &trackUser)
{
    bool access = RIGHT(fStaff->fGroup, right);
    trackUser = fStaff->fId;
    if (!access) {
        trackUser = 0;
        QString login;
        if (RLogin::getLogin(login, tr("Raise privileges"), this)) {
            User u(login);
            if (u.isValid()) {
                access = RIGHT(u.fGroup, right);
                if (access) {
                    trackUser = u.fId;
                }
            }
        }
    }
    if (!access) {
        message_error(tr("Access denied"));
    }
    return access;
}

void RDesk::on_btnTypeUp_clicked()
{
    ui->tblType->verticalScrollBar()->setValue(ui->tblType->verticalScrollBar()->value() - 6);
}

void RDesk::on_btnTypeDown_clicked()
{
    ui->tblType->verticalScrollBar()->setValue(ui->tblType->verticalScrollBar()->value() + 6);
}

void RDesk::on_btnDishUp_clicked()
{
    ui->tblDish->verticalScrollBar()->setValue(ui->tblDish->verticalScrollBar()->value() - 6);
}

void RDesk::on_btnDishDown_clicked()
{
    ui->tblDish->verticalScrollBar()->setValue(ui->tblDish->verticalScrollBar()->value() + 6);
}

void RDesk::on_btnOrdDown_clicked()
{
    ui->tblOrder->verticalScrollBar()->setValue(ui->tblOrder->verticalScrollBar()->value() + 6);
}

void RDesk::on_btnOrdUp_clicked()
{
    ui->tblOrder->verticalScrollBar()->setValue(ui->tblOrder->verticalScrollBar()->value() - 6);
}

void RDesk::on_btnSetRoom_clicked()
{
    DlgReservation *d = new DlgReservation(this);
    d->loadRoom();
    if (d->exec() == QDialog::Accepted) {
        fDbBind[":f_id"] = d->fReservationId;
        fDb.select("select r.f_room, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName) "
                   "from f_reservation r "
                   "inner join f_guests g on g.f_id=r.f_guest "
                   "where r.f_id=:f_id", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            fDbBind[":f_roomComment"] = fDbRows.at(0).at(0).toString() + ", " + fDbRows.at(0).at(1).toString();
            fTable->fRoomComment = fDbBind[":f_roomComment"].toString();
            fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
        }
    }
}

void RDesk::on_btnComplex_clicked()
{
    setComplexMode();
    fCloseTimeout = 0;
}

void RDesk::repaintTables()
{
    for (int c = 0; c < ui->tblTables->columnCount(); c++) {
        for (int r = 0; r < ui->tblTables->rowCount(); r++) {
            TableStruct *t = ui->tblTables->item(r, c)->data(Qt::UserRole).value<TableStruct*>();
            if (!t) {
                continue;
            }
            t->fOrder = 0;
            t->fAmount = "0";
        }
    }
    fDbBind[":f_state"] = DISH_STATE_READY;
    QString query = "select t.f_id, t.f_lockHost, t.f_order, "
            "h.f_dateOpen, h.f_comment, u.f_firstName, "
            "sum(d.f_total) "
            "from r_table t "
            "left join o_header h on t.f_order=h.f_id "
            "left join users u on u.f_id=h.f_staff "
            "left join o_dish d on t.f_order=d.f_header "
            "where d.f_state=:f_state and t.f_hall=" +  QString::number(fCurrentHall) +  " "
            "group by 1 ";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        for (int c = 0; c < ui->tblTables->columnCount(); c++) {
            for (int r = 0; r < ui->tblTables->rowCount(); r++) {
                TableStruct *t = ui->tblTables->item(r, c)->data(Qt::UserRole).value<TableStruct*>();
                if (!t) {
                    continue;
                }
                if (t->fId == it->at(0).toInt()) {
                    t->fOrder = it->at(2).toInt();
                    t->fAmount = it->at(6).toString();
                    goto GO;
                }
            }
        }
        GO:
        continue;
    }
    ui->tblTables->viewport()->update();
}

void RDesk::on_tblTables_itemClicked(QTableWidgetItem *item)
{
    if (!item) {
        return;
    }
    TableStruct *t = item->data(Qt::UserRole).value<TableStruct*>();
    if (!t) {
        return;
    }
    setTable(t);
    repaintTables();
}

void RDesk::on_btnPayment_2_clicked()
{
    if (fTable->fPrint > 0) {
        return;
    }
    printReceipt(false);
    changeBtnState();
}

void RDesk::on_btnSetCar_clicked()
{
    if (!fTable) {
        message_error(tr("Please, select table"));
        return;
    }
    if (fTable->fOrder == 0) {
        message_error(tr("Emtpy order"));
        return;
    }
    int model = fCarId;
    QString govNum = fCarGovNum;
    int costumer = fCostumerId;
    DlgCarSelection::selectCar(model, govNum, costumer);
    DatabaseResult dro;
    fDbBind[":f_order"] = fTable->fOrder;
    dro.select(fDb, "select * from o_car where f_order=:f_order", fDbBind);
    if (dro.rowCount() == 0) {
        fDbBind[":f_order"] = fTable->fOrder;
        fDb.insert("o_car", fDbBind);
    }
    govNum = govNum.toUpper();
    fDbBind[":f_model"] = model;
    fDbBind[":f_order"] = fTable->fOrder;
    fDbBind[":f_govNumber"] = govNum;
    fDbBind[":f_costumer"] = costumer;
    fDb.select("update o_car set f_model=:f_model, f_govNumber=:f_govNumber, f_costumer=:f_costumer where f_order=:f_order", fDbBind, fDbRows);
    fCarId = model;
    fCarGovNum = govNum;
    fCostumerId = costumer;
    fDbBind[":f_id"] = model;
    DatabaseResult dr;
    dr.select(fDb, "select concat(f_model, ' ', f_class) as car from d_car_model where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        fCarModel = dr.value("car").toString();
        ui->lbCar->setText(fCarModel + " " + fCarGovNum);
    }
    fDbBind[":f_govNumber"] = govNum;
    dr.select(fDb, "select * from o_debt_holder_car where upper(f_govNumber)=upper(:f_govNumber)", fDbBind);
    if (dr.rowCount() == 0) {
        fDbBind[":f_holder"] = costumer;
        fDbBind[":f_govNumber"] = govNum;
        fDb.insert("o_debt_holder_car", fDbBind);
    }
}

void RDesk::on_btnDiscount_clicked()
{
    message_error(tr("No discount mechanism"));
}

void RDesk::on_btnPackage_clicked()
{
    setComplexMode();
}

void RDesk::on_btnHallWash_clicked()
{
    loadHall(1);
}

void RDesk::on_btnHallCafe_clicked()
{
    loadHall(2);
}

void RDesk::on_btnExit_2_clicked()
{
    manualdisc(0.2, defrest(dr_discount_20).toInt());
}

void RDesk::on_btnDiss50_clicked()
{
    manualdisc(0.5, defrest(dr_discount_50).toInt());
}

void RDesk::on_btnHallVIP_clicked()
{
    loadHall(3);
}

void RDesk::on_btnShop_clicked()
{
    loadHall(4);
}
