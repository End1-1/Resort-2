#include "whotelstatus.h"
#include "ui_whotelstatus.h"
#include "databaseresult.h"

WHotelStatus::WHotelStatus(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WHotelStatus)
{
    ui->setupUi(this);
    DatabaseResult dr;
    ui->tblMain->setItemWithValue(0, 1, WORKING_DATE);
    ui->tblMain->setItemWithValue(1, 1, dr.singleResult("select count(f_id) from f_room"));
    ui->tblMain->setItemWithValue(2, 1, dr.singleResult("select sum(f_bedQty) from f_room"));
    ui->tblMain->setItemWithValue(2, 3, dr.singleResult("select count(f_id) from f_reservation where f_state=1"));
    ui->tblMain->setItemWithValue(0, 5, ui->tblMain->toInt(1, 1) - ui->tblMain->toInt(2, 3));
    ui->tblMain->setItemWithValue(2, 5, dr.singleResult("select sum(f_pricePerNight) / count(f_id) from f_reservation where f_state=1"));
    dr.select("select sum(f_man) as man, sum(f_woman) as woman, sum(f_child) as child from f_reservation where f_state=1", fDbBind);
    ui->tblMain->setItemWithValue(0, 9, dr.value("man"));
    ui->tblMain->setItemWithValue(1, 9, dr.value("woman"));
    ui->tblMain->setItemWithValue(2, 9, dr.value("child"));
    QMap<QString, int> fBB;
    fBB["F/B"] = 0;
    fBB["H/B"] = 1;
    fBB["B/B"] = 2;
    fBB["B/O"] = 3;
    dr.select("select ra.f_" + def_lang + ", count(r.f_id) from f_reservation r "
              "left join f_room_arrangement ra on ra.f_id=r.f_arrangement "
              "where f_state=1 group by 1", fDbBind);
    for (int i = 0; i  < dr.rowCount(); i++) {
        ui->tblMain->setItemWithValue(fBB[dr.value(i, 0).toString()], 11, dr.value(i, 1));
    }

    fDbBind[":f_startDate"] = WORKING_DATE;
    fDbBind[":f_state"] = RESERVE_RESERVE;
    fDb.select("select r.f_room, concat(g.f_firstName, ' ', g.f_lastName), "
               "r.f_man+r.f_woman+r.f_child, r.f_endDate "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_startDate=:f_startDate and r.f_state=:f_state", fDbBind, fDbRows);
    ui->tblArrivals->setColumnCount(4);
    Utils::tableSetHeaderCaptions(ui->tblArrivals, ui->tblArrivals->columnCount(),
                                  QString("Room"), QString("Guest"), QString("Pax"), QString("Depature"));
    Utils::tableSetColumnWidths(ui->tblArrivals, ui->tblArrivals->columnCount(),
                                80, 150, 50, 100);
    Utils::fillTableWithData(ui->tblArrivals, fDbRows);
    ui->lbArrivals->setText(QString("%1 (%2)").arg(tr("Arrivals Today")).arg(fDbRows.count()));
    ui->tblMain->setItemWithValue(0, 3, fDbRows.count());

    fDbBind[":f_startDate"] = WORKING_DATE;
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_room, concat(g.f_firstName, ' ', g.f_lastName), "
               "r.f_man+r.f_woman+r.f_child, r.f_endDate "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_startDate=:f_startDate and r.f_state=:f_state", fDbBind, fDbRows);
    ui->tblAlreadyArrived->setColumnCount(4);
    Utils::tableSetHeaderCaptions(ui->tblAlreadyArrived, ui->tblAlreadyArrived->columnCount(),
                                  QString("Room"), QString("Guest"), QString("Pax"), QString("Depature"));
    Utils::tableSetColumnWidths(ui->tblAlreadyArrived, ui->tblAlreadyArrived->columnCount(),
                                80, 150, 50, 100);
    Utils::fillTableWithData(ui->tblAlreadyArrived, fDbRows);
    ui->lbAlreadyAriived->setText(QString("%1 (%2)").arg(tr("Already arrived")).arg(fDbRows.count()));

    fDbBind[":f_endDate"] = WORKING_DATE;
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_room, concat(g.f_firstName, ' ', g.f_lastName), "
               "r.f_man+r.f_woman+r.f_child, r.f_endDate "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_endDate=:f_endDate and r.f_state=:f_state", fDbBind, fDbRows);
    ui->tblDepartures->setColumnCount(4);
    Utils::tableSetHeaderCaptions(ui->tblDepartures, ui->tblDepartures->columnCount(),
                                  QString("Room"), QString("Guest"), QString("Pax"), QString("Depature"));
    Utils::tableSetColumnWidths(ui->tblDepartures, ui->tblDepartures->columnCount(),
                                80, 150, 50, 100);
    Utils::fillTableWithData(ui->tblDepartures, fDbRows);
    ui->lbDepartures->setText(QString("%1 (%2)").arg(tr("Departures Today")).arg(fDbRows.count()));
    ui->tblMain->setItemWithValue(1, 3, fDbRows.count());

    fDbBind[":f_endDate"] = WORKING_DATE;
    fDbBind[":f_state"] = RESERVE_CHECKOUT;
    fDb.select("select r.f_room, concat(g.f_firstName, ' ', g.f_lastName), "
               "r.f_man+r.f_woman+r.f_child, r.f_endDate "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_endDate=:f_endDate and r.f_state=:f_state", fDbBind, fDbRows);
    ui->tblAlreadyCheckout->setColumnCount(4);
    Utils::tableSetHeaderCaptions(ui->tblAlreadyCheckout, ui->tblAlreadyCheckout->columnCount(),
                                  QString("Room"), QString("Guest"), QString("Pax"), QString("Depature"));
    Utils::tableSetColumnWidths(ui->tblAlreadyCheckout, ui->tblAlreadyCheckout->columnCount(),
                                80, 150, 50, 100);
    Utils::fillTableWithData(ui->tblAlreadyCheckout, fDbRows);
    ui->lbAlreadyCheckout->setText(QString("%1 (%2)").arg(tr("Already checkout")).arg(fDbRows.count()));

    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_room, concat(g.f_firstName, ' ', g.f_lastName), "
               "r.f_startDate, r.f_endDate, r.f_man+r.f_woman+r.f_child "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_state=:f_state order by r.f_room ", fDbBind, fDbRows);
    ui->tblGuestInHouse->setColumnCount(5);
    Utils::tableSetHeaderCaptions(ui->tblGuestInHouse, ui->tblGuestInHouse->columnCount(),
                                  QString("Room"), QString("Guest"), QString("Arrival"), QString("Departure"), QString("Pax"));
    Utils::tableSetColumnWidths(ui->tblGuestInHouse, ui->tblGuestInHouse->columnCount(),
                                80, 150, 100, 100, 0);
    Utils::fillTableWithData(ui->tblGuestInHouse, fDbRows);
    ui->lbGuestInHouse->setText(QString("%1 (%2 / %3)").arg(tr("Guest in house")).arg(fDbRows.count()).arg((int)ui->tblGuestInHouse->sumOfColumn(4)));

    Utils::tableSetColumnWidths(ui->tblCategory, ui->tblCategory->columnCount(), 30, 150, 50, 50);
    fDb.select("select c.f_short, c.f_description, coalesce(rmc.qty, 0), coalesce(rc.qty, 0) \
               from f_room_classes c \
               left join (select rm.f_class, count(f_id) as qty from f_room rm group by 1) rmc on rmc.f_class=c.f_id \
               left join (select rm.f_class, count(r.f_id) as qty from f_reservation r left join f_room rm on rm.f_id=r.f_room where r.f_state=1 group by 1) rc on rc.f_class=c.f_id \
               group by 1, 2, 3", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblCategory, fDbRows);

    Utils::tableSetColumnWidths(ui->tblRoomStatus, ui->tblRoomStatus->columnCount(), 200, 50);
    fDb.select("select rs.f_en, coalesce(rmc.total, 0) \
               from f_room_state rs \
               left join (select r.f_state, count(r.f_id) as total from f_room r group by 1) rmc on rmc.f_state=rs.f_id", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblRoomStatus, fDbRows);

    Utils::tableSetColumnWidths(ui->tblNationality, ui->tblNationality->columnCount(),
                                40, 200, 50, 50);
    fDb.select("select n.f_short, n.f_name, coalesce(rn.total, 0), coalesce(rp.total, 0) \
               from f_nationality n \
               left join (select g.f_nation, count(r.f_id) as total from f_reservation r \
                   left join f_guests g on g.f_id=r.f_guest \
                   where r.f_state=1 group by 1) rn on rn.f_nation=n.f_short \
               left join (select g.f_nation, count(rg.f_id) as total from f_reservation_guests rg \
                   left join f_reservation r on r.f_id=rg.f_reservation \
                   left join f_guests g on g.f_id=rg.f_guest \
                   where r.f_state=1 group by 1) rp on rp.f_nation=n.f_short \
               where rn.total is not null", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblNationality, fDbRows);

    ui->tblMain->setMaximumHeight((ui->tblMain->rowCount() * ui->tblMain->verticalHeader()->defaultSectionSize()) + 5);
    int maxWidth = 0;
    for (int i = 0; i < ui->tblRoomStatus->columnCount(); i++) {
        maxWidth += ui->tblRoomStatus->columnWidth(i);
    }
    maxWidth += 20;
    ui->tblRoomStatus->setMaximumWidth(maxWidth);

    maxWidth = 0;
    for (int i = 0; i < ui->tblCategory->columnCount(); i++) {
        maxWidth += ui->tblCategory->columnWidth(i);
    }
    maxWidth += 20;
    ui->tblCategory->setMaximumWidth(maxWidth);
}

WHotelStatus::~WHotelStatus()
{
    delete ui;
}

void WHotelStatus::setup()
{
    setupTabTextAndIcon(tr("Hotel Status"), ":/images/info.png");
}
