#include "freservebycreate.h"
#include "ui_freservebycreate.h"
#include "wreportgrid.h"
#include "wreservation.h"

FReserveByCreate::FReserveByCreate(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FReserveByCreate)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Reservation by date created"), ":/images/bed.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClick(QList<QVariant>)));
}

FReserveByCreate::~FReserveByCreate()
{
    delete ui;
}

QWidget *FReserveByCreate::firstElement()
{
    return ui->deStart;
}

QString FReserveByCreate::reportTitle()
{
    return QString("%1\r\n%2 - %3")
            .arg(tr("Reservation by date created"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FReserveByCreate::apply(WReportGrid *rg)
{
    QString query = "select r.f_id, r.f_created, r.f_createTime, r.f_startDate, r.f_endDate, r.f_room, rs.f_en, g.guest, "
            "u.f_username "
            "from f_reservation r "
            "left join guests g on g.f_id=r.f_guest "
            "left join f_reservation_state rs on rs.f_id=r.f_state "
            "left join users u on u.f_id=r.f_author "
            "where r.f_state in (1, 2, 3) and r.f_created between " + ui->deStart->dateMySql() + " and " + ui->deEnd->dateMySql();
    rg->fModel->clearColumns();
    rg->fModel->setColumn(80, "r.f_id", tr("Code"))
            .setColumn(100, "r.f_created", tr("Created"))
            .setColumn(80, "r.f_time", tr("Time"))
            .setColumn(120, "r.f_startDate", tr("Entry"))
            .setColumn(120, "r.f_endDate", tr("Departure"))
            .setColumn(100, "r.f_room", tr("Room"))
            .setColumn(150, "rs.f_en", tr("State"))
            .setColumn(200, "g.guest", tr("Guest"))
            .setColumn(200, "u.f_username", tr("Operator"));
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
}

void FReserveByCreate::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FReserveByCreate *y = new FReserveByCreate(rg);
    rg->addFilterWidget(y);
    y->apply(rg);
}

void FReserveByCreate::doubleClick(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        message_error_tr("Nothing is selected");
        return;
    }
    WReservation::openReserveWindows(row.at(0).toString());
}

void FReserveByCreate::on_toolButton_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FReserveByCreate::on_toolButton_2_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}
