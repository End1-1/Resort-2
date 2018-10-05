#include "freservegroups.h"
#include "ui_freservegroups.h"
#include "wreportgrid.h"
#include "dlggroupreservationfuck.h"

FReserveGroups::FReserveGroups(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FReserveGroups)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Reservation groups"), ":/images/bed.png");
    if (r__(cr__super_correction)) {
        fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove group"), SLOT(removeGroup()), this)->setFocusPolicy(Qt::NoFocus);
    }
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClick(QList<QVariant>)));
}

FReserveGroups::~FReserveGroups()
{
    delete ui;
}

QWidget *FReserveGroups::firstElement()
{
    return ui->deStart;
}

QString FReserveGroups::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Reservation groups"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

void FReserveGroups::apply(WReportGrid *rg)
{
    QString query = "select g.f_id, g.f_name, g.f_entry, g.f_departure, c.f_name, t.total, r.total, ch.total, ci.total, cc.total "
            "from f_reservation_group g "
            "left join f_cardex c on c.f_cardex=g.f_cardex "
            "left join (select rr.f_group, count(rr.f_id) as total from f_reservation rr where f_state in (1,2,3) group by 1) t on t.f_group=g.f_id "
            "left join (select rr.f_group, count(rr.f_id) as total from f_reservation rr where rr.f_state=2 group by 1) r on r.f_group=g.f_id "
            "left join (select rr.f_group, count(rr.f_id) as total from f_reservation rr where rr.f_state=3 group by 1) ch on ch.f_group=g.f_id "
            "left join (select rr.f_group, count(rr.f_id) as total from f_reservation rr where rr.f_state=1 group by 1) ci on ci.f_group=g.f_id "
            "left join (select rr.f_group, count(rr.f_id) as total from f_reservation rr where rr.f_state=6 group by 1) cc on cc.f_group=g.f_id "
            "where g.f_canceled=0 ";
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "f_id", tr("Code"))
            .setColumn(200, "f_name", tr("Group name"))
            .setColumn(120, "f_entry", tr("Entry"))
            .setColumn(120, "f_departure", tr("Departure"))
            .setColumn(200, "f_cardex", tr("Cardex"))
            .setColumn(80, "", tr("Rooms"))
            .setColumn(80, "", tr("Reserved"))
            .setColumn(80, "", tr("Checkout"))
            .setColumn(80, "", tr("Check in"))
            .setColumn(80, "", tr("Canceled"));
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
}

void FReserveGroups::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FReserveGroups *y = new FReserveGroups(rg);
    rg->addFilterWidget(y);
    y->apply(rg);
}

void FReserveGroups::doubleClick(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        message_error_tr("Nothing is selected");
        return;
    }
    DlgGroupReservationFuck *d = addTab<DlgGroupReservationFuck>();
    d->loadGroup(row.at(0).toInt());
}

void FReserveGroups::removeGroup()
{
    QList<QVariant> val;
    int row;
    if ((row = fReportGrid->fillRowValuesOut(val)) < 0) {
        message_error_tr("Nothing was selected");
        return;
    }
    if (message_confirm_tr("Confirm remove group") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_canceled"] = 1;
    fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
    fDbBind[":f_cancelUser"] = WORKING_USERID;
    fDb.update("f_reservation_group", fDbBind, where_id(val.at(0).toInt()));
    fDbBind[":f_state"] = RESERVE_REMOVED;
    fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
    fDbBind[":f_cancelUser"] = WORKING_USERID;
    fDb.update("f_reservation", fDbBind, where_field("f_group", val.at(0).toInt()));
    fReportGrid->fModel->removeRow(row);
}
