#include "fcanceledreservations.h"
#include "ui_fcanceledreservations.h"
#include "wreportgrid.h"
#include "wreservation.h"

FCanceledReservations::FCanceledReservations(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCanceledReservations)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Canceled reservations"), ":/images/garbage.png");
    connect(fReportGrid, &WReportGrid::doubleClickOnRow, [this](const QList<QVariant> &values) {
        if (values.count() == 0) {
            return;
        }
        WReservation::openReserveWindows(values.at(0).toString());
    });
}

FCanceledReservations::~FCanceledReservations()
{
    delete ui;
}

QWidget *FCanceledReservations::firstElement()
{
    return ui->deStart;
}

QString FCanceledReservations::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Canceled reservations"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FCanceledReservations::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Code"))
            .setColumn(120, "", tr("Arrival"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(120, "", tr("Canceled"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(80, "", tr("Room"))
            .setColumn(250, "", tr("OP"))
            .setColumn(250, "", tr("Cancel by"));
    QString query = "select r.f_id, r.f_startDate, r.f_cancelDate, r.f_endDate, g.guest, "
            "r.f_room, concat(u1.f_firstName, ' ', u1.f_lastName), concat(u2.f_firstName, ' ', u2.f_lastName) "
            "from f_reservation r   "
            "left join guests g on g.f_id=r.f_guest "
            "left join users u1 on u1.f_id=r.f_author "
            "left join users u2 on u2.f_id=r.f_cancelUser "
            "where r.f_cancelDate between :date1 and :date2 and r.f_state=:f_state";
    query.replace(":date1", ui->deStart->dateMySql());
    query.replace(":date2", ui->deEnd->dateMySql());
    query.replace(":f_state", QString::number(RESERVE_REMOVED));
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);

}

void FCanceledReservations::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCanceledReservations *clb = new FCanceledReservations(rg);
    rg->addFilterWidget(clb);
    clb->apply(rg);
}
