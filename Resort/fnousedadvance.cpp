#include "fnousedadvance.h"
#include "ui_fnousedadvance.h"
#include "wreportgrid.h"

FNoUsedAdvance::FNoUsedAdvance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FNoUsedAdvance)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Advance report"), ":/images/credit-card.png");
}

FNoUsedAdvance::~FNoUsedAdvance()
{
    delete ui;
}

QString FNoUsedAdvance::reportTitle()
{
    return QString("%1").arg(tr("Advanvces"));
}

QWidget *FNoUsedAdvance::firstElement()
{
    return this;
}

void FNoUsedAdvance::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Voucher"))
            .setColumn(100, "", tr("Date"))
            .setColumn(100, "", tr("Room"))
            .setColumn(150, "", tr("Guest"))
            .setColumn(120, "", tr("Entry"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(100, "", tr("Amount"))
            .setColumn(120, "", tr("Mode of payment"))
            .setColumn(100, "", tr("Comment"))
            .setColumn(80, "", tr("Tax"))
            .setColumn(100, "", tr("Operator"));
    QString query = "select m.f_id, m.f_wdate, m.f_room, m.f_guest, r.f_startDate, r.f_endDate,"
            "m.f_amountAmd*m.f_sign*-1, p.f_en, m.f_paymentComment, m.f_fiscal, u.f_username "
            "from m_register m "
            "left join f_reservation r on r.f_invoice=m.f_inv "
            "left join f_payment_type p on p.f_id=m.f_paymentMode "
            "left join users u on u.f_id=m.f_user "
            "where f_source='AV' and r.f_state=2 and m.f_canceled=0 "
            "order by r.f_id "
            ;
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 6;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);

}

void FNoUsedAdvance::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FNoUsedAdvance *v = new FNoUsedAdvance(rg);
    rg->addFilterWidget(v);
    v->apply(rg);
}
