#include "freservaionremarks.h"
#include "ui_freservaionremarks.h"
#include "wreportgrid.h"

FReservaionRemarks::FReservaionRemarks(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FReservaionRemarks)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Remarks"), ":/images/remarks.png");
}

FReservaionRemarks::~FReservaionRemarks()
{
    delete ui;
}

void FReservaionRemarks::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fTableView->verticalHeader()->setDefaultSectionSize(40);
    rg->fModel->setColumn(100, "f_room", tr("Room"))
            .setColumn(200, "f_guest", tr("Guest"))
            .setColumn(700, "f_remarks", tr("Remarks"));
    rg->fModel->setSqlQuery(QString("select r.f_room, concat(g.f_firstName, ' ' , g.f_lastName), r.f_remarks "
                        "from f_reservation r "
                        "left join f_guests g on g.f_id=r.f_guest "
                        "where r.f_state=%1 order by r.f_startDate")
                            .arg(ui->rbCheckin->isChecked() ? 1 : 2));
    rg->fModel->apply(rg);
    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        rg->fModel->setBackgroundColor(i, (i % 2 == 0 ? Qt::white : COLOR_DARK_ROW));
    }
}

QWidget *FReservaionRemarks::firstElement()
{
    return ui->rbCheckin;
}

QString FReservaionRemarks::reportTitle()
{
    return QString("%1, %2")
            .arg(tr("Reservation remarks"))
            .arg(ui->rbCheckin->isChecked() ? tr("Checkin") : tr("Reserved"));
}

void FReservaionRemarks::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FReservaionRemarks *y = new FReservaionRemarks(rg);
    rg->addFilterWidget(y);
    y->apply(rg);
}

void FReservaionRemarks::on_rbReserve_clicked()
{
    apply(fReportGrid);
}

void FReservaionRemarks::on_rbCheckin_clicked()
{
    apply(fReportGrid);
}
