#include "reservewidget.h"
#include "ui_reservewidget.h"
#include "reserverect.h"
#include "cacheusers.h"
#include "reservewidgetaction.h"
#include "cachereservation.h"
#include "dlgooroomprop.h"
#include "vauchers.h"
#include "paymentmode.h"
#include <QResizeEvent>

ReserveWidget::ReserveWidget(ReserveRect *rr, QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ReserveWidget)
{
    ui->setupUi(this);
    fReservation = 0;
    fRoom = rr->fRoom;
    fDateStart = rr->fDateStart;
    fDateEnd = rr->fDateEnd;
    //setSize();
}

ReserveWidget::ReserveWidget(CI_Reservation *r, QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ReserveWidget)
{
    ui->setupUi(this);
    fReservation = r;
    fDateStart = r->fDateStart;
    fDateEnd = r->fDateEnd;
    QSize s(COLUMN_WIDTH - 1, ROW_HEIGHT - 1);
    ui->btnCancel->setMaximumSize(s);
    ui->btnCancel->setMinimumSize(s);
    fRoom = CacheRoom::instance()->get(r->fRoom);
    setSize();
    setToolTip(fReservation->fRemarks);
    if (fReservation->fState == RESERVE_OUTOFROOM) {
        if (WORKING_DATE >= fReservation->fDateStart && WORKING_DATE <= fReservation->fDateEnd) {

        }
    }
}

ReserveWidget::~ReserveWidget()
{
    delete ui;
}

void ReserveWidget::createService()
{
    QMap<QString, CI_Reservation*> out;
    bool startok = true;
    bool endok = true;
    CacheReservation *cr = CacheReservation::instance();
    cr->check(fDateStart, fDateEnd, fRoom->fCode.toInt(), out, startok, endok, "");
    if (!startok || !endok || out.count() > 0) {
        deleteLater();
        return;
    }
    QString invId = uuid(VAUCHER_INVOICE_N, fAirDb);
    fDbBind[":f_state"] = RESERVE_SERVICE;
    fDbBind[":f_reserveState"] = CONFIRM_BLOCK;
    fDbBind[":f_room"] = fRoom->fCode;
    fDbBind[":f_roomFee"] = fRoom->fPrice;
    fDbBind[":f_arrangement"] = fPreferences.getDb(def_room_arrangement).toInt();
    fDbBind[":f_paymentType"] = PAYMENT_CASH;
    fDbBind[":f_man"] = 1;
    fDbBind[":f_mealIncluded"] = 1;
    fDbBind[":f_cardex"] = fPreferences.getDb(def_default_cardex);
    fDbBind[":f_startDate"] = fDateStart;
    fDbBind[":f_endDate"] = fDateEnd;
    fDbBind[":f_author"] = WORKING_USERID;
    fDbBind[":f_invoice"] = invId;
    fId = uuid("RS", fAirDb);
    fDb.insertId("f_reservation", fId);
    fDbBind[":f_id"] = fId;
    fDb.update("f_reservation", fDbBind, where_id(ap(fId)));

    TrackControl tc(TRACK_RESERVATION);
    tc.fReservation = fId;
    tc.fInvoice = invId;
    tc.insert("-=CREATED DRAFT=-", QString("%1, %2 - %3")
                              .arg(fRoom->fCode)
                              .arg(fDateStart.toString(def_date_format))
                              .arg(fDateEnd.toString(def_date_format)), "");
    BroadcastThread::cmdRefreshCache(cid_reservation, fId);
}

void ReserveWidget::removeService()
{
    if (fReservation->fArrangement > 0) {
        return;
    }
    fDbBind[":f_state"] = RESERVE_SERVICE_REMOVED;
    fDb.update("f_reservation", fDbBind, where_id(ap(fReservation->fId)));
    deleteLater();
    BroadcastThread::cmdRefreshCache(cid_reservation, fReservation->fId);
}

void ReserveWidget::resizeEvent(QResizeEvent *e)
{
    QString state;
    switch (fReservation->fState) {
    case RESERVE_OUTOFINVENTORY:
        state = "O/I ";
        setStyleSheet("background:#4a4a4a;");
        ui->lbCaption->setStyleSheet("color:white;");
        break;
    case RESERVE_OUTOFROOM:
        state = "O/O ";
        setStyleSheet("background:#585858;");
        ui->lbCaption->setStyleSheet("color:white;");
        break;
    default:
        setStyleSheet("background:#c7a0f4;");
        ui->lbCaption->setStyleSheet("color:black;");
        break;
    }

    QFont f(qApp->font().family(), 6);
    QString text = "#" + fRoom->fCode;
    if (e->size().width() < COLUMN_WIDTH * 3) {
        text = "";
    } if (e->size().width() > COLUMN_WIDTH * 3) {
        text = state + text + "\n" + QString("%1 - %2")
                .arg(fDateStart.toString("dd, ddd"))
                .arg(fDateEnd.toString("dd, ddd"));
        f.setPointSize(6);
    }
    if (e->size().width() > COLUMN_WIDTH * 6) {
        f.setPointSize(8);
        text = QString("%1#%2 %3 - %4")
                .arg(state)
                .arg(fRoom->fCode)
                .arg(fDateStart.toString("dd, ddd"))
                .arg(fDateEnd.toString("dd, ddd"));
    }
    ui->lbCaption->setFont(f);
    ui->lbCaption->setText(text);
    QWidget::resizeEvent(e);
}

void ReserveWidget::on_btnCancel_clicked()
{
    if (fReservation->fState == RESERVE_OUTOFROOM) {
        DlgOORoomProp *d = new DlgOORoomProp(fReservation, fMainWindow);
        d->exec();
        delete d;
        return;
    }
    ReserveWidgetAction *ra = new ReserveWidgetAction(fPreferences.getDefaultParentForMessage());
    ra->setReservation(fReservation);
    ra->setAuthor(CacheUsers::instance()->get(fReservation->fAuthor)->fName);
    ra->setRemarks(fReservation->fRemarks);
    ra->lock(fReservation->fArrangement);
    bool broadcast = true;
    if (ra->exec() == QDialog::Accepted) {
        QString remarks = ra->remarks();
        if (remarks != fReservation->fRemarks) {
            fReservation->fRemarks = remarks;
            fDbBind[":f_remarks"] = ra->remarks();
            fDb.update("f_reservation", fDbBind, where_id(ap(fReservation->fId)));
            broadcast = true;
        }
        switch (ra->fResult) {
        case 1:
            emit editReserv(fReservation);
            break;
        case 2:
            fDbBind[":f_arrangement"] = WORKING_USERID;
            fDb.update("f_reservation", fDbBind, where_id(ap(fReservation->fId)));
            break;
        case 3:
            fDbBind[":f_arrangement"] = 0;
            fDb.update("f_reservation", fDbBind, where_id(ap(fReservation->fId)));
            break;
        case 4:
            fReservation->fArrangement = 0;
            removeService();
            return;
        case 5:
            fDbBind[":f_state"] = RESERVE_OUTOFROOM;
            fDb.update("f_reservation", fDbBind, where_id(ap(fReservation->fId)));
            if (fReservation->fDateStart == WORKING_DATE) {
                fDbBind[":f_state"] = RESERVE_OUTOFROOM;
                fDb.update("f_room", fDbBind, where_id(fReservation->fRoom));
            }
            broadcast = true;
            break;
        case 6:
            fDbBind[":f_state"] = RESERVE_OUTOFINVENTORY;
            fDb.update("f_reservation", fDbBind, where_id(ap(fReservation->fId)));
            if (fReservation->fDateStart == WORKING_DATE) {
                fDbBind[":f_state"] = RESERVE_OUTOFINVENTORY;
                fDb.update("f_room", fDbBind, where_id(fReservation->fRoom));
            }
            broadcast = true;
            break;
        }
    } else {
        broadcast = false;
    }
    if (broadcast) {
        BroadcastThread::cmdRefreshCache(cid_reservation, fReservation->fId);
        BroadcastThread::cmdRefreshCache(cid_room, fReservation->fRoom);
    }
    delete ra;
}

void ReserveWidget::setSize()
{
    int div = 1;
    if (CacheReservation::instance()->hasNext(fReservation)) {
        div = 2;
    }
    int width = ((fDateStart.daysTo(fDateEnd)) * COLUMN_WIDTH) + (COLUMN_WIDTH / div);
    setMinimumSize(width, ROW_HEIGHT);
    setMaximumSize(width, ROW_HEIGHT);
}
