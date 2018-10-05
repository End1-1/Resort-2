#include "fallguestindaterange.h"
#include "ui_fallguestindaterange.h"
#include "wreportgrid.h"
#include "dwselectornation.h"

FAllGuestInDateRange::FAllGuestInDateRange(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAllGuestInDateRange)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Guest by nationality"), ":/images/groupreservation.png");
    DWSelectorNation *dNat = new DWSelectorNation(this);
    dNat->configure();
    dNat->setSelector(ui->leNatCode);
    connect(dNat, &DWSelectorNation::nation, [this](CI_Nation *ci) {
       dock<CI_Nation, CacheNation>(ui->leNatCode, ui->leNatName, ci);
    });
}

FAllGuestInDateRange::~FAllGuestInDateRange()
{
    delete ui;
}

QString FAllGuestInDateRange::reportTitle()
{
    return QString("%1 %2-%3")
            .arg("Guests by nationality")
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FAllGuestInDateRange::firstElement()
{
    return ui->deStart;
}

void FAllGuestInDateRange::apply(WReportGrid *rg)
{
    QString sql = "Select r.f_startdate, r.f_enddate, r.f_room, r.f_invoice, c.f_name, n.f_name, concat(g.f_firstName, ' ', g.f_lastName) \
        From f_reservation r \
        Left join f_reservation_guests rg on rg.f_reservation =r.f_id \
        left join f_cardex c on c.f_cardex=r.f_cardex \
        Left join f_guests g on g.f_id=rg.f_guest \
        left join f_nationality n on n.f_short=g.f_nation \
        Where r.f_state=:f_state :nation and r.f_startdate between :date1 and :date2 ";

    if (ui->chCheckinOnly->isChecked()) {
        sql.replace(":f_state", QString::number(RESERVE_CHECKIN));
        sql.replace(":date1", "'2000-01-01'");
        sql.replace(":date2", "'2050-01-01'");
    } else {
        sql.replace(":f_state", QString::number(RESERVE_CHECKOUT));
        sql.replace(":date1", ui->deStart->dateMySql());
        sql.replace(":date2", ui->deEnd->dateMySql());
    }


    if (ui->leNatCode->isEmpty()) {
        sql.replace(":nation", "");
    } else {
        sql.replace(":nation", "and r.f_guest in (select f_id from f_guests where f_nation=" + ap(ui->leNatCode->text()) + ")");
    }

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

void FAllGuestInDateRange::on_btnForward_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}

void FAllGuestInDateRange::on_btnBack_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}
