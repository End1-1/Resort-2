#include "wreservations.h"
#include "wreservation.h"

WReservations::WReservations(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(70, "f_id", tr("Code"))
            .setColumn(0, "f_state", tr("State code"))
            .setColumn(120, "f_state_name", tr("State"))
            .setColumn(80, "f_f_invoice", tr("Invoice"))
            .setColumn(0, "f_f_room", tr("Room code"))
            .setColumn(100, "f_room_short", tr("Room"))
            .setColumn(0, "f_guest", tr("Guest code"))
            .setColumn(200, "f_guest_name", tr("Guest"))
            .setColumn(120, "f_startDate", tr("Entry"))
            .setColumn(120, "f_endDate", tr("Departure"))
            .setColumn(0, "f_paymentType", tr("Payment code"))
            .setColumn(100, "f_payment", tr("Payment"))
            .setColumn(80, "f_price", tr("Price"))
            .setColumn(0, "f_meal", tr("Meal code"))
            .setColumn(80, "f_meal_name", tr("Meal"))
            .setColumn(80, "f_meal", tr("Meal price"));

    fModel->setSqlQuery("select r.f_id, r.f_state, s.f_en as f_state_name, r.f_invoice, r.f_room, "
                        "rm.f_short as f_room_short, r.f_guest, "
                        "concat(g.f_firstName, ' ', g.f_lastName) as f_guest_name, r.f_startDate, r.f_endDate, "
                        "r.f_paymentType, p.f_en, r.f_price, r.f_mealPlan, m.f_short as f_meal_name, r.f_meal "
                        "from f_reservation r "
                        "left join f_reservation_state s on r.f_state=s.f_id "
                        "left join f_room rm on r.f_room=rm.f_id "
                        "left join f_guests g on r.f_guest=g.f_id "
                        "left join f_payment_type p on  r.f_paymentType=p.f_id "
                        "left join f_meal m on r.f_meal=m.f_id ");
    fModel->apply(this);
}

void WReservations::processValues(int row, bool isNew)
{
    if (!isNew) {
        WReservation::openReserveWindows(fModel->data(row, 0).toString());
    }
}

void WReservations::setupTab()
{
    setupTabTextAndIcon(tr("Reservations"), ":/images/checkin.png");
}
