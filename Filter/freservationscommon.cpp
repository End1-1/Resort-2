#include "freservationscommon.h"
#include "ui_freservationscommon.h"
#include "wreportgrid.h"
#include "vauchers.h"

FReservationsCommon::FReservationsCommon(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FReservationsCommon)
{
    ui->setupUi(this);
    fReportGrid->addToolBarButton(":/images/invoice.png", tr("Open invoice"), SLOT(openInvoice()), this)->setFocusPolicy(Qt::NoFocus);
    ui->deStart->setDate(WORKING_DATE);
    ui->deEnd->setDate(WORKING_DATE.addDays(90));
    fDockStatus = new DWSelectorReserveStatus(this);
    fDockStatus->configure();
    fDockStatus->setSelector(ui->leStatus);
    connect(fDockStatus, SIGNAL(reserveStatus(CI_ReserveStatus*)), this, SLOT(status(CI_ReserveStatus*)));
    fDockState = new DWSelectorReserveState(this);
    fDockState->configure();
    fDockState->setSelector(ui->leState);
    connect(fDockState, SIGNAL(reserveState(CI_ReserveState*)), this, SLOT(state(CI_ReserveState *)));
    fDockRoom =  new DWSelectorRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->leRoom);
    connect(fDockRoom, SIGNAL(room(CI_Room*)), this, SLOT(room(CI_Room*)));
    fDockArr = new DWSelectorRoomArrangment(this);
    fDockArr->configure();
    fDockArr->setSelector(ui->leArrangement);
    connect(fDockArr, SIGNAL(roomArrangment(CI_RoomArrangment*)), this, SLOT(arrangement(CI_RoomArrangment*)));
    fDockCardex = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardex);
    connect(fDockCardex, SIGNAL(cardex(CI_Cardex*)), this, SLOT(cardex(CI_Cardex*)));
    CI_ReserveState *c = CacheReserveState::instance()->get(RESERVE_RESERVE);
    state(c);
    fDockGroup = new DWSelectorReservationGroup(this);
    fDockGroup->configure();
    fDockGroup->setSelector(ui->leGroupNum);
    connect(fDockGroup, SIGNAL(reserveGroup(CI_ReserveGroup*)), this, SLOT(reserveGroup(CI_ReserveGroup*)));
}

FReservationsCommon::~FReservationsCommon()
{
    delete ui;
}

void FReservationsCommon::apply(WReportGrid *rg)
{
    QString where = "where ";
    if (ui->rbArrival->isChecked()) {
        where += "r.f_startdate between " + ui->deStart->dateMySql() + " and " + ui->deEnd->dateMySql() + " ";
    } else {
        where += "r.f_endDate between " + ui->deStart->dateMySql() + " and " + ui->deEnd->dateMySql() + " ";
    }
    if (!ui->leState->text().isEmpty()) {
        where += " and r.f_state in (" + ui->leState->fHiddenText + ") ";
    }
    if (!ui->leStatus->text().isEmpty()) {
        where += " and r.f_reserveState in (" + ui->leStatus->fHiddenText + ") ";
    }
    if (!ui->leArrangement->text().isEmpty()) {
        where += " and r.f_arrangement in (" + ui->leArrangement->fHiddenText + ") ";
    }
    if (!ui->leRoom->text().isEmpty()) {
        where += " and r.f_room in (" + ui->leRoom->fHiddenText + ") ";
    }
    if (!ui->leCardex->text().isEmpty()) {
        where += " and r.f_cardex in (" + Utils::separateForQuote(ui->leCardex->fHiddenText) + ") ";
    }
    if (!ui->leGroupNum->fHiddenText.isEmpty()) {
        where += " and r.f_group in (" + ui->leGroupNum->fHiddenText + ") ";
    }
    if (ui->chOnlyWithCardex->isChecked()) {
        where += " and (r.f_cardex <> '' and r.f_cardex <>'" + fPreferences.getDb(def_default_cardex).toString() + "') ";
    }
    if (!ui->leBooking->isEmpty()) {
        where += " and r.f_booking like '%" + ui->leBooking->text() + "%' ";
    }
    if (ui->chOnlyGroup->isChecked()) {
        where += " and r.f_group>0 ";
    }
    where += " order by r.f_startDate ";
    buildQuery(rg, where);
    QList<int> cols;
    cols << 11 << 12 << 13 << 17;
    QList<double> vals;
    fReportGrid->fModel->sumOfColumns(cols, vals);
    fReportGrid->setTblTotalData(cols, vals);
    if (fReportGrid->fTableTotal->toInt(0, 12) > 0) {
        fReportGrid->fTableTotal->setItemWithValue(0, 13,
            fReportGrid->fTableTotal->toDouble(0, 17) / fReportGrid->fTableTotal->toInt(0, 12));
    }
}

QWidget *FReservationsCommon::firstElement()
{
    return ui->deStart;
}

QString FReservationsCommon::reportTitle()
{
    return QString("%1 %2 From: %3 To %4, State: %5")
            .arg(tr("Reservations"))
            .arg(ui->rbArrival->isChecked() ? ui->rbArrival->text() : ui->rbDeparture->text())
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text())
            .arg(ui->leState->text().isEmpty() ? "-" : ui->leState->text());
}

void FReservationsCommon::state(CI_ReserveState *c)
{
    dockResponse<CI_ReserveState, CacheReserveState>(ui->leState, c);
}

void FReservationsCommon::status(CI_ReserveStatus *c)
{
    dockResponse<CI_ReserveStatus, CacheReserveStatus>(ui->leStatus, c);
}

void FReservationsCommon::room(CI_Room *c)
{
    dockResponse<CI_Room, CacheRoom>(ui->leRoom, c);
}

void FReservationsCommon::arrangement(CI_RoomArrangment *c)
{
    dockResponse<CI_RoomArrangment, CacheRoomArrangment>(ui->leArrangement, c);
}

void FReservationsCommon::reserveGroup(CI_ReserveGroup *c)
{
    dockResponse<CI_ReserveGroup, CacheReserveGroup>(ui->leGroupNum, c);
}

void FReservationsCommon::cardex(CI_Cardex *c)
{
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardex, c);
}

void FReservationsCommon::openInvoice()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    fDbBind[":f_id"] = out.at(0);
    fDb.select("select f_invoice from f_reservation where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Cannot open invoice");
        return;
    }
    QString inv = fDbRows.at(0).at(0).toString();
    if (inv.isEmpty()) {
        return;
    }

    openInvoiceWithId(inv);
}
