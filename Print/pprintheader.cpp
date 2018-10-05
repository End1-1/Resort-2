#include "pprintheader.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "pimage.h"

PPrintHeader::PPrintHeader(PPrintScene *ps, const QString &headerText, int number) :
    Base()
{
    PTextRect *trHeader = new PTextRect(20, 20, 2000, 200, headerText, 0, QFont("Arial", 50));
    trHeader->setBorders(false, false, false, false);
    trHeader->setTextAlignment(Qt::AlignHCenter);
    QString inv = QString("#%1")
            .arg(QString("%1").arg(number,
                               fPreferences.getDb(def_vouchers_digits).toString().length(), 10, QChar('0')));
    PTextRect *trInvoice = new PTextRect(20, trHeader->textHeight(), 2000, 80, inv, 0, QFont("Arial", 20));
    trInvoice->setTextAlignment(Qt::AlignHCenter);
    trInvoice->setBorders(false, false, false, false);
    PTextRect *trInfo = new PTextRect(1500, 20, 500, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                      0, QFont("Arial", 25));
    trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
    trInfo->setBorders(false, false, false, false);
    ps->addItem(trInfo);
    ps->addItem(trInvoice);
    ps->addItem(trHeader);
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(20, 10, 500, 300));
}
