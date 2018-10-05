#include "fcouponsales.h"
#include "ui_fcouponsales.h"
#include "wreportgrid.h"
#include "dlgcouponsales.h"
#include "dwselectorstorepartners.h"

#define SEL_PARTNER 1

FCouponSales::FCouponSales(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCouponSales)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Coupons sales"), ":/images/discount.png");
    fReportGrid->addToolBarButton(":/images/discount.png", tr("Coupons sales"), SLOT(newSale()), this)->setFocusPolicy(Qt::NoFocus);
    connect(fReportGrid, &WReportGrid::doubleClickOnRow, [this](const QList<QVariant> &v) {
        if (v.count() == 0) {
            message_info(tr("Nothing was selected"));
            return;
        }
    });
    DWSelectorStorePartners *dockPart = new DWSelectorStorePartners(this);
    dockPart->configure();
    dockPart->setSelector(ui->lePartnerCode);
    dockPart->setDialog(this, SEL_PARTNER);
}

FCouponSales::~FCouponSales()
{
    delete ui;
}

QString FCouponSales::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Coupons sales"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FCouponSales::firstElement()
{
    return ui->deStart;
}

void FCouponSales::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Code"))
            .setColumn(120, "", tr("Date"))
            .setColumn(200, "", tr("Partner"))
            .setColumn(150, "", tr("Payment"))
            .setColumn(100, "", tr("Seria"))
            .setColumn(100, "", tr("Qty"))
            .setColumn(100, "", tr("Amount"));
    QString query = "select s.f_id, s.f_date, p.f_name, pm.f_name, cs.f_name, s.f_qty, s.f_amount "
            "from d_coupon_sale s "
            "left join r_partners p on p.f_id=s.f_partner "
            "left join r_doc_payment pm on pm.f_id=s.f_payment "
            "left join d_coupon_seria cs on cs.f_id=s.f_seria "
            "where s.f_date between " + ui->deStart->dateMySql() + " and " + ui->deEnd->dateMySql() + " ";
    if (ui->lePartnerCode->asInt() > 0) {
        query += " and s.f_partner=" + ui->lePartnerCode->text();
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 5 << 6;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FCouponSales::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_PARTNER: {
        dock<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, value.value<CI_StorePartners*>());
        break;
    }
    }
}

void FCouponSales::newSale()
{
    DlgCouponSales::sale();
}
