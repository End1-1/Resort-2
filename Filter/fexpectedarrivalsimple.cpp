#include "fexpectedarrivalsimple.h"
#include "ui_fexpectedarrivalsimple.h"
#include "wreportgrid.h"

FExpectedArrivalSimple::FExpectedArrivalSimple(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FExpectedArrivalSimple)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Expected arrivals / simple"),
                                     ":/images/arrival.png");
}

FExpectedArrivalSimple::~FExpectedArrivalSimple()
{
    delete ui;
}

void FExpectedArrivalSimple::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Room"))
            .setColumn(100, "", tr("Guests"));
    rg->fModel->setSqlQuery("select f_short, f_man+f_woman + f_child "
                            "from f_reservation r "
                            "inner join f_room rm on rm.f_id=r.f_room "
                            "where f_startDate='" + WORKING_DATE.toString(def_mysql_date_format) + "' "
                            "and r.f_state=2 "
                            "order by r.f_room ");
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 1;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
    rg->countToTotal(0);
}

QWidget *FExpectedArrivalSimple::firstElement()
{
    return 0;
}

QString FExpectedArrivalSimple::reportTitle()
{
    return QString("%1").arg(tr("Expected arrivals"));
}
