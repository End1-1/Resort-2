#include "guestcheckin.h"
#include "ui_guestcheckin.h"
#include "wreportgrid.h"

GuestCheckin::GuestCheckin(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::GuestCheckin)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Checkin guests"), ":/images/guests.png");
}

GuestCheckin::~GuestCheckin()
{
    delete ui;
}

QString GuestCheckin::reportTitle()
{
    return QString("%1").arg(tr("List of checkin guests"));
}

QWidget *GuestCheckin::firstElement()
{
    return 0;
}

void GuestCheckin::apply(WReportGrid *rg)
{
    QString sql = "Select r.f_startdate, r.f_enddate, r.f_room, r.f_invoice, c.f_name, n.f_name, concat(g.f_firstName, ' ', g.f_lastName) \
        From f_reservation r \
        Left join f_reservation_guests rg on rg.f_reservation =r.f_id \
        left join f_cardex c on c.f_cardex=r.f_cardex \
        Left join f_guests g on g.f_id=rg.f_guest \
        left join f_nationality n on n.f_short=g.f_nation \
        Where r.f_state=:f_state :nation and r.f_startdate between :date1 and :date2 ";

    sql.replace(":f_state", QString::number(RESERVE_CHECKIN));
    sql.replace(":date1", "'2000-01-01'");
    sql.replace(":date2", "'2050-01-01'");


        sql.replace(":nation", "");

    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Entry"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(80, "", tr("Room"))
            .setColumn(100, "", tr("Invoice"))
            .setColumn(200, "", tr("Cardex"))
            .setColumn(150, "", tr("Nationality"))
            .setColumn(300, "", tr("Guest"));
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
}
