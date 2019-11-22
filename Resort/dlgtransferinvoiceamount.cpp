#include "dlgtransferinvoiceamount.h"
#include "ui_dlgtransferinvoiceamount.h"
#include "cacheinvoiceitem.h"
#include "pprintpreview.h"
#include "paymentmode.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "pimage.h"
#include "vauchers.h"

DlgTransferInvoiceAmount::DlgTransferInvoiceAmount(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgTransferInvoiceAmount)
{
    ui->setupUi(this);
    fDockRoomFrom = new DWSelectorActiveRoom(this);
    fDockRoomFrom->configure();
    fDockRoomFrom->setSelector(ui->leRoomCodeFrom);
    connect(fDockRoomFrom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomFrom(CI_ActiveRoom*)));
    fDockRoomTo = new DWSelectorActiveRoom(this);
    fDockRoomTo->configure();
    fDockRoomTo->setSelector(ui->leRoomCodeTo);
    connect(fDockRoomTo, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomTo(CI_ActiveRoom*)));
}

DlgTransferInvoiceAmount::~DlgTransferInvoiceAmount()
{
    delete ui;
}

void DlgTransferInvoiceAmount::setRoomFrom(CI_ActiveRoom *r)
{
    if (r) {
        ui->leRoomCodeFrom->setText(r->fRoomCode);
        ui->leRoomNameFrom->setText(r->fName);
        ui->leGuestFrom->setText(r->fGuestName);
        getAmount(r->fInvoice);
    }
}

void DlgTransferInvoiceAmount::roomFrom(CI_ActiveRoom *c)
{
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leRoomCodeFrom, ui->leRoomNameFrom, c);
    if (c) {
        ui->leGuestFrom->setText(c->fGuestName);
        ui->leRoomCodeFrom->setText(c->fRoomCode);
        getAmount(c->fInvoice);
    }
}

void DlgTransferInvoiceAmount::roomTo(CI_ActiveRoom *c)
{
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leRoomCodeTo, ui->leRoomNameTo, c);
    if (c) {
        ui->leGuestTo->setText(c->fGuestName);
        ui->leRoomCodeTo->setText(c->fRoomCode);
    }
}

void DlgTransferInvoiceAmount::on_btnSave_clicked()
{
    QString errors;
    if (ui->leRoomCodeFrom->asInt() == 0) {
        errors += tr("Room from is not defined") + "<br>";
    }
    if (ui->leRoomCodeTo->asInt() == 0) {
        errors += tr("Room to is not defined") + "<br>";
    }
    if (ui->leAmountTo->asDouble() < 0.1) {
        errors += tr("Amount cannot be zero") + "<br>";
    }
    if (ui->leRoomCodeFrom->asInt() == ui->leRoomCodeTo->asInt()) {
        errors += tr("Same room code") + "<br>";
    }
    if (ui->leAmountTo->asDouble() - abs(ui->leAmountFrom->asDouble()) > 0.1) {
        errors += tr("Invalid amount") + "<br>";
    }
    if (!errors.isEmpty()) {
        message_error(errors);
        return;
    }
    CI_ActiveRoom *arFrom = CacheActiveRoom::instance()->get(ui->leRoomCodeFrom->text());
    if (!arFrom) {
        message_error("Cannot belive - 1!");
        return;
    }
    CI_ActiveRoom *arTo = CacheActiveRoom::instance()->get(ui->leRoomCodeTo->text());
    if (!arTo) {
        message_error("Cannot belive - 2!");
        return;
    }

    QString fromTo = QString("From %1 To %2").arg(ui->leRoomCodeFrom->text()).arg(ui->leRoomNameTo->text());
    fDb.fDb.transaction();

    int itemGroup = (ui->leAmountFrom->asDouble() < 0.1 ? IG_NEGATIVE_BALANCE : IG_POSITIVE_BALANCE);
    int itemId = (itemGroup == IG_NEGATIVE_BALANCE ? fPreferences.getDb(def_invoice_default_negative_transfer_id).toInt() : fPreferences.getDb(def_invoice_default_positive_transfer_id).toInt());
    int sign = (itemGroup == IG_NEGATIVE_BALANCE ? 1 : -1);
    bool result = true;

    QString rid1 = uuuid(VAUCHER_TRANSFER_AMOUNT_N, fAirDb);
    fDb.insertId("m_register", rid1);
    fDbBind[":f_source"] = VAUCHER_TRANSFER_AMOUNT_N;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_room"] = ui->leRoomCodeFrom->text();
    fDbBind[":f_guest"] = ui->leGuestFrom->text();
    fDbBind[":f_itemCode"] = itemId;
    fDbBind[":f_finalName"] = rid1 + " " + fromTo;
    fDbBind[":f_amountAmd"] = ui->leAmountTo->asDouble();
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = PAYMENT_TRANSFER;
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = 0;
    fDbBind[":f_paymentComment"] = "";
    fDbBind[":f_dc"] = (sign == -1 ? "CREDIT" : "DEBIT");
    fDbBind[":f_sign"] = sign;
    fDbBind[":f_doc"] = rid1;
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = CacheActiveRoom::instance()->get(ui->leRoomCodeFrom->text())->fInvoice;
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_remarks"] = fromTo;
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(rid1)));

    sign *= -1;
    QString rid2 = uuuid(VAUCHER_TRANSFER_AMOUNT_N, fAirDb);
    fDb.insertId("m_register", rid2);
    fDbBind[":f_source"] = VAUCHER_TRANSFER_AMOUNT_N;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_room"] = ui->leRoomCodeTo->text();
    fDbBind[":f_guest"] = ui->leGuestTo->text();
    fDbBind[":f_itemCode"] = itemId;
    fDbBind[":f_finalName"] = rid2 + " " + fromTo;
    fDbBind[":f_amountAmd"] = ui->leAmountTo->asDouble();
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = PAYMENT_TRANSFER;
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = 0;
    fDbBind[":f_paymentComment"] = "";
    fDbBind[":f_dc"] = (sign == -1 ? "CREDIT" : "DEBIT");;
    fDbBind[":f_sign"] = sign;
    fDbBind[":f_doc"] = rid2;
    fDbBind[":f_rec"] = 0;
    fDbBind[":f_inv"] = CacheActiveRoom::instance()->get(ui->leRoomCodeTo->text())->fInvoice;
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_remarks"] = fromTo;
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(rid2)));

    fDbBind[":f_doc"] = rid1;
    fDb.update("m_register", fDbBind, where_id(ap(rid2)));
    fDbBind[":f_doc"] = rid2;
    fDb.update("m_register", fDbBind, where_id(ap(rid1)));


    fTrackControl = new TrackControl(TRACK_RESERVATION);
    fTrackControl->insert("Transfer amount", QString("From %1 to %2, %3AMD")
                              .arg(ui->leRoomCodeFrom->text())
                              .arg(ui->leRoomCodeTo->text())
                              .arg(ui->leAmountTo->text()), "");
    fTrackControl->resetChanges();
    fTrackControl->insert("Transfer amount", QString("From %1 to %2, %3AMD")
                              .arg(ui->leRoomCodeFrom->text())
                              .arg(ui->leRoomCodeTo->text())
                              .arg(ui->leAmountTo->text()), "");
    if (result) {
        fDb.fDb.commit();
        ui->btnSave->setEnabled(false);
        ui->btnPrint->setEnabled(true);
    } else {
        fDb.fDb.rollback();
        message_error_tr("Transaction failed");
    }
}

void DlgTransferInvoiceAmount::on_btnCancel_clicked()
{
    reject();
}

void DlgTransferInvoiceAmount::on_leAmountTo_textChanged(const QString &arg1)
{
    if (arg1.toDouble() > abs(ui->leAmountFrom->asDouble())) {
        ui->leAmountTo->setDouble(abs(ui->leAmountFrom->asDouble()));
    }
}

void DlgTransferInvoiceAmount::on_btnPrint_clicked()
{
    CI_ActiveRoom *c = CacheActiveRoom::instance()->get(ui->leRoomCodeFrom->text());
    PPrintPreview *pp = new PPrintPreview(this);
    PPrintScene *ps = pp->addScene(0, Portrait);
    PTextRect *trHeader = new PTextRect(20, 20, 2000, 200, tr("Transfer voucher"), 0, QFont("Arial", 50));
    trHeader->setBorders(false, false, false, false);
    trHeader->setTextAlignment(Qt::AlignHCenter);
    QString inv = QString("S/N %1").arg(c->fInvoice);
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
    PTextRect th;
    th.setBorders(false, false, false, false);
    QFont f(QFont("Arial", 25));
    f.setBold(true);
    th.setFont(f);
    int top = 320;
    int rowHeight = 70;
    QList<int> cols;
    cols << 20 << 30 << 300 << 400 << 500 << 800;
    QStringList text;
    text << "#" << tr("Date") << tr("Amount") << tr("Mode of Payment") << tr("Purpose");
    th.setBorders(true, true, true, true);
    th.setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ps->addTableRow(top, rowHeight, cols, text, &th);
    th.setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    f.setBold(false);
    th.setFont(f);
    text << "1"
         << WORKING_DATE.toString(def_date_format)
         << ui->leAmountTo->text()
         << tr("Transfer")
         //<< (ui->rbRoom->isChecked() ? tr("Credit") : (ui->rbCity->isChecked() ? tr("Credit") : tr("Cash")))
         << QString("%1 transfer from %2 to %3")
            .arg(ui->lbBalanceType->text())
            .arg(ui->leRoomCodeFrom->text())
            .arg(ui->leRoomCodeTo->text());
    ps->addTableRow(top, rowHeight, cols, text, &th);

    top += 10;
    th.setBorders(false, false, false, false);
    ps->addTextRect(new PTextRect(20, top, 300, rowHeight, tr("Amount in words:"), &th, f));
    ps->addTextRect(new PTextRect(350, top, 2000, rowHeight, Utils::numberToWords(ui->leAmountTo->asDouble()), &th, f));
    top += (rowHeight * 4);

    ps->addTextRect(20, top, 450, rowHeight, tr("Guest"), &th);
    top += ps->addTextRect(new PTextRect(450, top, 400, rowHeight, ui->leGuestFrom->text(), &th))->textHeight();
    top += ps->addTextRect(new PTextRect(20, top, 400, rowHeight, tr("Signature:"), &th, f))->textHeight();
    ps->addItem(new QGraphicsLineItem(450, top, 1000, top));
    top += (rowHeight * 3);
    top += ps->addTextRect(new PTextRect(20, top, 400, rowHeight, tr("Office manager signagure:"), &th, f))->textHeight();
    ps->addItem(new QGraphicsLineItem(450, top, 1000, top));
    QString info = QString("%1: %2, %3. %4: %5")
            .arg(tr("Printer"))
            .arg(QDateTime::currentDateTime().toString(def_date_time_format))
            .arg(QHostInfo().localHostName())
            .arg(tr("Operator"))
            .arg(WORKING_USERNAME);
    f.setPointSize(10);
    th.setFont(f);
    ps->addTextRect(new PTextRect(20, 2900, 2000, rowHeight, info, &th, f));
    pp->exec();
    delete pp;
}

void DlgTransferInvoiceAmount::getAmount(const QString &invoice)
{
    fDbBind[":f_invoice"] = invoice;
    fDb.select("select sum(f_amountAmd*f_sign) from m_register "
               "where f_inv=:f_invoice and f_canceled=0 and f_finance=1 and f_side=0 ", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leAmountFrom->setText(fDbRows.at(0).at(0).toString());
        if (ui->leAmountFrom->asDouble() < 0.1) {
            ui->lbBalanceType->setText(tr("Negative balance"));
        } else {
            ui->lbBalanceType->setText(tr("Positive balance"));
        }
    }
}
