#include "flistsourcereserve.h"
#include "ui_flistsourcereserve.h"
#include "wreportgrid.h"

FListSourceReserve::FListSourceReserve(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FListSourceReserve)
{
    ui->setupUi(this);
    fDockState =  new DWSelectorReserveState(this);
    QStringList stateFilter;
    stateFilter << QString::number(RESERVE_CHECKIN)
                << QString::number(RESERVE_RESERVE)
                << QString::number(RESERVE_REMOVED);
    fDockState->setFilterList(stateFilter);
    fDockState->configure();
    fDockState->setSelector(ui->leState);
    connect(fDockState, SIGNAL(reserveState(CI_ReserveState*)), this, SLOT(state(CI_ReserveState*)));
    fDockCardex = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardex);
    connect(fDockCardex, SIGNAL(cardex(CI_Cardex*)), this, SLOT(cardex(CI_Cardex*)));
    CI_ReserveState *c = CacheReserveState::instance()->get("2");
    dockResponse<CI_ReserveState, CacheReserveState>(ui->leState, c);
    fReportGrid->fTableView->verticalHeader()->setDefaultSectionSize(20);
    setup();
}

FListSourceReserve::~FListSourceReserve()
{
    delete ui;
}

QString FListSourceReserve::reportTitle()
{
    return QString("%1\r\n%2 - %3\r\n%3 %5")
            .arg(tr("LIST BY SOURCE OF RESERVATION"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text())
            .arg(ui->leState->text())
            .arg(ui->leCardex->text());
}

QWidget *FListSourceReserve::firstElement()
{
    return ui->deStart;
}

void FListSourceReserve::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Cardex code"))
            .setColumn(0, "", tr("Cardex"))
            .setColumn(80, "", tr("Room"))
            .setColumn(100, "", tr("Arr. Date"))
            .setColumn(100, "", tr("Departure"))
            .setColumn(100, "", tr("Code"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(100, "", tr("Rate"))
            .setColumn(80, "", tr("Night"))
            .setColumn(80, "", tr("Total"))
            .setColumn(80, "", tr("Advance"))
            .setColumn(200, "", tr("Op."))
            .setColumn(200, "", tr("Cardex"));
    QString sql;
    switch (ui->leState->fHiddenText.toInt()) {
    case RESERVE_RESERVE:
    case RESERVE_REMOVED:
        sql = QString("select r.f_cardex, c.f_name, r.f_room, r.f_startDate, r.f_endDate, r.f_id, concat(g.f_firstName, ' ', g.f_lastName), \
        r.f_pricePerNight, datediff(r.f_endDate, r.f_startDate) , r.f_grandTotal, ad.total, concat(u.f_firstName, ' ', u.f_lastName), \
        c.f_name \
        from f_reservation r \
        left join f_cardex c on c.f_cardex=r.f_cardex\
        left join f_guests g on g.f_id=r.f_guest \
        left join users u on u.f_id=r.f_lastEdit \
        left join (select f_rec, sum(f_amountAmd) as total from m_register where f_source='AV' and f_canceled=0 group by 1) ad on ad.f_rec=r.f_id\
        where r.f_state=:f_state \
        and r.f_startDate between :f_date1 and :f_date2 :cardex \
        order by r.f_cardex ")
                .replace(":f_state", ui->leState->fHiddenText)
                .replace(":f_date1", ui->deStart->dateMySql())
                .replace(":f_date2", ui->deEnd->dateMySql());
        break;
    case RESERVE_CHECKIN:
        sql = QString("select r.f_cardex, c.f_name, r.f_room, r.f_startDate, r.f_endDate, r.f_id, concat(g.f_firstName, ' ', g.f_lastName), \
        r.f_pricePerNight, datediff(r.f_endDate, r.f_startDate) , r.f_grandTotal, ad.total, concat(u.f_firstName, ' ', u.f_lastName), \
        c.f_name \
        from f_reservation r \
        left join f_cardex c on c.f_cardex=r.f_cardex\
        left join f_guests g on g.f_id=r.f_guest \
        left join users u on u.f_id=r.f_lastEdit \
        left join (select f_rec, sum(f_amountAmd) as total from m_register where f_source='AV' and f_canceled=0 group by 1) ad on ad.f_rec=r.f_id\
        where r.f_state=:f_state \
        and date_add(r.f_startDate, interval datediff(:f_date2,:f_date1) day) between :f_date1 and :f_date2 :cardex \
        order by r.f_cardex ")
                .replace(":f_state", ui->leState->fHiddenText)
                .replace(":f_date1", ui->deStart->dateMySql())
                .replace(":f_date2", ui->deEnd->dateMySql());
        break;
    default:
        message_info_tr("This reservation state is not allowed here");
        return;
    }
    if (!ui->leCardex->text().isEmpty()) {
        sql.replace(":cardex", " and r.f_cardex='" + ui->leCardex->fHiddenText + "'");
    } else {
        sql.replace(":cardex", "");
    }
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<QVariant> emptyRow;
    for (int i = 0; i < rg->fModel->columnCount(); i++) {
        emptyRow << QVariant();
    }
    QString curr;
    int i = 0;
    while (i < rg->fModel->rowCount()) {
        if (i == 0) {
            curr = rg->fModel->data(i, 1).toString();
            QList<QVariant> row = emptyRow;
            row[0] = curr;
            curr = rg->fModel->data(i, 1).toString();
            rg->fModel->insertRow(i, row);
            rg->fTableView->setSpan(i, 0, 1, rg->fModel->columnCount());
            i++;
            continue;
        }
        if (rg->fModel->data(i, 1).toString() != curr) {
            QList<QVariant> row = emptyRow;
            row[0] = rg->fModel->data(i, 1).toString();
            curr = rg->fModel->data(i, 1).toString();
            rg->fModel->insertRow(i, row);
            rg->fTableView->setSpan(i, 0, 1, rg->fModel->columnCount());
            i++;
            continue;
        }
        i++;
    }
}

void FListSourceReserve::setup()
{
    fReportGrid->setupTabTextAndIcon(tr("List of source reservation"), ":/images/notebook.png");
}

void FListSourceReserve::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FListSourceReserve *cs = new FListSourceReserve(rg);
    rg->addFilterWidget(cs);
    cs->apply(rg);
}

void FListSourceReserve::state(CI_ReserveState *c)
{
    dockResponse<CI_ReserveState, CacheReserveState>(ui->leState, c);
}

void FListSourceReserve::cardex(CI_Cardex *c)
{
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardex, c);
}
