#include "dlgprintreservation.h"
#include "ui_dlgprintreservation.h"
#include "pprintpreview.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "pimage.h"
#include "cacheroom.h"
#include "pprintheader.h"
#include "pprintcheckin.h"

DlgPrintReservation::DlgPrintReservation(WReservationRoomTab *r) :
    BaseExtendedDialog(r),
    ui(new Ui::DlgPrintReservation)
{
    ui->setupUi(this);
    fSource = r;
    ui->btnPrintConfirmation->setEnabled(r->reserveState() == RESERVE_RESERVE);
    ui->btnPrintReservation->setEnabled(r->reserveState() == RESERVE_RESERVE);
    ui->btnPrintRegistrationCard->setEnabled(r->reserveState() == RESERVE_CHECKIN);
}

DlgPrintReservation::~DlgPrintReservation()
{
    delete ui;
}

void DlgPrintReservation::on_btnClose_clicked()
{
    reject();
}

void DlgPrintReservation::on_btnPrintConfirmation_clicked()
{
    PPrintPreview *pp = new PPrintPreview();
    PPrintScene *ps = pp->addScene(0, Portrait);
    PTextRect *trHeader = new PTextRect(20, 20, 2100, 200, tr("RESERVATION CONFIRMATION"), 0, QFont("Arial", 50));
    trHeader->setBorders(false, false, false, false);
    trHeader->setTextAlignment(Qt::AlignHCenter);
    QString inv = QString("S/N %1").arg(fSource->valueForWidget("Doc number"));
    PTextRect *trInvoice = new PTextRect(20, trHeader->textHeight(), 2100, 80, inv, 0, QFont("Arial", 20));
    trInvoice->setTextAlignment(Qt::AlignHCenter);
    trInvoice->setBorders(false, false, false, false);
    PTextRect *trInfo = new PTextRect(1500, 20, 600, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                      0, QFont("Arial", 25));
    trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
    trInfo->setBorders(false, false, false, false);
    ps->addItem(trInfo);
    ps->addItem(trInvoice);
    ps->addItem(trHeader);
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(20, 10, 500, 300));
    PTextRect th;
    th.setBorders(false, false, false, false);
    PTextRect *r = 0;
    QFont f(QFont("Arial", 25));
    f.setBold(true);
    th.setFont(f);
    int top = 310;
    int rowHeight = 60;
    ps->addTextRect(new PTextRect(20, top, 300, rowHeight, tr("To") + ":", &th, f));
    r = ps->addTextRect(new PTextRect(300, top, 2100, rowHeight, fSource->mainGuest(), &th, f));
    top += r->textHeight();
    ps->addTextRect(new PTextRect(20, top, 300, rowHeight, tr("Date") + ":", &th, f));
    r = ps->addTextRect(new PTextRect(300, top, 2100, rowHeight, fSource->valueForWidget("Created") + ":", &th, f));
    top += r->textHeight();
    QPen boldPen;
    boldPen.setWidth(4);
    top += 10;
    ps->addLine(20, top, 2100, top, boldPen);

    //Dear-shmiar text
    f.setBold(false);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(20, top, 2100, rowHeight, tr("Dear") + ", " + fSource->mainGuest(), &th, f))->textHeight();
    top += ps->addTextRect(new PTextRect(20, top, 2100, rowHeight, tr("We Thank you for your interest in further to your request, we are very pleased to confirm your reservation as follows:"), &th, f))->textHeight();
    top += rowHeight;
    //confirmation data
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Name of guest"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, fSource->mainGuest(), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Arrival date"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, fSource->valueForWidget("Entry date"), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Departure date"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, fSource->valueForWidget("Departure date"), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("No of Adults / Childs"), &th, f));
    QString adults_childs = QString::number(fSource->valueForWidget("Male").toInt() + fSource->valueForWidget("Female").toInt());
    int childs = fSource->valueForWidget("Childs").toInt() + fSource->valueForWidget("Baby").toInt();
    if (childs > 0) {
        adults_childs += "/" + QString::number(childs);
    }
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, adults_childs, &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Accomodation Type"), &th, f));
    CI_Room *room = CacheRoom::instance()->get(fSource->valueForWidget("Room"));
    if (!room) {
        message_error("Cannot print reservation confirmation, room was not defined. Contact to application developer.");
        return;
    }
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, room->fCategory, &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Room rate"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, fSource->valueForWidget("Price per night"), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Buffet Breakfast"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, (fSource->valueForWidget("Meal included").toInt() == 0 ? tr("Not included") : tr("Included")), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Payment mode"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, fSource->valueForWidget("Payment mode"), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Check In Time"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, "", &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Check Out Time"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, "", &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Airport pick up"), &th, f));
    top += ps->addTextRect(new PTextRect(20, top, 600, rowHeight, fSource->valueForWidget("Pickup fee"), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Exchange rate"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, QString::number(def_usd, 'f', 2) + " USD", &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Remarks"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, fSource->valueForWidget("Remarks"), &th, f))->textHeight();
    f.setBold(true);
    f.setUnderline(true);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(20, top,600, rowHeight, tr("Billing instructions"), &th, f))->textHeight();
    f.setBold(false);
    f.setUnderline(false);
    th.setFont(f);
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("No Show Policy"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, tr("One Night will be charged"), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Cancelation Policy"), &th, f));
    top += ps->addTextRect(new PTextRect(600, top, 1500, rowHeight, tr("Cancelation Policy"), &th, f))->textHeight();
    top += rowHeight;
    ps->addLine(20, top, 2100, top, boldPen);
    top += 2;
    ps->addLine(20, top, 2100, top, boldPen);
    top += (rowHeight * 2);
    ps->addTextRect(new PTextRect(20, top, 600, rowHeight, tr("Executive Director"), &th, f));
    pp->exec();
    delete pp;
    accept();
}

void DlgPrintReservation::on_btnPrintReservation_clicked()
{
    QList<int> col;
    QStringList val;
    PPrintPreview *pp = new PPrintPreview(this);
    PPrintScene *ps = pp->addScene(0, Portrait);
    PPrintHeader(ps, tr("RESERVATION VOUCHER"), fSource->valueForWidget("Doc number").toInt());
    int top = 310;
    QPen boldPen;
    boldPen.setWidth(6);
    PTextRect th;
    th.setBorders(false, false, false, false);
    QFont f("Arial", 25);
    th.setFont(f);
    int rowHeight = 60;
    top += 5;
    top += ps->addTextRect(20, top, 2100, rowHeight, tr("Date") + " " + QDate::currentDate().toString(def_date_format), &th)->textHeight();
    top += ps->addTextRect(20, top, 2000, rowHeight, tr("Guest name") + " " + fSource->mainGuest(), &th)->textHeight();
    ps->addLine(20, top, 2100, top, boldPen);
    top += 20;
    th.setBorders(true, true, true, true);
    col << 20 << 300 << 600 << 300 << 600 << 300;
    val << tr("Arrival date")
        << fSource->valueForWidget("Entry date")
        << tr("Departure date")
        << fSource->valueForWidget("Departure date")
        << tr("Number of nights");
    ps->addTableRow(top, rowHeight, col, val, &th);
    val << tr("Arrival time")
        << fSource->valueForWidget("Arrival time")
        << tr("Departure time")
        << fSource->valueForWidget("Departure time")
        << fSource->valueForWidget("Nights");
    ps->addTableRow(top, rowHeight, col, val, &th);
    col.clear();
    top += 20;
    col << 20 << 300 << 600 << 400 << 90 << 400 << 300;
    CI_Room *c = CacheRoom::instance()->get(fSource->valueForWidget("Room"));
    if (!c) {
        return;
    }
    val << tr("Category")
        << c->fCategory
        << tr("Male")
        << fSource->valueForWidget("Male")
        << tr("Arrangement")
        << fSource->valueForWidget("Arrangement");
    ps->addTableRow(top, rowHeight, col, val, &th);
    val << tr("View")
        << c->fView
        << tr("Female")
        << fSource->valueForWidget("Female")
        << tr("VIP")
        << " ";
    ps->addTableRow(top, rowHeight, col, val, &th);
    val << tr("Bed type")
        << c->fBed
        << tr("Child")
        << fSource->valueForWidget("Childs")
        << tr("Bed and brakfast")
        << "1";
    ps->addTableRow(top, rowHeight, col, val, &th);
    val << tr("Room")
        << fSource->valueForWidget("Room")
        << tr(" ") // block
        << " "
        << tr(" ") // level
        << " ";
    ps->addTableRow(top, rowHeight, col, val, &th);
    val << tr("Cardex")
        << fSource->valueForWidget("Cardex") + "/" + fSource->valueForWidget("Cardex name")
        << tr(" ") //none
        << " "
        << tr(" ") //none
        << " " ;
    ps->addTableRow(top, rowHeight, col, val, &th);
    top += 20;
    col.clear();
    col << 20 << 200 << 300 << 200 << 600 << 300 << 490;
    val << tr("Rooming")
        << fSource->valueForWidget("Rate")
        << tr("VAT")
        << fSource->valueForWidget("VAT")
        << tr("Status")
        << fSource->valueForWidget("Status");
    ps->addTableRow(top, rowHeight, col, val, &th);
    pp->exec();
    delete pp;
    accept();
}

void DlgPrintReservation::on_btnPrintRegistrationCard_clicked()
{
    PPrintCheckin::print(fSource->reserveId());
}
