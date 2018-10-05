#include "reservationinfo.h"
#include "ui_reservationinfo.h"
#include "dlgreservequickinfo.h"
#include "winvoice.h"
#include "wreservation.h"
#include <QPaintEvent>
#include <QPainter>

static const QColor rc[][2] = {
    {Qt::white, Qt::white},
    {QColor::fromRgb(70, 170, 255), QColor::fromRgb(120, 190, 255)},
    {QColor::fromRgb(255, 80, 80), QColor::fromRgb(255, 135, 135)},
    {QColor::fromRgb(88, 88, 88), QColor::fromRgb(170, 170, 170)},
    {QColor::fromRgb(88, 88, 88), QColor::fromRgb(170, 170, 170)},
    {QColor::fromRgb(88, 88, 88), QColor::fromRgb(170, 170, 170)},
    {QColor::fromRgb(88, 88, 88), QColor::fromRgb(170, 170, 170)}
};

static const QColor rs[] = {
    Qt::yellow,
    Qt::green,
    Qt::gray
};

ReservationInfo::ReservationInfo(CI_Reservation *r, QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ReservationInfo)
{
    ui->setupUi(this);
    fReservation = r;
    QSize size;
    size.setHeight(ROW_HEIGHT - 4);
    if (r->fDateEnd > r->fDateStart) {
        size.setWidth(COLUMN_WIDTH * r->fDateStart.daysTo(r->fDateEnd) - 4);
    } else {
        size.setWidth(COLUMN_WIDTH  / 2);
    }
    setMinimumSize(size);
    setMaximumSize(size);
}

ReservationInfo::~ReservationInfo()
{
    delete ui;
}

void ReservationInfo::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    switch (fReservation->fState) {
    case RESERVE_CHECKIN: {
        WInvoice *w = 0;
        for (int i = 0; i < fMainWindow->fTab->count(); i++) {
            w = dynamic_cast<WInvoice*>(fMainWindow->fTab->widget(i));
            if (w) {
                if (w->activeDoc(fReservation->fInvoice)) {
                    fMainWindow->fTab->setCurrentIndex(i);
                    return;
                }
            }
        }
        w = addTab<WInvoice>();
        w->loadInvoice(fReservation->fInvoice);
        break;
    }
    case RESERVE_RESERVE: {
        WReservation *w = 0;
        for (int i = 0; i < fMainWindow->fTab->count(); i++) {
            w = dynamic_cast<WReservation*>(fMainWindow->fTab->widget(i));
            if (w) {
                if (w->activateDoc(fReservation->fId.toInt())) {
                    fMainWindow->fTab->setCurrentIndex(i);
                    return;
                }
            }
        }
        w = addTab<WReservation>();
        if (fReservation->fGroup == 0) {
            w->loadReservation(fReservation->fId.toInt());
        } else {
            w->setGroup(fReservation->fGroup);
            w->loadGroup(fReservation->fGroup);
        }
        break;
    }
    }
}

void ReservationInfo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    switch (fReservation->fState) {
    case RESERVE_CHECKIN:
    case RESERVE_CHECKOUT:
    case RESERVE_RESERVE:
        break;
    default:
        setVisible(false);
        deleteLater();
        return;
    }

    QRect rect(0, 0, width(), height());
    QLinearGradient bgFill(0, 0, 0, rect.height());
    bgFill.setColorAt(0, rc[fReservation->fState][0]);
    bgFill.setColorAt(0.5, rc[fReservation->fState][1]);
    bgFill.setColorAt(1, rc[fReservation->fState][0]);
    QBrush brush(bgFill);
    QPainter painter(this);
    painter.fillRect(rect, brush);
    painter.setPen(Qt::gray);
    painter.drawRect(0, 0, rect.width() - 1, rect.height() -1);
    QFont f("Times", 8);
    f.setBold(true);
    painter.setFont(f);
    int leftOffcet = 4;
    if (fReservation->fState == RESERVE_RESERVE) {
        QRect statusRect(4, 4, 7, 7);
        leftOffcet += 10;
        painter.setBrush(rs[fReservation->fStatus - 1]);
        painter.drawEllipse(statusRect);
    }
    QTextOption to;
    if (fReservation->fState == RESERVE_CHECKIN) {
        to.setAlignment(Qt::AlignRight);
    }
    QRect textRect = rect;
    painter.setPen(rc[fReservation->fState][1]);
    textRect.adjust(leftOffcet, 2, -1 * (ui->btnView->width() + 10), (-1 * (textRect.height() / 2)) + 3);
    painter.drawText(textRect, fReservation->fGuest, to);
    painter.setPen(Qt::black);
    textRect.adjust(-1, -1, 0, 0);
    painter.drawText(textRect, fReservation->fGuest, to);
    QRect dateRect = rect;
    dateRect.adjust(2, (dateRect.height() / 2), -1 * (ui->btnView->width() + 10), -2);
    QString strDate = QString("%1 - %2")
            .arg(fReservation->fDateStart.toString("dd/MM"))
            .arg(fReservation->fDateEnd.toString("dd/MM"));
    if (fReservation->fState == RESERVE_RESERVE) {
        if (fReservation->fAdvance.toDouble() > 0.01) {
            strDate += " (" + fReservation->fAdvance + "AMD)";
        }
    }
    painter.drawText(dateRect, strDate, to);
}

void ReservationInfo::on_btnView_clicked()
{
    DlgReserveQuickInfo *d = new DlgReserveQuickInfo(fReservation, fMainWindow);
    d->exec();
    delete d;
}
