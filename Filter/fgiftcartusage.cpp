#include "fgiftcartusage.h"
#include "ui_fgiftcartusage.h"
#include "wreportgrid.h"
#include "dlggposorderinfo.h"

FGiftCartUsage::FGiftCartUsage(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FGiftCartUsage)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Gift card usage"), ":/images/car.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(onRowDoubleClick(QList<QVariant>)));
}

FGiftCartUsage::~FGiftCartUsage()
{
    delete ui;
}

QString FGiftCartUsage::reportTitle()
{
    return QString("%1 — %2").arg(tr("Gift card usage"), ui->leCard->text());
}

QWidget *FGiftCartUsage::firstElement()
{
    return ui->leCard;
}

void FGiftCartUsage::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(80, "f_order", tr("Order #"))
            .setColumn(120, "f_order_date", tr("Order date"))
            .setColumn(120, "f_govnumber", tr("Lisence plate"))
            .setColumn(200, "f_car_model", tr("Car model"))
            .setColumn(100, "f_used", tr("Used"))
            .setColumn(350, "f_payments", tr("Payment"));

    if(fCardCode.isEmpty()) {
        rg->fModel->setSqlQuery(QStringLiteral("select 1 where 1=0"));
        rg->fModel->apply(rg);
        return;
    }

    auto sqlLiteral = [](QString text) {
        text.replace("'", "''");
        return QString("'%1'").arg(text);
    };

    QString escapedCode = fCardCode;
    escapedCode.replace("'", "''");
    const QString codeSql = QString("'%1'").arg(escapedCode);

    const QString lblCash = sqlLiteral(tr("Cash"));
    const QString lblCard = sqlLiteral(tr("Card"));
    const QString lblIdram = sqlLiteral(tr("Idram"));
    const QString lblPrepaid = sqlLiteral(tr("Prepaid"));
    const QString lblDebt = sqlLiteral(tr("Debt"));
    const QString lblCoupon = sqlLiteral(tr("Gift card"));
    const QString lblCouponBank = sqlLiteral(tr("Gift transfer"));
    const QString lblCouponService = sqlLiteral(tr("Coupon service"));

    const QString query = QString(R"(
SELECT u.f_order AS f_order,
       oh.f_dateCash AS f_order_date,
       coalesce(oc.f_govnumber, '') AS f_govnumber,
       trim(concat(coalesce(cm.f_model, ''), ' ', coalesce(cm.f_class, ''))) AS f_car_model,
       abs(u.f_amount) AS f_used,
       coalesce(trim(both ', ' from concat_ws(', ',
           if(coalesce(op.f_cash, 0) > 0.001, concat(%2, ': ', cast(round(op.f_cash) as char)), null),
           if(coalesce(op.f_card, 0) > 0.001, concat(%3, ': ', cast(round(op.f_card) as char)), null),
           if(coalesce(op.f_idram, 0) > 0.001, concat(%4, ': ', cast(round(op.f_idram) as char)), null),
           if(coalesce(op.f_prepaid, 0) > 0.001, concat(%5, ': ', cast(round(op.f_prepaid) as char)), null),
           if(coalesce(op.f_debt, 0) > 0.001, concat(%6, ': ', cast(round(op.f_debt) as char)), null),
           if(coalesce(op.f_coupon, 0) > 0.001, concat(%7, ': ', cast(round(op.f_coupon) as char)), null),
           if(coalesce(op.f_couponbank, 0) > 0.001, concat(%8, ': ', cast(round(op.f_couponbank) as char)), null),
           if(coalesce(op.f_couponservice, 0) > 0.001, concat(%9, ': ', cast(round(op.f_couponservice) as char)), null)
       )), '') AS f_payments
FROM d_gift_cart_use u
INNER JOIN o_header oh ON oh.f_id = u.f_order
LEFT JOIN o_header_payment op ON op.f_id = oh.f_id
LEFT JOIN o_car oc ON oc.f_order = oh.f_id
LEFT JOIN d_car_model cm ON cm.f_id = oc.f_model
WHERE u.f_code = %1
  AND u.f_order > 0
  AND u.f_amount < 0
ORDER BY oh.f_dateCash DESC, u.f_order DESC
)").arg(codeSql, lblCash, lblCard, lblIdram, lblPrepaid, lblDebt, lblCoupon, lblCouponBank, lblCouponService);

    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);

    QList<int> cols;
    cols << 4;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FGiftCartUsage::openReport(const QString &cardCode, const QString &cardLabel)
{
    if(cardCode.isEmpty()) {
        return;
    }

    WReportGrid *rg = addTab<WReportGrid>();
    FGiftCartUsage *filter = new FGiftCartUsage(rg);
    rg->addFilterWidget(filter);
    filter->fCardCode = cardCode;
    filter->ui->leCard->setText(cardLabel.isEmpty() ? cardCode : cardLabel);
    filter->apply(rg);
}

void FGiftCartUsage::onRowDoubleClick(const QList<QVariant> &row)
{
    if(row.isEmpty()) {
        return;
    }

    const int orderId = row.at(0).toInt();
    if(orderId <= 0) {
        return;
    }

    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setReadOnly(true);
    d->setOrder(QString::number(orderId));
    d->exec();
    delete d;
}
