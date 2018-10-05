#include "flengthofstay.h"
#include "ui_flengthofstay.h"
#include "wreportgrid.h"
#include "wreservation.h"
#include "waccinvoice.h"

FLengthOfStay::FLengthOfStay(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FLengthOfStay)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Length of stay"), ":/images/bed.png");
    QToolButton *btnReserve = fReportGrid->addToolBarButton(":/images/bed.png", tr("Open reservation"), SLOT(openReservation()), this);
    btnReserve->setVisible(r__(cr__edit_reservation));
    btnReserve->setFocusPolicy(Qt::ClickFocus);
    QToolButton *btnInvoice = fReportGrid->addToolBarButton(":/images/invoice.png", tr("Open invoice"), SLOT(openInvoice()), this);
    btnInvoice->setFocusPolicy(Qt::ClickFocus);
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(dblClick(QList<QVariant>)));
}

FLengthOfStay::~FLengthOfStay()
{
    delete ui;
}

QString FLengthOfStay::reportTitle()
{
    return QString("%1 %2-%3, %4")
            .arg(tr("Length of stay"))
            .arg(ui->wd->ds1())
            .arg(ui->wd->ds2())
            .arg(ui->rbDates->isChecked() ? tr("By dates") : tr("By rooming"));
}

QWidget *FLengthOfStay::firstElement()
{
    return ui->wd->fw();
}

void FLengthOfStay::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Reservation id"))
            .setColumn(0, "", tr("Invoice"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(80, "", tr("Arr."))
            .setColumn(120, "", tr("Entry"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(80, "", tr("Nights"))
            .setColumn(80, "", tr("Rooming"))
            .setColumn(80, "", tr("Pax"))
            .setColumn(80, "", tr("Rate"))
            .setColumn(200, "", tr("Cardex"))
            .setColumn(200, "", tr("Booking"))
            .setColumn(100, "", tr("Debit"))
            .setColumn(100, "", tr("Credit"));
    QString query = "select r.f_id, r.f_invoice, g.guest, ar.f_" + def_lang + ", "
            "r.f_startDate, r.f_endDate, datediff(r.f_endDate,r.f_startDate) as nights, rm.qty as rooming, "
            "r.f_man+r.f_woman+r.f_child as pax, r.f_pricePerNight, cx.f_name, "
            "r.f_booking, d.total as debit, c.total as credit "
            "from f_reservation r "
            "left join f_room_arrangement ar on ar.f_id=r.f_arrangement "
            "left join guests g on g.f_id=r.f_guest "
            "left join (select f_inv, count(f_id) as qty from m_register where f_itemCode in (:rooming) and f_canceled=0 and f_finance=1 group by 1) rm on rm.f_inv=r.f_invoice "
            "left join f_cardex cx on cx.f_cardex=r.f_cardex "
            "left join (select f_inv, sum(f_amountAmd) as total from m_register where f_sign=1 and f_Canceled=0 and f_finance=1 group by 1) d on d.f_inv=r.f_invoice "
            "left join (select f_inv, sum(f_amountAmd) as total from m_register where f_sign=-1 and f_Canceled=0 and f_finance=1 group by 1) c on c.f_inv=r.f_invoice "
            "where r.f_state=3 and (r.f_startDate between :d1 and :d2 or r.f_endDate between :d1 and :d2) :and ";
    query.replace(":rooming", fPreferences.getDb(def_rooming_list).toString());
    query.replace(":d1", ui->wd->ds1());
    query.replace(":d2", ui->wd->ds2());
    if (ui->sp2->value() > 0) {
        if (ui->rbDates->isChecked()) {
            query.replace(":and", QString(" and datediff(r.f_endDate,r.f_startDate) between %1 and %2 ")
                          .arg(ui->sp1->value())
                          .arg(ui->sp2->value()));
        } else {
            query.replace(":and", QString(" and rm.qty between %1 and %2 ")
                          .arg(ui->sp1->value())
                          .arg(ui->sp2->value()));
        }
    } else {
        query.replace(":and", "");
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);

}

void FLengthOfStay::openReservation()
{
    QList<QVariant> out;
    int row = fReportGrid->fillRowValuesOut(out);
    if (row < 0) {
        return;
    }
    WReservation::openReserveWindows(out.at(0).toString());
}

void FLengthOfStay::openInvoice()
{
    QList<QVariant> out;
    int row = fReportGrid->fillRowValuesOut(out);
    if (row < 0) {
        return;
    }
    WAccInvoice *ac = addTab<WAccInvoice>();
    ac->load(out.at(1).toString());
}

void FLengthOfStay::dblClick(const QList<QVariant> &r)
{
    if (r.count() < 1) {
        return;
    }
    WAccInvoice *ac = addTab<WAccInvoice>();
    ac->load(r.at(1).toString());
}
