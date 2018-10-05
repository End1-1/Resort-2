#include "froomstate.h"
#include "ui_froomstate.h"
#include "wreportgrid.h"

FRoomState::FRoomState(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FRoomState)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Changes of states of room"), ":/images/refresh.png");
}

FRoomState::~FRoomState()
{
    delete ui;
}

QWidget *FRoomState::firstElement()
{
    return ui->deStart;
}

QString FRoomState::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Changes of room state"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FRoomState::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Date"))
            .setColumn(100, "", tr("Time"))
            .setColumn(120, "", tr("Old state"))
            .setColumn(120, "", tr("New state"))
            .setColumn(300, "", tr("User"))
            .setColumn(200, "", tr("Comment"))
            .setColumn(80, "", tr("Room"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(120, "", tr("Entry"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(200, "", tr("Cardex"));
    QString query = "select c.f_date, c.f_time, rs1.f_en, rs2.f_en, concat(u.f_firstName, ' ', u.f_lastName), c.f_comment, "
            "r.f_room, g.guest, r.f_startDate, r.f_endDate, ca.f_name "
            "from f_room_state_change c "
            "left join f_room_state rs1 on rs1.f_id=c.f_oldState "
            "left join f_room_state rs2 on rs2.f_id=c.f_newState "
            "left join users u on u.f_id=c.f_user "
            "left join f_reservation r on r.f_id=c.f_reservation "
            "left join guests g on g.f_id=r.f_guest "
            "left join f_cardex ca on ca.f_cardex=r.f_cardex "
            "where c.:col between " + ap(ui->deStart->date().toString(def_mysql_date_format)) + " and " + ap(ui->deEnd->date().toString(def_mysql_date_format));
    if (ui->chOnlyChecinCheckout->isChecked()) {
        query += " and c.f_newState in (1,2) ";
    }
    query += " order by 1, 2 ";
    if (ui->rbReal->isChecked()) {
        query.replace(":col", "f_date");
    } else {
        query.replace(":col", "f_wdate");
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
}

void FRoomState::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FRoomState *clb = new FRoomState(rg);
    rg->addFilterWidget(clb);
    clb->apply(rg);
}

void FRoomState::on_btnDateLeft_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FRoomState::on_btnDateRight_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}
