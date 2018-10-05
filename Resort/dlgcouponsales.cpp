#include "dlgcouponsales.h"
#include "ui_dlgcouponsales.h"
#include "dwselectorstorepartners.h"
#include "dwselectorcouponseria.h"

#define SEL_PARTNER 1
#define SEL_COUPON_SERIA 2

DlgCouponSales::DlgCouponSales(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgCouponSales)
{
    ui->setupUi(this);
    DWSelectorStorePartners *dockPartner = new DWSelectorStorePartners(this);
    dockPartner->configure();
    dockPartner->setSelector(ui->lePartnerCode);
    dockPartner->setDialog(this, SEL_PARTNER);
    DWSelectorCouponSeria *dockSeria = new DWSelectorCouponSeria(this);
    dockSeria->configure();
    dockSeria->setSelector(ui->leSeriaNum);
    dockSeria->setDialog(this, SEL_COUPON_SERIA);
}

DlgCouponSales::~DlgCouponSales()
{
    delete ui;
}

void DlgCouponSales::sale()
{
    DlgCouponSales *d = new DlgCouponSales(fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}

void DlgCouponSales::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_PARTNER: {
        dockResponse<CI_StorePartners, CacheStorePartners>(ui->lePartnerCode, ui->lePartnerName, value.value<CI_StorePartners*>());
        break;
    }
    case SEL_COUPON_SERIA: {
        CI_CouponSeria *ci = value.value<CI_CouponSeria*>();
        dockResponse<CI_CouponSeria, CacheCouponSeria>(ui->leSeriaNum, ui->leSeraiName, ci);
        on_leQty_textChanged(ui->leQty->text());
        break;
    }
    }
}

void DlgCouponSales::on_btnCancel_clicked()
{
    reject();
}

void DlgCouponSales::on_btnSave_clicked()
{
    if (ui->leSeriaNum->asInt() == 0) {
        message_error(tr("Coupon seria is not defined"));
        return;
    }
    if (ui->leQty->asInt() == 0) {
        message_error(tr("Coupon count must be greater than 0"));
        return;
    }
    if (ui->lePartnerCode->asInt() == 0) {
        message_error(tr("Partner is not defined"));
        return;
    }

    DatabaseResult dr;
    fDbBind[":f_seria"] = ui->leSeriaNum->asInt();
    dr.select(fDb, "select f_id from d_coupon where f_seria=:f_seria and (f_partner=0 or f_partner is null) and f_used=0", fDbBind);
    if (dr.rowCount() < ui->leQty->asInt()) {
        message_error(tr("No enough coupons avaiable"));
        return;
    }

    if (ui->leCode->asInt() == 0) {
        fDbBind[":f_id"] = 0;
        ui->leCode->setInt(fDb.insert("d_coupon_sale", fDbBind));
    } else {
        message_error(tr("This sale cannot be saved"));
        return;
    }

    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_id"] = dr.value(i, "f_id");
        fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
        fDb.select("update d_coupon set f_partner=:f_partner where f_id=:f_id", fDbBind, fDbRows);
        fDbBind[":f_doc"] = ui->leCode->asInt();
        fDbBind[":f_coupon"] = dr.value(i, "f_id");
        fDb.insert("d_coupon_sale_member", fDbBind);
        if (i == ui->leQty->asInt() - 1) {
            break;
        }
    }

    fDbBind[":f_date"] = ui->deDate->date();
    fDbBind[":f_partner"] = ui->lePartnerCode->asInt();
    fDbBind[":f_payment"] = ui->cbPayment->currentData();
    fDbBind[":f_seria"] = ui->leSeriaNum->asInt();
    fDbBind[":f_qty"] = ui->leQty->asInt();
    fDbBind[":f_amount"] = ui->leAmount->asDouble();
    fDb.update("d_coupon_sale", fDbBind, where_id(ui->leCode->asInt()));

    message_info_tr("Saved");
}

void DlgCouponSales::on_leQty_textChanged(const QString &arg1)
{
    CI_CouponSeria *ci = CacheCouponSeria::instance()->get(ui->leSeriaNum->text());
    if (!ci) {
        ui->leAmount->setDouble(0);
        return;
    }
    ui->leAmount->setDouble(arg1.toDouble() * ci->fPrice);
}
