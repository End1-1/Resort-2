#include "rdesk.h"
#include <QDir>
#include <QElapsedTimer>
#include <QInputDialog>
#include <QItemDelegate>
#include <QJsonDocument>
#include <QImage>
#include <QJsonObject>
#include <QPrinter>
#include <QPrinterInfo>
#include <QScrollBar>
#include <QTimer>
#include "baseorder.h"
#include "branchstoremap.h"
#include "cachecar.h"
#include "cacherights.h"
#include "cacheusers.h"
#include "checktime.h"
#include "database2.h"
#include "databaseresult.h"
#include "defrest.h"
#include "customerdisplay.h"
#include "dlgcalc.h"
#include "rmessage.h"
#include "winprinternames.h"

namespace {

QString resolveSystemPrinterName(const QString &requested, QStringList *availablePrinters)
{
#ifdef Q_OS_WIN
    const QStringList available = winInstalledPrinterNames();
#else
    const QStringList available = QPrinterInfo::availablePrinterNames();
#endif
    if(availablePrinters) {
        *availablePrinters = available;
    }
    const QString trimmed = requested.trimmed();
    for(const QString &name : available) {
        if(name.compare(trimmed, Qt::CaseInsensitive) == 0) {
            return name;
        }
    }
    return QString();
}

void showPrinterNotInstalledError(QWidget *parent, const QString &requested, const QStringList &availablePrinters)
{
    const QString listText = availablePrinters.isEmpty()
                                 ? QObject::tr("(no printers found)")
                                 : availablePrinters.join("<br>");
    RMessage::showError(QObject::tr("Printer \"%1\" is not installed on this computer.")
                        .arg(requested.toHtmlEscaped())
                        + "<br><br>"
                        + QObject::tr("Available printers:") + "<br>"
                        + listText,
                        parent);
}

} // namespace
#include "dlgcarselection.h"
#include "dlgdate.h"
#include "dlgdeptholder.h"
#include "dlglist.h"
#include "dlgpassword.h"
#include "dlgpayment.h"
#include "dlgprintmultiplefiscal.h"
#include "dlgsalary.h"
#include "dlgsmile.h"
#include "logging.h"
#include "logwriter.h"
#include "paymentmode.h"
#include "pprintreceipt.h"
#include "restaurantc5print.h"
#include "printtaxno.h"
#include "rchangelanguage.h"
#include "rchangemenu.h"
#include "rdishcomment.h"
#include "reportprint.h"
#include "rmodifiers.h"
#include "rnumbers.h"
#include "rtools.h"
#include "dlgsessionopen.h"
#include "orderlog.h"
#include "session.h"
#include "sessiondebtclose.h"
#include "ui_rdesk.h"
#include <Windows.h>

static QString emarkForDb(const QString &emark, const QString &adgt)
{
    if (emark.isEmpty() || emark == adgt || !isValidEmarkCode(emark)) {
        return QString();
    }

    return emark;
}

QMap<int, DishStruct*> RDesk::fQuickDish;

class PartItemDelegate : public QItemDelegate
{
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->fillRect(option.rect, QBrush());

        if(!index.isValid()) {
            return;
        }

        DishPartStruct *p = index.data(Qt::UserRole).value<DishPartStruct*>();

        if(!p) {
            return;
        }

        if(option.state & QStyle::State_Selected) {
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

class TypeItemDelegate : public QItemDelegate
{
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->fillRect(option.rect, Qt::white);
        TypeStruct *t = index.data(Qt::UserRole).value<TypeStruct*>();

        if(!t) {
            return;
        }

        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        o.setWrapMode(QTextOption::WordWrap);
        painter->fillRect(option.rect, QColor::fromRgb(t->fBgColor));

        if(option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(42, 42, 42));
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

class DishItemDelegate : public QItemDelegate
{
public:
    DishItemDelegate(RDesk *desk)
    {
        fDesk = desk;
    }
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->fillRect(option.rect, Qt::white);

        if(!index.isValid()) {
            return;
        }

        DishStruct *d = index.data(Qt::UserRole).value<DishStruct*>();

        if(!d) {
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
        QString text = QString("%1 [%2]").arg(d->fName, QString::number(d->fPrice, 'f', 0));
        painter->drawText(textRect, text, o);
    }
private:
    RDesk* fDesk;
};

class OrderDishDelegate : public QItemDelegate
{
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->save();
        painter->fillRect(option.rect, Qt::white);

        if(!index.isValid()) {
            return;
        }

        OrderDishStruct *od = index.data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od) {
            return;
        }

        QFont f(qApp->font());

        if(option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(42, 42, 42));
            painter->setPen(Qt::white);
            f.setBold(true);
            painter->setFont(f);
        } else {
            painter->setPen(QColor::fromRgb(42, 42, 42));
        }

        if(od->fState != DISH_STATE_READY) {
            f.setStrikeOut(true);
            painter->setFont(f);
        }

        QTextOption o;
        o.setWrapMode(QTextOption::WordWrap);

        switch(index.column()) {
        case 0: {
            QRect textRect = option.rect;
            textRect.adjust(2, 2, -2, -2);
            painter->drawText(textRect, od->fName, o);
            f.setPointSize(8);
            painter->setFont(f);
            int h = QFontMetrics(f).height();
            QRect commentRect = textRect;
            commentRect.adjust(0, commentRect.height() - h - 1, 0, 0);
            painter->drawText(commentRect, od->fComment, o);

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

class TotalItemDelegate : public QItemDelegate
{
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->setPen(Qt::white);
        QItemDelegate::paint(painter, option, index);
    }
};

class TablesItemDeletgate : public QItemDelegate
{
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QItemDelegate::paint(painter, option, index);
        TableStruct *t = index.data(Qt::UserRole).value<TableStruct*>();

        if(!t) {
            return;
        }

        if(t->fHall == 1 || t->fHall == 5 || t->fHall == 9) {
            painter->fillRect(option.rect, Qt::black);
            painter->setPen(Qt::white);

            if(t->fAmount.toDouble() > 0.1 || t->fOrder > 0) {
                painter->fillRect(option.rect, Qt::yellow);
                painter->setPen(Qt::black);
            }

            QTextOption o;
            o.setAlignment(Qt::AlignLeft);
            QRect r = option.rect;
            r.adjust(5, 5, -10, 0);
            QFont font = painter->font();
            font.setPointSize(12);
            font.setBold(true);
            painter->setFont(font);
            painter->drawText(r, t->fName, o);
            font.setBold(false);
            painter->setFont(font);
            o.setAlignment(Qt::AlignRight);
            painter->drawText(r, t->fAmount, o);

            if(t->fOrder > 0) {
                o.setWrapMode(QTextOption::NoWrap);
                font.setPointSize(font.pointSize() - 2);
                painter->setFont(font);
                r.adjust(0, 15, 0, 0);
                o.setAlignment(Qt::AlignLeft);
                painter->drawText(r, t->fCar, o);
                r.adjust(0, 15, 0, 0);
                painter->drawText(r, t->fGovNumber, o);
            }
        } else {
            painter->fillRect(option.rect, Qt::black);
            painter->setPen(Qt::white);

            if(t->fAmount.toDouble() > 0.1 || t->fOrder > 0) {
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
    }
};

RDesk::RDesk(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RDesk)
{
    ui->setupUi(this);
    fCanClose = false;
    fShowRemoved = false;
    fTable  = 0;
    fTimerCounter = 0;
    ui->tblType->setItemDelegate(new TypeItemDelegate());
    ui->tblDish->setItemDelegate(new DishItemDelegate(this));
    ui->tblOrder->setItemDelegate(new OrderDishDelegate());
    ui->tblTotal->setItemDelegate(new TotalItemDelegate());
    ui->tblTables->setItemDelegate(new TablesItemDeletgate());

    if(fQuickDish.count() == 0) {
        //        fDbBind[":f_hall"] = def_default_hall
    }

    fTrackControl = new TrackControl(TRACK_REST_ORDER);
    fHall = Hall::getHallById(fPreferences.getDb(def_default_hall).toInt());
    fPreferences.setDb(def_working_day, QDate::currentDate().toString(def_date_format));
    DatabaseResult dr;
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    dr.select(fDb, "select f_key, f_value from r_config where f_comp=:f_comp", fDbBind);

    for(int i = 0; i < dr.rowCount(); i++) {
        fPreferences.setDb(dr.value(i, "f_key").toString(), dr.value("f_value").toString());
    }

    fCostumerId = 0;
    fCarId = 0;
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RDesk::timeout);
    timer->start(3000);
    ui->btnHallCafe->setVisible(false);
    ui->btnShop->setVisible(false);
    fNoService = false;
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    const int branch = defrest(dr_branch).toInt();
    db2[":f_branch"] = branch;
    db2.exec("select mn.* from r_menu_names mn "
             "inner join r_branch_menu bm on bm.f_menu=mn.f_id and bm.f_branch=:f_branch "
             "where mn.f_enabled=1 "
             "order by mn.f_id");
    QPushButton *firstBtn = nullptr;

    while(db2.next()) {
        auto *b = new QPushButton(db2.string("f_am"));
        const int menuId = db2.integer("f_id");
        b->setProperty("id", menuId);
        b->setProperty("noservice", db2.integer("f_noservice"));
        b->setProperty("needcar", db2.integer("f_needcar"));
        b->setMinimumSize(QSize(150, 40));
        connect(b, &QPushButton::clicked, this, &RDesk::changeMenu);
        ui->wbtn->layout()->addWidget(b);
        if (!firstBtn) {
            firstBtn = b;
        }
    }

    fDataVersion = 0;
    ui->widget->setVisible(false);
    if (firstBtn) {
        firstBtn->click();
    }
}

RDesk::~RDesk()
{
    delete ui;
    delete fStaff;
}

void RDesk::prepareToShow()
{
#ifdef QT_DEBUG
    showMaximized();
#else
    showFullScreen();
#endif
    CustomerDisplay::tryAttachSecondScreen();
    qApp->processEvents();
}

bool RDesk::setup(TableStruct *t)
{
    if(!t) {
        t = ui->tblTables->item(0, 0)->data(Qt::UserRole).value<TableStruct*>();
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

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od) {
            continue;
        }

        setOrderRowHidden(i, od);
    }
}

void RDesk::setOrderComment()
{
    QString comment;

    if(RDishComment::getComment(comment, this)) {
        fTable->fComment = comment;
        fDbBind[":f_comment"] = comment;
        fDb.update("o_header", fDbBind, QString("where f_id='%1'").arg(fTable->fOrder));
        fTrackControl->insert("Set order comment", comment, "");
    }
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

    if(RNumbers::getFloat(num, 100000, "ԳՈՒՄԱՐ", this)) {
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


void RDesk::closeOrder(int state)
{
    //DISABLE AUTOMATIC OUTPUT
    BaseOrder bo(fTable->fOrder);
    bo.calculateOutput(fDb);
    fDbBind[":f_state"] = state;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    fDbBind[":f_dateClose"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fDbBind[":f_order"] = 0;
    fDb.update("r_table", fDbBind, where_id(ap(fTable->fId)));

    if (state == ORDER_STATE_REMOVED) {
        printCanceledOrder(fTable->fOrder);
    }

    clearOrder();
}

void RDesk::printTotalShort()
{
    int trackUser = 1;
    QString userName = fStaff->fName;
    QDate date;

    if(!DlgDate::getDate(date)) {
        return;
    }

    CI_User *u = CacheUsers::instance()->get(trackUser);

    if(u) {
        userName = u->fFull;
    }

    ReportPrint::printTotalShort(date, userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalToday()
{
    int trackUser = 1;
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);

    if(u) {
        userName = u->fFull;
    }

    ReportPrint::printTotal(WORKING_DATE, userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalYesterday()
{
    int trackUser = 1;
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);

    if(u) {
        userName = u->fFull;
    }

    ReportPrint::printTotal(WORKING_DATE.addDays(-1), userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalAnyDay()
{
    int trackUser = 1;
    QString userName = fStaff->fName;
    QDate date;

    if(!DlgDate::getDate(date)) {
        return;
    }

    CI_User *u = CacheUsers::instance()->get(trackUser);

    if(u) {
        userName = u->fFull;
    }

    ReportPrint::printTotal(date, userName, fHall->fReceiptPrinter);
}

void RDesk::printReceiptByNumber()
{
    int trackUser = fStaff->fId;
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);

    if(u) {
        userName = u->fFull;
    } else {
        userName = "#Username Error";
    }

    int ordNum = 0;

    if(!RNumbers::getInt(ordNum, "ՊԱՏՎԵՐԻ ՀԱՄԱՐԸ", this)) {
        return;
    }

    if(ordNum == 0) {
        return;
    }

    PPrintReceipt::printOrder(fHall->fReceiptPrinter, ordNum, trackUser);
}

void RDesk::printVoidReport()
{
    int trackUser = fStaff->fId;
    DatabaseResult dr;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    dr.select(fDb,
              "select oh.f_id, h.f_name as hname, t.f_name as tname, concat(u1.f_firstName, ' ' , u1.f_lastName) as staff, \
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
    ReceiptPrinter printer(fHall->fReceiptPrinter);
    C5Printing doc;
    setupC5Printing(doc, printer.printer());

    doc.image("./logo_print.png", Qt::AlignHCenter);
    doc.br(4);
    doc.setFontSize(receiptFontPt(12));
    doc.setFontBold(true);
    doc.ctext(tr("VOID REPORT"));
    doc.br();
    doc.setFontSize(receiptFontPt(10));
    doc.setFontBold(false);
    doc.ctext(WORKING_DATE.toString(def_date_format));
    doc.br();
    doc.ltext(tr("Printed by ") + CacheUsers::instance()->get(trackUser)->fFull, 0);
    doc.br();
    doc.line();
    doc.br(2);

    for(int i = 0; i < dr.rowCount(); i++) {
        doc.setFontBold(true);
        doc.ltext(QString("%1 / %2 / #%3")
                      .arg(dr.value(i, "hname").toString())
                      .arg(dr.value(i, "tname").toString())
                      .arg(dr.value(i, "f_id").toString()),
                  0);
        doc.br();
        doc.setFontBold(false);
        doc.line();
        doc.ltext(QString("%1 / %2 / %3")
                      .arg(dr.value(i, "dish").toString())
                      .arg(dr.value(i, "f_qty").toString())
                      .arg(dr.value(i, "f_total").toString()),
                  0);
        doc.br();
        doc.ltext(dr.value(i, "staff").toString(), 0);
        doc.br();
        doc.ltext(tr("Type: ") + dr.value(i, "state").toString(), 0);
        doc.br();
        doc.ltext(tr("Manager"), 0);
        doc.br();
        doc.ltext(dr.value(i, "staffcancel").toString(), 0);
        doc.br();
        doc.line();
        doc.br(2);
    }

    doc.ctext("_");
    printC5(doc, printer.printer());
}

void RDesk::openTools()
{
    RTools *t = new RTools(this);
    t->setNoTable();

    if(t->exec() == QDialog::Accepted) {
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

    if(dtotal.rowCount() > 0) {
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

void RDesk::closeSession()
{
    const int sessionId = Session::currentId();

    if(sessionId <= 0) {
        message_error(tr("Session is not open"));
        return;
    }

    const int branch = defrest(dr_branch).toInt();
    int openTables = Session::openTableCount(branch);

    if(openTables < 0) {
        message_error(tr("Cannot close session"));
        return;
    }

    if(openTables > 0) {
        if(!message_question(tr("There are %1 open table(s). Close all orders as debt and finish the session?")
                           .arg(openTables))) {
            return;
        }

        QList<SessionDebtOrder> orders;

        if(!SessionDebtClose::loadOpenOrders(branch, orders)) {
            message_error(tr("Cannot close session"));
            return;
        }

        QString validationError;

        if(!SessionDebtClose::validateOrders(orders, validationError)) {
            message_error(validationError);
            return;
        }

        QString closeError;

        if(!SessionDebtClose::closeOrdersAsDebt(fDb, orders, fStaff->fId, closeError)) {
            message_error(closeError);
            Hall().refresh();
            repaintTables();
            return;
        }

        Hall().refresh();
        repaintTables();
        openTables = Session::openTableCount(branch);

        if(openTables < 0) {
            message_error(tr("Cannot close session"));
            return;
        }

        if(openTables > 0) {
            message_error(tr("Cannot close session: %1 open table(s) remain").arg(openTables));
            return;
        }
    }

    if(!Session::close(sessionId)) {
        message_error(tr("Cannot close session"));
        return;
    }

    Session::setCurrentId(0);
    message_info(tr("Session closed"));

    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(fStaff->fId);

    if(u) {
        userName = u->fFull;
    }

    const QString printer = fHall ? fHall->fReceiptPrinter : defrest(dr_first_receipt_printer);
    ReportPrint::printSessionCloseTotal(userName, printer);

    if(!DlgSessionOpen::ensureOpen(branch, this, userName, printer)) {
        fCanClose = true;
        close();
    }
}

void RDesk::salary()
{
    CheckTime ct;
    if (!ct.check()) {
        return;
    }
    DlgSalary::salary();
}

void RDesk::visitStat()
{
    int id;
    QString name;

    if(!DlgDeptHolder::getHolder(id, name)) {
        return;
    }

    fDbBind[":f_debtHolder"] = id;
    DatabaseResult dr;
    dr.select(fDb, "select count(o.f_id) as qty, sum(o.f_total)  as amount \
              from o_header o \
              left join o_car c on c.f_order=o.f_id \
              where c.f_costumer=:f_debtHolder and o.f_state=2 ", fDbBind);
    QString msg;

    if(dr.rowCount() > 0) {
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

    if(dr.rowCount() > 0) {
        double debt = 0;

        for(int i = 0; i < dr.rowCount(); i++) {
            debt += dr.value(i, "f_debt").toDouble();
        }

        if(debt > 1 || debt < -1) {
            msg += "<br> Փոխանցում՝ " + float_str(debt, 2);
        }
    }

    message_info(msg);
}

void RDesk::checkCardAmount()
{
    QString name;
    QVariant result;

    if(!DlgList::getValue(tr("Card holder"), name, result, "select f_code, right(f_code, 4) from d_gift_cart")) {
        return;
    }

    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_code"] = result;
    db2.exec("select sum(f_amount) from d_gift_cart_use where f_code=:f_code");

    if(!db2.next()) {
        message_error(tr("Invalid card code"));
        return;
    }

    message_info(tr("Balance") + "<br>" + float_str(db2.doubleValue(0), 1));
    //    DatabaseResult dr;
    //    fDbBind[":f_card"] = result;
    //    dr.select(fDb, "select f_mode from d_car_client where f_card=:f_card", fDbBind);
    //    if (dr.rowCount() == 0) {
    //        message_error(tr("Invalid card code"));
    //        return;
    //    }
    //    QStringList l = dr.value(0, 0).toString().split(";");
    //    if (l.count() < 0) {
    //        message_error(tr("Card error"));
    //        return;
    //    }
    //    message_info(tr("Balance") + "<br>" + l.at(2));
}

void RDesk::cardStat()
{
    QString name;
    QVariant result;

    if(!DlgList::getValue(tr("Card holder"), name, result,
                          "select f_id, f_name from d_car_client where length(f_name) >0 order by 2")) {
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
    //    Db b = Preferences().getDatabase(Base::fDbName);
    //    Database2 db2;
    //    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    //    db2[":f_id"] = fTable->fOrder;
    //    db2.exec("select sum(f_amount) from g_gift_card_use where f_code=:f_id");
    //    if (!db2.next()) {
    //        message_error(tr("Not valid order id"));
    //        return;
    //    }
    msg += "<br>" + tr("Cards") + "<br>";
    msg += QString("%1 / %2 / %3 / %4<br>").arg(tr("Card"), tr("Total qty"), tr("Total amount"), tr("Current visits"));

    for(int i = 0; i < dr.rowCount(); i++) {
        msg += dr.value(i, "card").toString() + ": " + dr.value(i, "qty").toString() + "/" + float_str(dr.value(i,
               "amount").toDouble(), 2) + "/";
        msg += QString("%1<br>").arg(dr.value(i, "qty").toInt() % 11);
    }

    message_info(msg);
}

void RDesk::saledItem()
{
    QDate date;

    if(!DlgDate::getDate(date)) {
        return;
    }

    const int bs = receiptFontPt(10);
    ReceiptPrinter printer(defrest(dr_first_receipt_printer));
    C5Printing p;
    setupC5Printing(p, printer.printer(), 2.0);
    p.setFont(qApp->font());
    p.setFontSize(bs);
    p.ctext(tr("Daily sale"));
    p.br();
    p.ctext(tr("Goods"));
    p.br();
    p.ctext(date.toString("dd/MM/yyyy"));
    p.br();
    fDbBind[":f_datecash"] = date;
    fDbBind[":f_ostate"] = ORDER_STATE_CLOSED;
    fDbBind[":f_dstate"] = DISH_STATE_READY;
    fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    DatabaseResult dr;
    dr.select(fDb, "select d.f_en, od.f_store, sum(od.f_qty) as f_qty, sum(od.f_total) as f_total "
                   "from o_dish od "
                   "inner join o_header oh on oh.f_Id=od.f_header "
                   "inner join r_dish d on d.f_id=od.f_dish "
                   "where oh.f_state=:f_ostate and od.f_state=:f_dstate "
                   "and oh.f_datecash=:f_datecash and oh.f_branch=:f_branch "
                   "group by 1, 2 "
                   "order by od.f_store ", fDbBind);

    if(dr.rowCount() == 0) {
        return;
    }

    double total = 0.0;
    int store = 0;

    for(int i = 0; i < dr.rowCount(); i++) {
        if(store != dr.value(i, "f_store").toInt()) {
            if(total > 0.01) {
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

            if(dr2.rowCount() > 0) {
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

    if(total > 0.01) {
        p.br();
        p.ltext(tr("Total"), 0);
        p.rtext(float_str(total, 2));
    }

    p.br();
    p.br();
    p.setFontSize(receiptFontPt(8));
    p.ltext(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss"), 0);
    printC5(p, printer.printer());
}

void RDesk::employesOfDay()
{
    DlgSalary::salary2();
}

void RDesk::extracted(QSettings &s,
                      QMap<QString, QMap<QString, QVariant>> &fFiscalMachines,
                      const QString &g, QStringList &keys)
{
    for(const QString &k : keys) {
        fFiscalMachines[g][k] = s.value(k);
    }
}
void RDesk::fiscalCancel()
{
    int ordNum;
    if(!RNumbers::getInt(ordNum, "ՊԱՏՎԵՐԻ ՀԱՄԱՐԸ", this)) {
        return;
    }
    if(ordNum == 0) {
        return;
    }
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":rseq"] = ordNum;
    db2.exec("select * from o_tax_log where f_time>DATE_SUB(NOW(), INTERVAL 7 "
             "DAY) AND JSON_VALUE(f_out, '$.rseq')=:rseq");
    if(db2.next() == false) {
        message_error(tr("Fiscal not found"));
        return;
    }
    QString fromBase64 = QByteArray::fromBase64(db2.string("f_in").toLatin1());
    QJsonObject jin = QJsonDocument::fromJson(fromBase64.toUtf8()).object();
    QJsonObject jtax =
        QJsonDocument::fromJson(db2.string("f_out").toUtf8()).object();
    int orderId = db2.integer("f_order");
    QSettings s(QString("%1\\fiscal.ini").arg(qApp->applicationDirPath()),
                QSettings::IniFormat);
    QStringList groups = s.childGroups();
    QString fDefaultFiscalMachine;
    QMap<QString, QMap<QString, QVariant>> fFiscalMachines;

    for(const QString &g : qAsConst(groups)) {
        s.beginGroup(g);

        if(s.value("default").toBool()) {
            fDefaultFiscalMachine = g;
        }

        QStringList keys = s.childKeys();
        extracted(s, fFiscalMachines, g, keys);
        s.endGroup();
    }
    QMap<QString, QVariant> sf = fFiscalMachines[fDefaultFiscalMachine];
    PrintTaxNO pn(sf.value("ip").toString(),
                  sf.value("port").toInt(),
                  sf.value("password").toString(),
                  sf.value("extpos").toString(),
                  sf.value("opcode").toString(),
                  sf.value("oppin").toString());
    QString in, out, err;

    if(jin["eMarks"].toArray().isEmpty() == false) {
        QJsonArray emarks = jin["eMarks"].toArray();
        // message_info(tr("eMarks exists"));

        for(int i = 0; i < emarks.count(); i++) {
            //  message_info(emarks.at(i).toString());
            pn.fEmarks.append(emarks.at(i).toString());
        }
    } else {
        //  message_info(tr("No eMarks"));
    }
    int result = pn.printTaxback(jtax["rseq"].toInt(), jtax["crn"].toString(), in, out,
                                 err);
#ifdef QT_DEBUG
    out =
        "{\"rseq\":77,\"crn\":\"63219817\",\"sn\":\"V98745506068\",\"tin\":\"01588771\",\"taxpayer\":\"«Ռոգա էնդ կոպիտա ՍՊԸ»\",\"address\":\"Արշակունյանց 34\",\"time\":1676794194840,\"fiscal\":\"98198105\",\"lottery\":\"00000000\",\"prize\":0,\"total\":1540.0,\"change\":0.0}";
    out =
        "{\"address\":\"ԿԵՆՏՐՈՆ ԹԱՂԱՄԱՍ Ամիրյան 4/3 \",\"change\":0.0,\"crn\":\"53235782\",\"fiscal\":\"54704153\",\"lottery\":\"\",\"prize\":0,\"rseq\":1327,\"sn\":\"00022154380\",\"taxpayer\":\"«ՊԼԱԶԱ ՍԻՍՏԵՄՍ»\",\"time\":1709630105632,\"tin\":\"02596277\",\"total\":93600.0}";
    result = 0;
#endif
    db2[":f_order"] = orderId;
    db2[":f_in"] = QByteArray(in.toUtf8()).toBase64();
    db2[":f_out"] = out;
    db2[":f_err"] = err;
    db2.insert("o_tax_log");

    if(result == 0) {
        db2[":f_id"] = orderId;
        db2.exec("update o_dish set f_emark=null where f_header=:f_id");
        message_info(tr("Done"));
    } else {
        message_error(QJsonDocument(jin).toJson() + out);
    }
}

void RDesk::closeEvent(QCloseEvent *e)
{
    if(!fCanClose) {
        e->ignore();
        return;
    }

    if(fTable) {
        //unlock previous table
        QString query = QString("update r_table set f_lockHost='' where f_id=%1")
                        .arg(fTable->fId);
        fDb.queryDirect(query);
    }

    checkEmpty();
    CustomerDisplay::destroyInstance();
    BaseExtendedDialog::closeEvent(e);
}

void RDesk::printCanceledOrder(int id)
{
    Q_UNUSED(id);
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(fStaff->fId);

    if(u) {
        userName = u->fFull;
    }

    ui->tblOrder->viewport()->update();

    const QString printerName = defrest(dr_first_receipt_printer).isEmpty()
                                    ? QStringLiteral("local")
                                    : defrest(dr_first_receipt_printer);
    ReceiptPrinter printer(printerName);
    C5Printing doc;
    setupC5Printing(doc, printer.printer());

    doc.image("./logo_print.png", Qt::AlignHCenter);
    doc.br(4);
    doc.setFontSize(receiptFontPt(12));
    doc.setFontBold(true);
    doc.ctext(fHall->fName);
    doc.br();
    doc.ctext(tr("CANCELED"));
    doc.br();
    doc.ctext(QString("%1 %2").arg(tr("Receipt S/N ")).arg(fTable->fOrder));
    doc.br();
    doc.setFontSize(receiptFontPt(10));
    doc.setFontBold(false);
    doc.lrtext(tr("Table"), fTable->fName);
    doc.br();

    if(!fCarModel.isEmpty()) {
        doc.lrtext(tr("Car"), fCarModel + ": " + fCarGovNum);
        doc.br();
    }

    doc.lrtext(tr("Date"), WORKING_DATE.toString(def_date_format));
    doc.br();
    doc.lrtext(tr("Waiter"), userName);
    doc.br();
    doc.lrtext(tr("Opened"), fTable->fOpened.toString(def_date_time_format));
    doc.br();
    doc.lrtext(tr("Canceled"), QDateTime::currentDateTime().toString(def_date_time_format));
    doc.br();
    doc.line();
    doc.br(2);
    doc.setFontBold(true);
    doc.ltext(tr("Qty"), 0, 14);
    doc.ltext(tr("Description"), 14, 50);
    doc.ltext(tr("Amount"), 64, 0);
    doc.br();
    doc.setFontBold(false);
    doc.line();
    doc.br(2);

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od || od->fState != DISH_STATE_READY) {
            continue;
        }

        doc.ltext(float_str(od->fQty, 1), 0, 14);
        doc.ltext(od->fName, 14, 50);
        doc.rtext(float_str(od->fTotal, 2));
        doc.br();
    }

    if(ui->tblTotal->item(2, 0) && !ui->tblTotal->item(2, 0)->data(Qt::DisplayRole).toString().isEmpty()) {
        doc.ltext(ui->tblTotal->item(2, 0)->data(Qt::DisplayRole).toString(), 0, 50);
        doc.rtext(ui->tblTotal->item(2, 1)->data(Qt::DisplayRole).toString());
        doc.br();
    }

    doc.line();
    doc.br(2);
    doc.setFontBold(true);
    doc.ltext(tr("Total, AMD"), 0, 50);
    doc.rtext(ui->tblTotal->item(1, 1)->data(Qt::EditRole).toString());
    doc.br();
    doc.setFontBold(false);
    doc.br(2);

    if(!fTable->fRoomComment.isEmpty()) {
        doc.ctext(fTable->fRoomComment);
        doc.br();
        doc.ctext(tr("Signature"));
        doc.br();
        doc.line();
        doc.br();
    }

    if(fTable->fPaymentMode == PAYMENT_COMPLIMENTARY) {
        doc.ctext(tr("COMPLIMENTARY"));
        doc.br();
    }

    doc.br(2);
    doc.setFontBold(true);
    doc.ctext(tr("****VOID****"));
    doc.br();
    doc.setFontBold(false);

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od || od->fState != DISH_STATE_REMOVED_STORE) {
            continue;
        }

        doc.ltext(float_str(od->fQty, 1), 0, 14);
        doc.ltext(od->fName, 14, 50);
        doc.rtext(float_str(od->fTotal, 2));
        doc.br();
    }

    doc.br();
    doc.setFontBold(true);
    doc.ctext(tr("****MISTAKE****"));
    doc.br();
    doc.setFontBold(false);

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od || od->fState != DISH_STATE_REMOVED_NOSTORE) {
            continue;
        }

        doc.ltext(float_str(od->fQty, 1), 0, 14);
        doc.ltext(od->fName, 14, 50);
        doc.rtext(float_str(od->fTotal, 2));
        doc.br();
    }

    doc.br();
    doc.ctext("_");
    printC5(doc, printer.printer());

    fTable->fPrint = abs(fTable->fPrint) + 1;
    fDbBind[":f_print"] = fTable->fPrint;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    changeBtnState();
}

void RDesk::timeout()
{
    fTimerCounter++;
    ui->leCmd->setFocus();

    if(fTimerCounter % 3 == 0) {
        //startService();
        if(fHall) {
            Hall().refresh();
        }
    }

    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select f_version from s_app where f_app='data'");

    if(db2.next()) {
        if(fDataVersion != db2.value("f_version").toString().toInt()) {
            fDataVersion = db2.value("f_version").toString().toInt();
            Hall().refresh();
            repaintTables();
        }
    }
}

void RDesk::changeMenu()
{
    auto *b = static_cast<QPushButton*>(sender());
    const int oldMenu = fMenu;
    const int newMenu = b->property("id").toInt();

    if(fTable && fTable->fOrder > 0) {
        OrderLog::write(fTable->fOrder, OrderLog::ACTION_MENU_CHANGE,
                        QString("from=%1;to=%2;source=button").arg(oldMenu).arg(newMenu));
    }

    fMenu = newMenu;
    fNoService = b->property("noservice").toInt() == 1;
    fNeedCar = b->property("needcar").toInt() == 1;

    setBtnMenuText();
    setupType(0);
}
void RDesk::onBtnQtyClicked()
{
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();

    if(sel.count() == 0) {
        return;
    }

    QPushButton *b = static_cast<QPushButton*>(sender());
    float qty;

    if(b->text() == "-0.5") {
        qty = -0.5;
    } else if(b->text() == "+0.5") {
        qty = 0.5;
    } else {
        qty = b->text().toFloat();
    }

    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();

    if(!od->fEmark.isEmpty()) {
        message_error(tr("Cannot change quantity of dish thats contains emarks"));
        return;
    }

    if(!od) {
        return;
    }

    if(od->fDishId == fHall->fServiceItem) {
        message_error(tr("This item is not editable"));
        return;
    }

    if(od->fState != DISH_STATE_READY) {
        message_error(tr("You cannot edit the quantity of selected item"));
        return;
    }

    if(od->fQty + qty < 0.1) {
        return;
    }

        QString oldQty = QString("%1, %2 / %3").arg(od->fName)
                         .arg(od->fQty)
                         .arg(od->fQtyPrint);

        if(od->fQtyPrint < 0.01) {
            if(qty > 0.9) {
                if(qty < 10.0) {
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

        QString newQty = QString("%1, %2 / %3").arg(od->fName)
                         .arg(od->fQty)
                         .arg(od->fQtyPrint);
        countDish(od);
        fTrackControl->insert("Dish qty", oldQty, newQty);
        updateDish(od);

        resetPrintQty();
        ui->tblOrder->viewport()->update();
        countTotal();
        changeBtnState();
        repaintTables();
}
void RDesk::on_btnExit_clicked()
{
    if(message_question(tr("Confirm to close application")) != QDialog::Accepted) {
        return;
    }

    if(fTable) {
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
    if(RChangeLanguage::changeLanguage(this)) {
        ui->tblType->viewport()->update();
        ui->tblDish->viewport()->update();
        ui->tblOrder->viewport()->update();
    }
}
void RDesk::on_btnMenu_clicked()
{
    int newMenu;
    const int oldMenu = fMenu;

    if(RChangeMenu::changeMenu(fMenu, newMenu, this)) {
        if(fTable && fTable->fOrder > 0) {
            OrderLog::write(fTable->fOrder, OrderLog::ACTION_MENU_CHANGE,
                            QString("from=%1;to=%2;source=dialog").arg(oldMenu).arg(newMenu));
        }

        fMenu = newMenu;
        setBtnMenuText();
        setupType(0);
    }
}
void RDesk::setBtnMenuText()
{
    if(fMenu == 0) {
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
    ui->tblType->horizontalHeader()->setDefaultSectionSize((ui->tblType->width() - 10)  / 2);
    int row = -1, col = 3;

    for(QMap<int, TypeStruct* >::const_iterator it = type.begin(); it != type.end(); it++) {
        if(col > ui->tblType->columnCount() - 1) {
            col = 0;
            row ++;
            ui->tblType->setRowCount(row + 1);
        }

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, QVariant::fromValue(it.value()));
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

    if(dish.count() % ui->tblDish->columnCount() > 0) {
        rowCount++;
    }

    ui->tblDish->setRowCount(rowCount);
    int col = 0, row = 0;

    for(QMap<int, DishStruct* >::const_iterator it = dish.constBegin(); it != dish.constEnd(); it++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, QVariant::fromValue(*it));
        ui->tblDish->setItem(row, col++, item);

        if(col == ui->tblDish->columnCount()) {
            row++;
            col = 0;
        }
    }
}
int RDesk::addDishToOrder(DishStruct * d, bool counttotal)
{
    QString sessionError;

    if(!Session::isValidForWorkingDate(sessionError)) {
        message_error(sessionError);
        return 0;
    }

    double max = 999;
    double min = 0.25;

    if(d->fNeedEmarks) {
        max = 1;
        min = 1;
    } else {
        if(!DlgPassword::getQty(d->fName, max, min)) {
            return 0;
        }
    }

    if (fTable && (!fHall || fHall->fId != fTable->fHall)) {
        fHall = Hall::getHallById(fTable->fHall);
    }

    if(!fTable) {
        return 0;
    }

    if(!fStaff) {
        message_error(tr("Staff is not set."));
        return 0;
    }

    checkOrderHeader(fTable);

    if(!fHall) {
        fHall = Hall::getHallById(fTable->fHall);
    }

    if (fNeedCar) {
        if(fCarId == 0) {
            on_btnSetCar_clicked();

            if(fCarId == 0) {
                return 0;
            }
        }
    }

    OrderDishStruct *od = new OrderDishStruct();

    if(d->fMod.count() > 0) {
        QStringList mods;

        for(QList<QMap<QString, QString> >::const_iterator it = d->fMod.constBegin(); it != d->fMod.constEnd(); it++) {
            mods.append(it->value(def_lang));
        }

        RModifiers *m = new RModifiers(this);
        m->setModifiers(mods);

        if(m->exec() == QDialog::Accepted) {
            od->fComment = m->mod();
        }

        delete m;
    }

    if(!fHall) {
        message_error(tr("Hall configuration not found for this table."));
        delete od;
        return 0;
    }

    od->fQty = max;
    od->fDishId = d->fId;
    od->fState = DISH_STATE_READY;
    od->fPrint1 = d->fPrint1.trimmed();
    od->fPrint2 = d->fPrint2;
    const bool autoPrintKitchen = !od->fPrint1.isEmpty()
                                  && fHall
                                  && od->fDishId != fHall->fServiceItem;
    {
        int mappedStore = 0;
        if(!BranchStoreMap::lookup(d->fStore, &mappedStore)) {
            message_error(tr("Store %1 is not mapped for branch %2. "
                             "Add it in r_branch_storemap (Resort / branch store map).")
                          .arg(d->fStore)
                          .arg(defrest(dr_branch)));
            delete od;
            return 0;
        }
        od->fStore = mappedStore;
    }
    od->fName = d->fName;
    od->fPrice = d->fPrice;

    if(fNoService) {
        od->fSvcValue = 0;
        od->fSvcAmount = 0;
    } else {
        od->fSvcValue = d->fId == 487 ? 0 : d->fSvcValue;
        od->fSvcAmount = d->fId == 487 ? 0 : od->fSvcValue * od->fTotal;
    }

    if(od->fDishId == fHall->fServiceItem) {
        max = 1;
    }

    od->fDctValue = 0;
    od->fDctAmount = 0;
    od->fQty = max;
    od->fQtyPrint = od->fQty;
    od->fAdgt = d->fAdgt;
    od->fTax = d->fTax;
    od->fRow = ui->tblOrder->rowCount();
    od->fEmark = d->tempEmark;
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
    fDbBind[":f_complexId"] = 0;
    fDbBind[":f_adgt"] = od->fAdgt;
    fDbBind[":f_row"] = od->fRow;
    od->fEmark = emarkForDb(od->fEmark, od->fAdgt);
    fDbBind[":f_emark"] = od->fEmark;
    od->fRecId = fDb.insert("o_dish", fDbBind);
    d->tempEmark.clear();
    fTable->fPrint = abs(fTable->fPrint) * -1;
    updateDishQtyHistory(od);
    OrderLog::write(fTable->fOrder, OrderLog::ACTION_DISH_ADD,
                    QString("dishId=%1;name=%2;qty=%3;price=%4;recId=%5")
                    .arg(od->fDishId)
                    .arg(od->fName)
                    .arg(od->fQty)
                    .arg(od->fPrice)
                    .arg(od->fRecId));
    if(!addDishToTable(od, counttotal, true)) {
        fDb.queryDirect(QString("delete from o_dish where f_id=%1").arg(od->fRecId));
        delete od;
        message_error(tr("Failed to add dish to order."));
        return 0;
    }

    if(autoPrintKitchen) {
        QStringList availablePrinters;
        const QString systemPrinter = resolveSystemPrinterName(od->fPrint1, &availablePrinters);
        const int recId = od->fRecId;
        const QString print1 = od->fPrint1;
        if(systemPrinter.isEmpty()) {
            QTimer::singleShot(0, this, [this, print1, availablePrinters]() {
                showPrinterNotInstalledError(this, print1, availablePrinters);
                changeBtnState();
            });
        } else {
            QTimer::singleShot(0, this, [this, recId, print1]() {
                if(printServiceCheck(print1, 1, recId)) {
                    fTrackControl->insert("Printed service check", print1, "");
                }
                ui->tblOrder->viewport()->update();
                changeBtnState();
            });
        }
    }

    resetPrintQty();
    fTrackControl->insert("New dish", od->fName, "");
    return od->fRecId;
}
bool RDesk::addDishToTable(OrderDishStruct * od, bool counttotal, bool checkservice)
{
    if(!fTable || !fHall || !od) {
        return false;
    }

    int row = ui->tblOrder->rowCount();
    ui->tblOrder->setRowCount(row + 1);

    for(int i = 0; i < 3; i++) {
        ui->tblOrder->setItem(row, i, new QTableWidgetItem());
    }

    bool addService = !fNoService && od->fSvcValue > 0.01;
    bool serviceItemExists = false;

    for(int i = 0; i < ui->tblOrder->rowCount() - 1; i++) {
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *odd = cell->data(Qt::UserRole).value<OrderDishStruct*>();

        if(odd) {
            if(odd->fDishId == fHall->fServiceItem && odd->fState == DISH_STATE_READY) {
                addService = false;
                serviceItemExists = true;
                break;
            }
        }
    }

    if(addService && checkservice) {
        //            bool found = false;
        //            for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        //                OrderDishStruct *os = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        //                if (os->fDishId == fHall->fServiceItem) {
        //                    found = true;
        //                    break;
        //                }
        //            }
        //            if (!found) {
        OrderDishStruct *so = new OrderDishStruct();
        so->fDishId = fHall->fServiceItem;
        so->fState = DISH_STATE_READY;
        so->fPrint1 = "";
        so->fPrint2 = "";
        {
            int serviceStore = 0;
            if(!BranchStoreMap::lookup(3, &serviceStore)) {
                message_error(tr("Store 3 is not mapped for branch %1 (required for service charge line). "
                                 "Configure r_branch_storemap.")
                              .arg(defrest(dr_branch)));
                return false;
            }
            so->fStore = serviceStore;
        }
        so->fName = fHall->fServiceName;
        so->fPrice = 0;
        so->fSvcValue = od->fSvcValue;
        so->fSvcAmount = 0;
        so->fDctValue = 0;
        so->fDctAmount = 0;
        so->fQty = 1;
        so->fQtyPrint = 1;
        so->fAdgt = od->fAdgt;
        so->fTax = od->fTax;
        so->fRow = 125;
        fDbBind[":f_header"] = fTable->fOrder;
        fDbBind[":f_state"] = DISH_STATE_READY;
        fDbBind[":f_dish"] = so->fDishId;
        fDbBind[":f_qty"] = so->fQty;
        fDbBind[":f_qtyPrint"] = so->fQtyPrint;
        fDbBind[":f_price"] = so->fPrice;
        fDbBind[":f_svcValue"] = so->fSvcValue;
        fDbBind[":f_svcAmount"] = so->fSvcAmount;
        fDbBind[":f_dctValue"] = so->fDctValue;
        fDbBind[":f_dctAmount"] = so->fDctAmount;
        fDbBind[":f_total"] = so->fTotal;
        fDbBind[":f_totalUSD"] = so->fTotal;
        fDbBind[":f_print1"] = so->fPrint1;
        fDbBind[":f_print2"] = so->fPrint2;
        fDbBind[":f_store"] = so->fStore;
        fDbBind[":f_comment"] = so->fComment;
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_complexId"] = 0;
        fDbBind[":f_adgt"] = so->fAdgt;
        fDbBind[":f_row"] = so->fRow;
        so->fRecId = fDb.insert("o_dish", fDbBind);
        updateDishQtyHistory(so);
        OrderLog::write(fTable->fOrder, OrderLog::ACTION_DISH_ADD,
                        QString("dishId=%1;name=%2;qty=%3;price=%4;recId=%5;service=1")
                        .arg(so->fDishId)
                        .arg(so->fName)
                        .arg(so->fQty)
                        .arg(so->fPrice)
                        .arg(so->fRecId));
        int rows = row + 1;
        ui->tblOrder->setRowCount(rows + 1);

        for(int i = 0; i < 3; i++) {
            ui->tblOrder->setItem(rows, i, new QTableWidgetItem());
        }

        QTableWidgetItem *serviceCells[3] = {
            ui->tblOrder->item(rows, 0),
            ui->tblOrder->item(rows, 1),
            ui->tblOrder->item(rows, 2)
        };
        if(!serviceCells[0] || !serviceCells[1] || !serviceCells[2]) {
            return false;
        }
        serviceCells[0]->setData(Qt::UserRole, QVariant::fromValue(so));
        serviceCells[1]->setData(Qt::UserRole, QVariant::fromValue(so));
        serviceCells[2]->setData(Qt::UserRole, QVariant::fromValue(so));
        //}
    }

    QTableWidgetItem *dishCells[3] = {
        ui->tblOrder->item(row, 0),
        ui->tblOrder->item(row, 1),
        ui->tblOrder->item(row, 2)
    };
    if(!dishCells[0] || !dishCells[1] || !dishCells[2]) {
        return false;
    }
    dishCells[0]->setData(Qt::UserRole, QVariant::fromValue(od));
    dishCells[1]->setData(Qt::UserRole, QVariant::fromValue(od));
    dishCells[2]->setData(Qt::UserRole, QVariant::fromValue(od));
    ui->tblOrder->setCurrentCell(row, 0);
    setOrderRowHidden(row, od);

    if(counttotal) {
        countTotal();
        changeBtnState();
    }
    return true;
}
void RDesk::updateDish(OrderDishStruct * od)
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
    od->fEmark = emarkForDb(od->fEmark, od->fAdgt);
    fDbBind[":f_emark"] = od->fEmark;
    fDb.update("o_dish", fDbBind, QString("where f_id=%1").arg(od->fRecId));
    updateDishQtyHistory(od);
}
double RDesk::countTotal()
{
    if (!fTable || !fHall) {
        return 0;
    }

    double total = 0;
    double servicevalue = 0;

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *od = cell->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od) {
            continue;
        }

        if(od->fState != DISH_STATE_READY) {
            continue;
        }

        if(fHall->fServiceItem != od->fDishId) {
            if (!fNoService) {
                servicevalue += od->fTotal * od->fSvcValue;
            }
            total += od->fTotal;
        }

        //        if (od->fDishId == fHall->fServiceItem) {
        //            od->fPrice = servicevalue;
        //            od->fTotal = servicevalue;
        //            ui->tblOrder->item(i, 0)->setData(Qt::UserRole, QVariant::fromValue(od));
        //            ui->tblOrder->item(i, 1)->setData(Qt::UserRole, QVariant::fromValue(od));
        //            ui->tblOrder->item(i, 2)->setData(Qt::UserRole, QVariant::fromValue(od));
        //            updateDish(od);
        //            continue;
        //        }
    }

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *od = cell->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od) {
            continue;
        }

        if(od->fDishId != fHall->fServiceItem) {
            continue;
        }

        if (fNoService) {
            if (od->fPrice > 0.001 || od->fTotal > 0.001) {
                od->fPrice = 0;
                od->fTotal = 0;
                od->fQty = 1;
                updateDish(od);
            }
            continue;
        }

        od->fPrice = servicevalue;
        od->fTotal = servicevalue;
        od->fQty = 1;
        updateDish(od);
    }

    double grandTotal = total + (fNoService ? 0 : servicevalue);
    fHall = Hall::getHallById(fTable->fHall);
    if(QTableWidgetItem *totalItem = ui->tblTotal->item(1, 1)) {
        totalItem->setData(Qt::EditRole, float_str(grandTotal, 2));
    }
    fDbBind[":f_total"] = grandTotal;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fDbBind[":f_cash"] = grandTotal;
    fDbBind[":f_card"] = 0;
    fDbBind[":f_debt"] = 0;
    fDbBind[":f_coupon"] = 0;
    fDb.update("o_header_payment", fDbBind, where_id(ap(fTable->fOrder)));
    fTable->fAmount = float_str(grandTotal, 2);
    updateTableInfo();
    refreshCustomerDisplay();
    return grandTotal;
}
void RDesk::countDish(OrderDishStruct * d)
{
    d->fTotal = d->fQty * d->fPrice;
    if (fNoService) {
        d->fSvcValue = 0;
        d->fSvcAmount = 0;
    } else {
        d->fSvcAmount = d->fTotal * d->fSvcValue;
    }
    d->fDctAmount = d->fTotal * d->fDctValue;
}
bool RDesk::setTable(TableStruct * t, bool nosmile)
{
    if(fTable) {
        if(t == fTable && !nosmile) {
            DlgSmile *ds = new DlgSmile(this);
            ds->exec();
            delete ds;
            return true;
        }
    }

    //try lock new table
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    if(t) {
        db2[":f_id"] = t->fId;
        db2.exec("select f_lockhost from r_table where f_id=:f_id");
        db2.next();

        if(db2.string("f_lockhost").isEmpty()) {
            db2[":f_id"] = t->fId;
            db2[":f_lockhost"] = HOSTNAME;
            db2.exec("update r_table set f_lockhost=:f_lockhost where f_id=:f_id");
        } else {
            if(db2.string("f_lockhost") != HOSTNAME) {
                if(!nosmile) {
                    message_error(tr("Table locked by other user"));
                }

                return false;
            }
        }

        db2[":f_id"] = t->fId;
        db2[":f_lockhost"] = HOSTNAME;
        db2.exec("update r_table set f_lockHost='' where f_lockhost=:f_lockhost and f_id <>:f_id");
    }

    if(fTable) {
        //unlock previous table
        checkEmpty();
    }

    clearOrder();

    if(t == 0) {
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
    fDb.select("select  f_order from r_table where f_id=:f_id ", fDbBind, fDbRows);

    if(fTable) {
        fTable->fOrder = fDbRows.at(0).at(0).toInt();

        if(fTable->fOrder > 0) {
            loadOrder(!nosmile);
        }

        fHall = Hall::getHallById(fTable->fHall);
        ui->lbCar->setText(fCarModel + " " + fCarGovNum);
        s.close();

        if(fTable->fHall == 3 || fTable->fHall == 11) {
            OrderDishStruct *od = nullptr;

            for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
                QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
                if(!cell) {
                    continue;
                }
                od = cell->data(Qt::UserRole).value<OrderDishStruct*>();

                if(!od) {
                    continue;
                }

                if(od->fDishId == 487) {
                    break;
                }
            }

            if(od == nullptr) {
                if(message_question(tr("Open new VIP Table?")) == QDialog::Accepted) {
                    DishStruct *d = nullptr;

                    for(int i = 0; i < fDishTable.fDish.count(); i++) {
                        if(fDishTable.fDish.at(i)->fId == 487) {
                            d = fDishTable.fDish.at(i);
                        }
                    }

                    if(!d) {
                        setTable(nullptr, false);
                        return false;
                    }

                    addDishToOrder(d, false);
                } else {
                    setTable(nullptr, false);
                    return false;
                }
            }
        }
    }

    countTotal();
    changeBtnState();
    return true;
}
void RDesk::checkOrderHeader(TableStruct * t)
{
    if(!t) {
        return;
    }

    if(!fHall || fHall->fId != t->fHall) {
        fHall = Hall::getHallById(t->fHall);
    }

    if(t->fOrder == 0) {
        QString sessionError;

        if(!Session::isValidForWorkingDate(sessionError)) {
            message_error(sessionError);
            return;
        }

        if(!fHall) {
            message_error(tr("Hall configuration not found for this table."));
            return;
        }

        fDb.fDb.transaction();
        QString query = QString("select f_id from r_table where f_id='%1' for update")
                        .arg(t->fId);
        fDb.queryDirect(query);
        fDbBind[":f_state"] = ORDER_STATE_OPENED;
        fDbBind[":f_branch"] = defrest(dr_branch).toInt();
        fDbBind[":f_table"] = t->fId;
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_dateOpen"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        fDbBind[":f_dateCash"] = WORKING_DATE;
        fDbBind[":f_tax"] = 0;
        fDbBind[":f_paymentMode"] = PAYMENT_CASH;
        fDbBind[":f_hall"] = t->fHall;
        fDbBind[":f_servicevalue"] = fNoService ? 0 : fHall->fServiceValue;
        t->fOpened = fDbBind[":f_dateOpen"].toDateTime();
        t->fOrder = fDb.insert("o_header", fDbBind);
        fDbBind[":f_order"] = t->fOrder;
        fDb.update("r_table", fDbBind, QString("where f_id=%1").arg(t->fId));
        fDb.fDb.commit();
        OrderLog::write(t->fOrder, OrderLog::ACTION_ORDER_OPEN,
                        QString("table=%1;hall=%2;staff=%3;opened=%4")
                        .arg(t->fId)
                        .arg(t->fHall)
                        .arg(fStaff->fId)
                        .arg(t->fOpened.toString("yyyy-MM-dd HH:mm:ss")));
        ui->tblTables->viewport()->update();
    }
}
void RDesk::clearOrder()
{
    if(!fTable) {
        return;
    }

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        delete ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
    }

    ui->tblOrder->clear();
    ui->tblOrder->setRowCount(0);
    fTable->fPrint = 0;
    fTable->fOrder = 0;

    ui->lbCar->clear();
    fCarModel = "";
    fCarGovNum = "";
    fCostumerId = 0;
    fCarId = 0;
    ui->tblTotal->item(1, 1)->setData(Qt::EditRole, "0");

    if(ui->tblTotal->item(2, 1)) {
        ui->tblTotal->item(2, 1)->setData(Qt::EditRole, "0");
    }

    fTable = nullptr;
    refreshCustomerDisplay();
}
void RDesk::refreshCustomerDisplay()
{
    CustomerDisplay *cd = CustomerDisplay::instance();
    if(!cd) {
        return;
    }

    if(!fTable || fTable->fOrder <= 0) {
        cd->showWelcome();
        return;
    }

    CustomerDisplayOrder order;

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        if(ui->tblOrder->isRowHidden(i)) {
            continue;
        }
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *od = cell->data(Qt::UserRole).value<OrderDishStruct *>();
        if(!od || od->fState != DISH_STATE_READY) {
            continue;
        }

        CustomerDisplayLine line;
        line.name = od->fName;
        line.qty = float_str(od->fQty, 1);
        line.total = float_str(od->fTotal, 2);
        line.comment = od->fComment.trimmed();
        order.lines.append(line);
    }

    const QTableWidgetItem *totalItem = ui->tblTotal->item(1, 1);
    order.grandTotal = totalItem ? totalItem->data(Qt::EditRole).toString().trimmed() : QStringLiteral("0");
    if(order.grandTotal.isEmpty()) {
        order.grandTotal = QStringLiteral("0");
    }

    cd->showOrder(order);
}
void RDesk::loadOrder(bool showwarning)
{
    QElapsedTimer et;
    et.start();
    //ui->tblOrder->clear();
    ui->tblOrder->setRowCount(0);
    User u;
    QString query = QString("select h.f_staff, h.f_comment, concat(u.f_firstName, ' ', u.f_lastName), h.f_print, "
                            "h.f_paymentMode, h.f_paymentModeComment, h.f_cityLedger, h.f_roomComment, h.f_dateOpen, "
                            "h.f_tax, h.f_recoverfrom "
                            "from o_header h "
                            "left join users u on u.f_id=h.f_staff "
                            "where h.f_id=%1")
                    .arg(fTable->fOrder);
    fDb.select(query, fDbBind, fDbRows);

    if(fDbRows.count() == 0) {
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
    et.restart();
    et.restart();
    query = "select od.f_id, od.f_dish, d.f_en, od.f_qty, od.f_qtyPrint, od.f_price, "
            "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
            "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
            "od.f_adgt, od.f_complexRec, od.f_emark "
            "from o_dish od "
            "left join r_dish d on d.f_id=od.f_dish "
            "where od.f_header=:f_header  and f_state=1 "
            "order by od.f_row ";
    fDbBind[":f_header"] = fTable->fOrder;
    QList<QList<QVariant> > dbr;
    fDb.select(query, fDbBind, dbr);

    for(QList<QList<QVariant> >::const_iterator it = dbr.constBegin(); it != dbr.constEnd(); it++) {
        OrderDishStruct *d = new OrderDishStruct();
        int c = 0;
        d->fRecId = it->at(c++).toInt();
        d->fDishId = it->at(c++).toInt();
        d->fName = it->at(c++).toString();
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
        c++; // od.f_complex
        c++; // od.f_complexId
        d->fAdgt = it->at(c++).toString();
        c++; // od.f_complexRec
        d->fEmark = it->at(c++).toString();

        countDish(d);
        addDishToTable(d, false, false);
        setOrderRowHidden(ui->tblOrder->rowCount() - 1, d);
    }

    et.restart();
    changeBtnState();
    countTotal();
    DatabaseResult dr;
    fDbBind[":f_order"] = fTable->fOrder;
    dr.select(fDb, "select f_model, f_govNumber,f_costumer from o_car where f_order=:f_order", fDbBind);

    if(dr.rowCount() > 0) {
        fCarId = dr.value("f_model").toInt();
        CI_Car *car = CacheCar::instance()->get(dr.value("f_model").toString());

        if(car) {
            fCarModel = car->fName;
        }

        fCarGovNum = dr.value("f_govNumber").toString();
        fCostumerId = dr.value("f_costumer").toInt();
    }

    ui->lbCar->setText(fCarModel + " " + fCarGovNum);
}
void RDesk::setOrderRowHidden(int row, OrderDishStruct * od)
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
bool RDesk::printServiceCheck(const QString & prn, int side, int onlyRecId)
{
    if(!fTable || !fHall || !fStaff) {
        message_error(tr("Cannot print service check: order is not open."));
        return false;
    }

    QStringList availablePrinters;
    const QString systemPrinter = resolveSystemPrinterName(prn, &availablePrinters);

    if(systemPrinter.isEmpty()) {
        QTimer::singleShot(0, this, [this, prn, availablePrinters]() {
            showPrinterNotInstalledError(this, prn, availablePrinters);
        });
        return false;
    }

    if(QPrinterInfo::printerInfo(systemPrinter).isNull()) {
        showPrinterNotInstalledError(this, prn, availablePrinters);
        return false;
    }

    ReceiptPrinter printer(systemPrinter);
    C5Printing doc;
    setupC5Printing(doc, printer.printer());

    doc.setFontSize(receiptFontPt(12));
    doc.setFontBold(true);
    doc.ctext(fHall->fName);
    doc.br();

    doc.setFontSize(receiptFontPt(10));
    doc.setFontBold(false);
    doc.ctext(QString("%1 %2").arg(tr("Service check, order #")).arg(fTable->fOrder));
    doc.br();
    doc.lrtext(tr("Table"), fTable->fName);
    doc.br();
    doc.lrtext(tr("Time"), QDateTime::currentDateTime().toString(def_date_time_format));
    doc.br();
    doc.lrtext(tr("Waiter"), fStaff->fName);
    doc.br();
    doc.line();
    doc.br(2);
    doc.setFontBold(true);
    doc.ltext(tr("Qty"), 0, 14);
    doc.ltext(tr("Description"), 14, 0);
    doc.br();
    doc.setFontBold(false);
    doc.line();
    doc.br(2);

    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *od = cell->data(Qt::UserRole).value<OrderDishStruct*>();

        if(!od || od->fState != DISH_STATE_READY) {
            continue;
        }

        if(side == 1 && od->fPrint1.compare(prn, Qt::CaseInsensitive) != 0) {
            continue;
        }
        if(side == 2 && od->fPrint2.compare(prn, Qt::CaseInsensitive) != 0) {
            continue;
        }
        if(onlyRecId > 0 && od->fRecId != onlyRecId) {
            continue;
        }

        const float qty = (onlyRecId > 0 && od->fRecId == onlyRecId)
                              ? od->fQty
                              : (od->fQty - od->fQtyPrint);

        if(qty < 0.1f) {
            continue;
        }

        doc.ltext(float_str(qty, 1), 0, 14);
        doc.ltext(od->fName, 14, 0);
        doc.br();

        if(!od->fComment.isEmpty()) {
            doc.setFontSize(receiptFontPt(9));
            doc.setFontBold(true);
            doc.ltext(od->fComment, 14, 0);
            doc.br();
            doc.setFontSize(receiptFontPt(10));
            doc.setFontBold(false);
        }

        doc.line();
    }

    doc.br(4);
    doc.ltext(tr("Printer: ") + systemPrinter, 0);
    doc.br();
    doc.ctext("_");

    if(!printC5(doc, printer.printer())) {
        message_error(tr("Failed to start printing on printer \"%1\".").arg(systemPrinter));
        return false;
    }

    return true;
}

void RDesk::markKitchenPrinted(const QString &prn, int side, int onlyRecId)
{
    for(int i = 0; i < ui->tblOrder->rowCount(); i++) {
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *od = cell->data(Qt::UserRole).value<OrderDishStruct *>();
        if(!od || od->fState != DISH_STATE_READY) {
            continue;
        }
        if(onlyRecId > 0 && od->fRecId != onlyRecId) {
            continue;
        }
        if(side == 1) {
            if(od->fPrint1.isEmpty() || od->fPrint1.compare(prn, Qt::CaseInsensitive) != 0) {
                continue;
            }
        } else if(side == 2) {
            if(od->fPrint2.isEmpty() || od->fPrint2.compare(prn, Qt::CaseInsensitive) != 0) {
                continue;
            }
        }
        if(od->fQty - od->fQtyPrint < 0.1f) {
            continue;
        }
        od->fQtyPrint = od->fQty;
        updateDish(od);
    }
}

void RDesk::printReceipt(bool printModePayment)
{
    LogWriter::write(LogWriterLevel::verbose, "open database", "start receipt printing");

    // --- 1. Проверка прав ---
    if (!printModePayment && fTable->fPrint > 0) {
        if (!check_permission(pr_hall_manager)) {
            message_error(tr("Access denied"));
            return;
        }
    }

    // --- 2. Подготовка данных пользователя ---
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(fStaff->fId);
    if (u)
        userName = u->fFull;

    const int bs = receiptFontPt(10);
    ReceiptPrinter printer(defrest(dr_first_receipt_printer));
    C5Printing p;
    setupC5Printing(p, printer.printer());

    // --- 4. Шапка (Лого и Заголовок) ---
    p.image("logo_print.png", Qt::AlignHCenter);
    p.br();

    p.setFont(QFont("Arial LatArm Unicode", bs));
    p.ctext(fHall->fName);
    p.br();

    p.setFontSize(bs);
    p.ctext(QString("%1 %2").arg(tr("Receipt S/N")).arg(fTable->fOrder));
    p.br();

    // --- 5. Работа с БД и Фискальные данные ---
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    db2[":f_id"] = fTable->fOrder;
    db2.exec("select f_tax from o_header where f_id=:f_id");
    if (!db2.next()) {
        message_error(tr("Not valid order id"));
        return;
    }

    int fiscalnumber = db2.integer("f_tax");
    if (fiscalnumber > 0) {
        db2[":f_fiscal"] = fiscalnumber;
        db2.exec("select f_in, f_out from o_tax_log where f_fiscal=:f_fiscal");
        if (db2.next()) {
            QJsonObject jIn = QJsonDocument::fromJson(db2.string("f_in").toUtf8()).object();
            QJsonObject jf = QJsonDocument::fromJson(db2.string("f_out").toUtf8()).object();

            p.setFontSize(bs);
            p.setFontBold(false);
            p.ltext(jf["taxpayer"].toString(), 0);
            p.br();
            p.lrtext(tr("Taxpayer id"), jf["tin"].toString());
            p.br();
            p.lrtext(tr("Device number"), jf["crn"].toString());
            p.br();
            p.lrtext(tr("Serial"), jf["sn"].toString());
            p.br();
            p.lrtext(tr("Fiscal"), jf["fiscal"].toString());
            p.br();
            p.lrtext(tr("Receipt number"), QString::number(jf["rseq"].toInt()));
            p.br();
            p.lrtext(tr("Date"), QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
            p.br();
            p.ctext(tr("(F)"));
            p.br();

            QString partnerTin = jIn["partnerTin"].toString();
            if (!partnerTin.isEmpty()) {
                p.lrtext(tr("Partner tin"), partnerTin);
                p.br();
            }
        }
    }

    // --- 6. Инфо о заказе (Стол, Машина, Официант) ---
    p.line(1);
    p.br(2);
    p.setFontSize(bs);
    p.lrtext(tr("Table"), fTable->fName);
    p.br();

    if (!fCarModel.isEmpty()) {
        p.lrtext(tr("Car"), fCarModel + ": " + fCarGovNum);
        p.br();
    }

    p.lrtext(tr("Date"), WORKING_DATE.toString(def_date_format));
    p.br();
    p.lrtext(tr("Waiter"), userName);
    p.br();

    if (printModePayment) {
        p.lrtext(tr("Opened"), fTable->fOpened.toString(def_date_time_format));
        p.br();
        p.lrtext(tr("Closed"), QDateTime::currentDateTime().toString(def_date_time_format));
        p.br();
    }

    // --- 7. Таблица товаров ---
    p.line(2);
    p.br(2);
    p.setFontBold(true);
    p.ltext(tr("Description"), 0);
    p.rtext(tr("Amount"));
    p.br();
    p.line(1);
    p.br(2);

    // Обычные блюда
    p.setFontSize(receiptFontPt(8));
    p.setFontBold(false);
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct *>();
        if (!od || od->fState != DISH_STATE_READY)
            continue;

        p.ltext(QString("%1 x %2").arg(float_str(od->fQty, 1)).arg(od->fName), 0, 50);
        p.rtext(float_str(od->fTotal, 2));
        p.br();
    }

    // --- 8. Итоги ---
    p.line(2);
    p.br(2);
    p.setFontSize(bs);
    p.setFontBold(true);
    p.lrtext(tr("Total, AMD"), ui->tblTotal->item(1, 1)->data(Qt::EditRole).toString());
    p.br();

    // --- 9. Платежи (только если PrintModePayment) ---
    if (printModePayment) {
        p.setFontSize(bs);
        p.setFontBold(true);
        DatabaseResult dr;
        fDbBind[":f_id"] = fTable->fOrder;
        dr.select(fDb, "select * from o_header_payment where f_id=:f_id", fDbBind);

        if (dr.rowCount() > 0) {
            auto addPayment = [&](const QString &label, const QString &field) {
                double val = dr.value(field).toDouble();
                if (val > 0.01) {
                    p.lrtext(label, float_str(val, 2));
                    p.br();
                }
            };

            addPayment(tr("Cash"), "f_cash");
            addPayment(tr("Card"), "f_card");
            addPayment("Idram", "f_idram");

            if (dr.value("f_debt").toDouble() > 0.1) {
                p.lrtext(tr("Debt"), float_str(dr.value("f_debt").toDouble(), 2));
                p.br();
                p.ctext(tr("Signature: _________________"));
                p.br();
            }
        }
    }

    // --- 10. Удаленные (VOID) ---
    if (fShowRemoved) {
        p.br(5);
        p.setFontBold(true);
        p.ctext("**** VOID ****");
        p.br();
        // ... тут логика аналогична циклу блюд, только по DISH_STATE_REMOVED_STORE
    }

    // --- 11. Финализация и Печать ---

    printC5(p, printer.printer());

    // Обновление счетчика печати
    fTable->fPrint = abs(fTable->fPrint) + 1;
    fDbBind[":f_print"] = fTable->fPrint;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));

    changeBtnState();
    fTrackControl->insert("Print receipt", fTable->fPaymentComment, "");
}

void RDesk::changeBtnState()
{
    bool emptyReceipt = true;
    bool btnPrintService = false;
    bool btnPrintReceipt = false;

    if(!fTable) {
        ui->btnPayment_2->setEnabled(false);
        ui->btnPrint->setEnabled(false);
        ui->btnPayment->setEnabled(false);
        return;
    }

    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        QTableWidgetItem *cell = ui->tblOrder->item(i, 0);
        if(!cell) {
            continue;
        }
        OrderDishStruct *od = cell->data(Qt::UserRole).value<OrderDishStruct *>();

        if (!od) {
            continue;
        }

        if (od->fState != DISH_STATE_READY) {
            continue;
        }

        emptyReceipt = false;

        const float qty = od->fQty - od->fQtyPrint;
        const bool needsKitchenPrint = !od->fPrint1.isEmpty() || !od->fPrint2.isEmpty();
        if (needsKitchenPrint && qty > 0.1f) {
            btnPrintService = true;
            break;
        }
    }

    btnPrintReceipt = !btnPrintService;
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

    int orderid = 0;
    bool orderEmpty = true;
    fDbBind[":f_table"] = fTable->fId;
    fDb.select("select f_id from o_header where f_table=:f_table and f_state=1", fDbBind, fDbRows);

    if (fDbRows.count() > 0) {
        orderid = fDbRows[0][0].toInt();
        fDbBind[":f_header"] = orderid;
        fDb.select("select f_id from o_dish where f_state=1 and f_header=:f_header", fDbBind, fDbRows);

        if (fDbRows.count() > 0) {
            orderEmpty = false;
        }
    }

    if (orderEmpty) {
        if (orderid > 0) {
            fDbBind[":f_state"] = ORDER_STATE_EMPTY;
            fDbBind[":f_dateClose"] = QDateTime::currentDateTime();
            fDb.update("o_header", fDbBind, where_id(orderid));
        }

        fDbBind[":f_order"] = 0;
        fDb.update("r_table", fDbBind, where_id(fTable->fId));
    } else {
        //        fDbBind[":f_id"] = fTable->fId;
        //        fDbBind[":f_order"] = orderid;
        //        fDb.select("update r_table set f_order=:f_order where f_id=:f_id");
    }

    fDb.select("select f_id, f_table from o_header where f_state=1 order by f_id", fDbBind, fDbRows);
    QMap<int, int> orders;

    for (int i = 0; i < fDbRows.count(); i++) {
        if (orders.contains(fDbRows.at(i).at(1).toInt())) {
            continue;
        }

        orders[fDbRows.at(i).at(1).toInt()] = fDbRows.at(i).at(0).toInt();
    }

    for (QMap<int, int>::const_iterator it = orders.constBegin(); it != orders.constEnd(); it++) {
        fDbBind[":f_order"] = it.value();
        fDbBind[":f_id"] = it.key();
        fDb.select("update r_table set f_order=:f_order where f_id=:f_id", fDbBind, fDbRows);
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

    if (!message_question(QString("%1 %2").arg(tr("Confirm to discount")).arg(val * 100))) {
        return;
    }

    double totalDisc = 0;

    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct *>();

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
TableStruct *RDesk::loadHall(int hall)
{
    fCurrentHall = hall;
    ui->tblTables->setRowCount(3);
    int row = 0, col = 0;

    for (int i = 0; i < Hall::fTables.count(); i++) {
        TableStruct *t = Hall::fTables.at(i);

        if (t->fHall != hall) {
            continue;
        }

        ui->tblTables->item(row, col)->setText(t->fName);
        ui->tblTables->item(row, col)->setData(Qt::UserRole, QVariant::fromValue(t));
        col++;

        if (col > 2) {
            col = 0;
            row++;

            if (row >= ui->tblTables->rowCount()) {
                ui->tblTables->setRowCount(row + 1);

                for (int c = 0; c < ui->tblTables->columnCount(); c++) {
                    ui->tblTables->setItem(row, c, new QTableWidgetItem());
                }
            }
        }
    }

    HallStruct *hs = Hall::getHallById(hall);

    if (hs == nullptr) {
        message_error(tr("Hall id %1 is not configured for this branch (check «Show hall» and branch in hall settings).")
                      .arg(hall));
        return nullptr;
    }

    fHall = hs;
    setupType(0);
    TableStruct *ts = nullptr; //ui->tblTables->item(0, 0)->data(Qt::UserRole).value<TableStruct*>();
    setTable(ts, false);
    ui->tblTables->viewport()->update();
    return ts;
}
void RDesk::on_tblPart_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    DishPartStruct *p = index.data(Qt::UserRole).value<DishPartStruct *>();

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

    TypeStruct *t = index.data(Qt::UserRole).value<TypeStruct *>();

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

    DishStruct *d = index.data(Qt::UserRole).value<DishStruct *>();

    if (!d) {
        return;
    }

    if (ui->btnCalculationMode->isChecked()) {
        DlgCalc(d->fId, this).exec();
        return;
    }

    if (d->fNeedEmarks > 0) {
        message_error(tr("Only using QR code"));
        return;
    }

    if (!d) {
        return;
    }

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
    removeRow(sel.at(0).row(), true);
}
void RDesk::on_btnPayment_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    CheckTime ct;
    if (!ct.check()) {
        return;
    }

    if (!DlgPayment::payment(fTable->fOrder, fTable->fHall)) {
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
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct *>();

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
            if(printServiceCheck(*prn, 1)) {
                markKitchenPrinted(*prn, 1);
                printed = true;
            }
        }
    }

    if (prn2.count() > 0) {
        for (QSet<QString>::const_iterator prn = prn2.begin(); prn != prn2.end(); prn++) {
            if(printServiceCheck(*prn, 2)) {
                markKitchenPrinted(*prn, 2);
                printed = true;
            }
        }
    }

    ui->tblOrder->viewport()->update();

    if (printed) {
        fTrackControl->insert("Printed service check", "", "");
    }

    changeBtnState();
    repaintTables();
}
void RDesk::on_btnComment_clicked()
{
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();

    if (sel.count() == 0) {
        return;
    }

    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct *>();

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

void RDesk::repaintTables()
{
    for (int c = 0; c < ui->tblTables->columnCount(); c++) {
        for (int r = 0; r < ui->tblTables->rowCount(); r++) {
            TableStruct *t = ui->tblTables->item(r, c)->data(Qt::UserRole).value<TableStruct *>();

            if (!t) {
                continue;
            }

            t->fOrder = 0;
            t->fAmount = "0";
        }
    }

    fDbBind[":f_state"] = ORDER_STATE_OPENED;
    QString query = "select t.f_id, t.f_lockHost, t.f_order, "
                    "h.f_dateOpen, h.f_comment, u.f_firstName, "
                    "h.f_total "
                    "from r_table t "
                    "left join o_header h on t.f_order=h.f_id "
                    "left join users u on u.f_id=h.f_staff "
                    "where h.f_state=:f_state and t.f_hall="
                    + QString::number(fCurrentHall)
                    + " "
                      "group by 1 ";
    fDb.select(query, fDbBind, fDbRows);

    for (QList<QList<QVariant>>::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        for (int c = 0; c < ui->tblTables->columnCount(); c++) {
            for (int r = 0; r < ui->tblTables->rowCount(); r++) {
                TableStruct *t = ui->tblTables->item(r, c)->data(Qt::UserRole).value<TableStruct *>();

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

    TableStruct *t = item->data(Qt::UserRole).value<TableStruct *>();

    if (!t) {
        return;
    }

    setTable(t, false);
    repaintTables();
}
void RDesk::on_btnPayment_2_clicked()
{
    if (!fTable) {
        message_info("Սեղանը նշված չէ");
        return;
    }

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
    fDb.select("update o_car set f_model=:f_model, f_govNumber=:f_govNumber, f_costumer=:f_costumer where f_order=:f_order",
               fDbBind,
               fDbRows);
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

    Database2 db2;
    Db b = Preferences().getDatabase(Base::fDbName);
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_govnumber"] = govNum;
    db2[":f_order"] = fTable->fOrder;
    db2.exec("select * from o_car "
             "where f_order in (select f_id from o_header where f_datecash=current_date() and f_order<>:f_order) "
             "and f_govnumber=:f_govnumber ");

    if (db2.next()) {
        message_info(QString("Այսօր մեքենան գրանցվել է %1 անգամ").arg(db2.rowCount()));
    }
}
void RDesk::on_btnDiscount_clicked()
{
    message_error(tr("No discount mechanism"));
}

void RDesk::on_btnHallWash_clicked()
{
    loadHall(1 + ((defrest(dr_branch).toInt() - 1) * 4));
}
void RDesk::on_btnHallCafe_clicked()
{
    loadHall(2 + ((defrest(dr_branch).toInt() - 1) * 4));
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
    loadHall(3 + ((defrest(dr_branch).toInt() - 1) * 4));
}
void RDesk::on_btnShop_clicked()
{
    loadHall(4 + ((defrest(dr_branch).toInt() - 1) * 4));
}
void RDesk::startService()
{
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;
    LPCWSTR szSvcName = L"Breeze";
    // Get a handle to the SCM database.
    SC_HANDLE schSCManager = OpenSCManager(NULL,                   // local computer
                                           NULL,                   // servicesActive database
                                           SC_MANAGER_ALL_ACCESS); // full access rights

    if (NULL == schSCManager) {
        qDebug() << "OpenSCManager failed (%d)\n" << GetLastError();
        return;
    }

    // Get a handle to the service.
    SC_HANDLE schService = OpenService(schSCManager,        // SCM database
                                       szSvcName,           // name of service
                                       SERVICE_ALL_ACCESS); // full access

    if (schService == NULL) {
        qDebug() << "OpenService failed (%d)\n" << GetLastError();
        CloseServiceHandle(schSCManager);
        return;
    }

    // Check the status in case the service is not stopped.
    if (!QueryServiceStatusEx(schService,                     // handle to service
                              SC_STATUS_PROCESS_INFO,         // information level
                              (LPBYTE) &ssStatus,             // address of structure
                              sizeof(SERVICE_STATUS_PROCESS), // size of structure
                              &dwBytesNeeded)) {              // size needed if buffer is too small
        qDebug() << "QueryServiceStatusEx failed (%d)\n" << GetLastError();
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }

    // Check if the service is already running. It would be possible
    // to stop the service here, but for simplicity this example just returns.
    if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING) {
        qDebug() << "Cannot start the service because it is already running\n";
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }

    // Save the tick count and initial checkpoint.
    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    // Wait for the service to stop before attempting to start it.
    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING) {
        // Do not wait longer than the wait hint. A good interval is
        // one-tenth of the wait hint but not less than 1 second
        // and not more than 10 seconds.
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status until the service is no longer stop pending.
        if (!QueryServiceStatusEx(schService,                     // handle to service
                                  SC_STATUS_PROCESS_INFO,         // information level
                                  (LPBYTE) &ssStatus,             // address of structure
                                  sizeof(SERVICE_STATUS_PROCESS), // size of structure
                                  &dwBytesNeeded)) {              // size needed if buffer is too small
            qDebug() << "QueryServiceStatusEx failed (%d)\n" << GetLastError();
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint) {
            // Continue to wait and check.
            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        } else {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint) {
                qDebug() << "Timeout waiting for service to stop\n";
                CloseServiceHandle(schService);
                CloseServiceHandle(schSCManager);
                return;
            }
        }
    }

    // Attempt to start the service.
    if (!StartService(schService, // handle to service
                      0,          // number of arguments
                      NULL)) {    // no arguments
        qDebug() << "StartService failed (%d)\n" << GetLastError();
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    } else
        printf("Service start pending...\n");

    // Check the status until the service is no longer start pending.
    if (!QueryServiceStatusEx(schService,                     // handle to service
                              SC_STATUS_PROCESS_INFO,         // info level
                              (LPBYTE) &ssStatus,             // address of structure
                              sizeof(SERVICE_STATUS_PROCESS), // size of structure
                              &dwBytesNeeded)) {              // if buffer too small
        qDebug() << "QueryServiceStatusEx failed (%d)\n" << GetLastError();
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }

    // Save the tick count and initial checkpoint.
    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING) {
        // Do not wait longer than the wait hint. A good interval is
        // one-tenth the wait hint, but no less than 1 second and no
        // more than 10 seconds.
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status again.
        if (!QueryServiceStatusEx(schService,                     // handle to service
                                  SC_STATUS_PROCESS_INFO,         // info level
                                  (LPBYTE) &ssStatus,             // address of structure
                                  sizeof(SERVICE_STATUS_PROCESS), // size of structure
                                  &dwBytesNeeded)) {              // if buffer too small
            qDebug() << "QueryServiceStatusEx failed (%d)\n" << GetLastError();
            break;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint) {
            // Continue to wait and check.
            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        } else {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint) {
                // No progress made within the wait hint.
                break;
            }
        }
    }

    // Determine whether the service is running.
    if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
        qDebug() << "Service started successfully.\n";
    } else {
        qDebug() << ("Service not started. \n") << "  Current State: %d\n"
                 << ssStatus.dwCurrentState << "  Exit Code: %d\n"
                 << ssStatus.dwWin32ExitCode << "  Check Point: %d\n"
                 << ssStatus.dwCheckPoint << "  Wait Hint: %d\n"
                 << ssStatus.dwWaitHint;
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}
void RDesk::removeRow(int index, bool confirm)
{
    OrderDishStruct *od = ui->tblOrder->item(index, 0)->data(Qt::UserRole).value<OrderDishStruct *>();

    if (!od) {
        return;
    }

    if (od->fDishId == fHall->fServiceItem) {
        if (od->fPrice > 0.01) {
            message_error(tr("This item cannot be removed"));
            return;
        }
    }

    if (confirm) {
        if (!message_question(tr("Confirm to delete the selected item"))) {
            return;
        }
    }

    QString oldQty = QString("%1, %2/%3").arg(od->fName).arg(od->fQty).arg(od->fQtyPrint);
    int trackUser = fStaff->fId;

    if (od->fQtyPrint < 0.01) {
        od->fState = DISH_STATE_EMPTY;
        od->fEmark.clear();
        ui->tblOrder->setRowHidden(index, true);
    } else {
        message_error(tr("Cannot remove printed dish, use order correction tool"));
        return;
    }

        QString newQty = QString("%1, %2/%3").arg(od->fName).arg(od->fQty).arg(od->fQtyPrint);
        fTrackControl->insert("Dish qty", oldQty, newQty);
        countDish(od);
        updateDish(od);
        resetPrintQty();

        int serviceIndex = -1;
        bool removeService = true;

        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct *>();

            if (od->fDishId == fHall->fServiceItem && od->fState == DISH_STATE_READY) {
                serviceIndex = i;
                continue;
            }

            if (!fNoService && od->fState == DISH_STATE_READY && od->fSvcValue > 0.001) {
                removeService = false;
                continue;
            }
        }

    countTotal();

    if (removeService && serviceIndex > -1) {
        ui->tblOrder->setCurrentCell(serviceIndex, 0);
        removeRow(serviceIndex, false);
    }

    changeBtnState();
    repaintTables();
}
void RDesk::on_btnPrintMultipleFiscal_clicked()
{
    DlgPrintMultipleFiscal d(this);
    d.exec();
}

void RDesk::on_btnQr_clicked()
{
    if (fTable == nullptr) {
        message_error(tr("Please, select table"));
        return;
    }

    bool ok;
    QString barcode = QInputDialog::getText(this, "", "", QLineEdit::Normal, "", &ok);

    if (!ok) {
        return;
    }

    QString part = barcode;

    if (part.length() > 28) {
        if (part.length() > 36) {
            part = part.mid(3, 13);
        } else {
            part = part.mid(0, 13);
        }
    }

    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_scancode"] = part;
    db2.exec("select f_id from r_dish where f_scancode=:f_scancode");

    if (db2.next() == false) {
        message_error(tr("Invalid barcode"));
        return;
    }

    int dishid = db2.integer("f_id");
    DishStruct *dd = nullptr;

    for (QMap<int, QList<DishStruct *>>::const_iterator it = fDishTable.fDishMenu.constBegin(); it != fDishTable.fDishMenu.constEnd();
         it++) {
        for (DishStruct *d : it.value()) {
            if (d->fId == dishid) {
                dd = d;
                break;
            }
        }
    }

    if (!dd) {
        message_error(tr("Dish not in menu"));
        return;
    }

    int rec = addDishToOrder(dd, true);

    if (rec == 0) {
        message_error(tr("Error while append dish to order"));
        return;
    }

    if (dd->fNeedEmarks && isValidEmarkCode(barcode)) {
        db2[":f_id"] = rec;
        db2[":f_emark"] = barcode;

        if (!db2.exec("update o_dish set f_emark=:f_emark where f_id=:f_id")) {
            QString err = db2.lastDbError();

            if (err.contains("Duplicate entry")) {
                err = tr("Emark already used");
                message_error(err);
            }
        }
    }

    repaintTables();
}
void RDesk::on_leCmd_returnPressed()
{
    QString code = ui->leCmd->text();
    ui->leCmd->clear();

    if (fTable == nullptr) {
        message_error(tr("Please, select table"));
        return;
    }

    if (code.length() == 13) {
        DishStruct *d = fDishTable.getDishStructByBarcode(code, fMenu);

        if (d) {
            addDishToOrder(d, true);
        }
    } else if (code.length() == 8) {
        DishStruct *d = fDishTable.getDishStructByBarcode(code, fMenu);

        if (d) {
            if (d->fNeedEmarks) {
                message_error(tr("Only by Emarks code"));
                return;
            }

            addDishToOrder(d, true);
        }
    } else if (code.length() >= 29) {
        QString emarks = code;
        QString barcode;
        DishStruct *d = nullptr;

        if (code.mid(0, 8) == "01000000") {
            barcode = code.mid(8, 8);
        } else if (code.mid(0, 6) == "000000") {
            barcode = code.mid(6, 8);
        } else if (code.mid(0, 3) == "010") {
            barcode = code.mid(3, 13);
        } else {
            barcode = code.mid(1, 8);
            d = fDishTable.getDishStructByBarcode(barcode, fMenu);

            if (!d) {
                barcode.clear();
            }

            if (barcode.isEmpty()) {
                barcode = code.mid(1, 13);
            }

            if (!(d = fDishTable.getDishStructByBarcode(barcode, fMenu))) {
                barcode = "";
            }
        }

        emarks = code;
        code = barcode;

        if (barcode.isEmpty()) {
            message_error(tr("Invalid emarks"));
            return;
        }

        d = fDishTable.getDishStructByBarcode(barcode, fMenu);

        if (!d) {
            barcode.clear();
        }

        if (!d) {
            message_error(tr("Invalid barcode"));
            return;
        }

        Db b = Preferences().getDatabase(Base::fDbName);
        Database2 db2;
        db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
        db2[":f_emark"] = emarks;
        db2.exec("select f_id from o_dish where f_emark=:f_emark");

        if (db2.next()) {
            message_error(tr("Used emarks detected"));
            return;
        }

        d->tempEmark = emarks;
        int recid = addDishToOrder(d, true);
        db2[":f_emark"] = emarks;
        db2[":f_id"] = recid;
        db2.exec("update o_dish set f_emark=:f_emark where f_id=:f_id");
    }
}
