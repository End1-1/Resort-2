#include "dlgpayment.h"
#include "ui_dlgpayment.h"
#include "rmessage.h"
#include "dlglist.h"
#include "rnumbers.h"
#include "defstore.h"
#include "cachecouponseria.h"
#include "dlgdeptholder.h"

DlgPayment::DlgPayment(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPayment)
{
    ui->setupUi(this);
    fUpdateHeader = false;
}

DlgPayment::~DlgPayment()
{
    delete ui;
}

bool DlgPayment::payment(int order)
{
    bool result;
    DlgPayment *d = new DlgPayment();
    d->fOrder = order;
    DatabaseResult dr;
    double servicevalue = 0.0;
    d->fDbBind[":f_id"] = order;
    dr.select(d->fDb, "select f_servicevalue from o_header where f_id=:f_id", d->fDbBind);
    servicevalue = dr.value("f_servicevalue").toDouble() / 100;

    d->fDbBind[":f_id"] = order;
    dr.select(d->fDb, "select f_id from o_header_payment where f_id=:f_id", d->fDbBind);

    if (dr.rowCount() == 0) {
        d->fDbBind[":f_header"] = order;
        d->fDbBind[":f_state"] = DISH_STATE_READY;
        dr.select(d->fDb, "select sum(f_total) as f_total from o_dish where f_header=:f_header and f_state=:f_state", d->fDbBind);
        if (dr.rowCount() > 0) {
            d->ui->leServiceValue->setDouble(dr.value("f_total").toDouble() * servicevalue);
            d->ui->leFinalAmount->setDouble(dr.value("f_total").toDouble() + (dr.value("f_total").toDouble() * servicevalue));
            d->ui->leCash->setDouble(dr.value("f_total").toDouble() + (dr.value("f_total").toDouble() * servicevalue));
        }
    } else {
        d->fUpdateHeader = true;
    }

    result = d->exec() == QDialog::Accepted;
    delete d;
    return result;
}

void DlgPayment::on_btnCancel_clicked()
{
    reject();
}

void DlgPayment::on_btnOk_clicked()
{
    fDbBind[":f_id"] = fOrder;
    DatabaseResult drDisc;
    drDisc.select(fDb, "select * from o_temp_disc where f_id=:f_id", fDbBind);

    if (ui->leDept->asDouble() > 0.1) {
        if (ui->leDeptHolder->fHiddenText.toInt() == 0) {
            message_error(tr("Dept holder is not defined"));
            return;
        }
    }
    fDbBind[":f_cash"] = ui->leCash->asDouble();
    fDbBind[":f_card"] = ui->leCard->asDouble();
    fDbBind[":f_coupon"] = ui->leCouponAmount->asDouble();
    fDbBind[":f_discount"] = ui->leDiscountAmount->asDouble();
    fDbBind[":f_couponSeria"] = ui->leCouponSerial->text();
    fDbBind[":f_couponNumber"] = ui->leCouponNumber->text();
    fDbBind[":f_discountCard"] = ui->leDiscount->text();
    fDbBind[":f_costumer"] = ui->leCardHolder->fHiddenText.toInt();
    fDbBind[":f_finalAmount"] = ui->leFinalAmount->asDouble();
    fDbBind[":f_debt"] = ui->leDept->asDouble();
    fDbBind[":f_debtHolder"] = ui->leDeptHolder->fHiddenText.toInt();
    if (drDisc.rowCount() > 0) {
        fDbBind[":f_discount"] = drDisc.value("f_amount");
        fDbBind[":f_costumer"] = drDisc.value("f_costumer");
    }
    if (fUpdateHeader) {
        fDb.update("o_header_payment", fDbBind, where_id(fOrder));
    } else {
        fDbBind[":f_id"] = fOrder;
        fDb.insertWithoutId("o_header_payment", fDbBind);
    }
    if (ui->leDept->asDouble() > 0) {
        fDbBind[":f_id"] = fOrder;
        fDbBind[":f_holder"] = ui->leDeptHolder->fHiddenText.toInt();
        fDbBind[":f_debt"] = ui->leDept->asDouble();
        fDb.insertWithoutId("o_debts", fDbBind);
    }
    if (ui->leCouponNumber->asInt() > 0) {
        fDbBind[":f_seria"] = ui->leCouponSerial->fHiddenText.toInt();
        fDbBind[":f_number"] = ui->leCouponNumber->asInt();
        fDb.select("update d_coupon set f_used=1 where f_seria=:f_seria and cast(f_number as signed)=:f_number", fDbBind, fDbRows);
    }

    accept();
}

void DlgPayment::on_btnCard_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, ui->leCash->asDouble(), this)) {
        if (num > ui->leFinalAmount->asInt()) {
            message_error(tr("Incorrect amount"));
            return;
        }
        ui->leCard->setInt(num);
        calcCash();
    }
}

void DlgPayment::calcCash()
{
    ui->leCash->setDouble(ui->leFinalAmount->asDouble() - ui->leCard->asDouble() - ui->leDept->asDouble() - ui->leCouponAmount->asDouble());
    if (ui->leCash->asDouble() < -0.01) {
        ui->leCash->setText(ui->leFinalAmount->text());
        ui->leCouponAmount->setText("0");
        ui->leCard->setText("0");
        ui->leDept->setText("0");
        ui->leCouponNumber->clear();
        ui->leCouponSerial->clear();
        ui->leCouponNumber->fHiddenText.clear();
        ui->leCouponSerial->fHiddenText.clear();
        ui->leDiscount->clear();
        ui->leDiscountAmount->setText("0");
        ui->leCardHolder->clear();
        ui->leCardHolder->fHiddenText.clear();
    }
}

void DlgPayment::on_btnDept_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        if (num > ui->leCash->asInt()) {
            message_error(tr("Incorrect amount"));
            return;
        }
        ui->leDept->setInt(num);
        calcCash();
    }
}

void DlgPayment::on_leDept_textChanged(const QString &arg1)
{
    ui->btnDeptHolder->setEnabled(arg1.toDouble() > 0.1);
}

void DlgPayment::on_btnDeptHolder_clicked()
{
    int debtId;
    QString debtName;
    if (!DlgDeptHolder::getHolder(debtId, debtName)) {
        return;
    }
    ui->leDeptHolder->setText(debtName);
    ui->leDeptHolder->fHiddenText = QString::number(debtId);
}

void DlgPayment::on_btnCouponNumber_clicked()
{
    if (ui->leCouponSerial->isEmpty()) {
        message_error(tr("Coupon seria is not defined"));
        return;
    }
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        ui->leCouponNumber->setText(QString::number(num, 'f', 0));
    }
    DatabaseResult dr;
    fDbBind[":f_seria"] = ui->leCouponSerial->fHiddenText.toInt();
    fDbBind[":f_number"] = ui->leCouponNumber->asInt();
    dr.select(fDb, "select f_id, f_value from d_coupon where f_seria=:f_seria and cast(f_number as signed)=:f_number and f_used=0", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Invalid coupon number"));
        ui->leCouponNumber->clear();
        return;
    }
    CI_CouponSeria *ci = CacheCouponSeria::instance()->get(ui->leCouponSerial->fHiddenText);
    if (!ci) {
        message_error(tr("Invalide coupon seria"));
        return;
    }
    ui->leCouponAmount->setText(float_str(ci->fPrice, 2));
    fCountId = dr.value("f_id").toInt();
    calcCash();
}

void DlgPayment::on_btnCouponSerail_clicked()
{
    if (ui->leCardHolder->asInt() > 0) {
        message_error(tr("Cannot use coupon with discount"));
        return;
    }
    QString name;
    QVariant res;
    if (!DlgList::getValue(tr("Coupon seria"), name, res, "select f_id, f_name from d_coupon_seria ")) {
        return;
    }
    CI_CouponSeria *c = CacheCouponSeria::instance()->get(res.toString());
    if (!c) {
        message_error(tr("Invalid coupon seria"));
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_header"] = fOrder;
    fDbBind[":f_state"] = DISH_STATE_READY;
    dr.select(fDb, "select f_dish from o_dish where f_header=:f_header and f_state=:f_state", fDbBind);
    bool found = false;
    for (int i = 0; i < dr.rowCount(); i++) {
        if (c->fItems.contains(dr.value(i, "f_dish").toString())) {
            found = true;
            break;
        }
    }
    if (!found) {
        message_error(tr("This order is not contains any items for this coupon seria"));
        return;
    }
    ui->leCouponSerial->setText(name);
    ui->leCouponSerial->fHiddenText = res.toString();
}

void DlgPayment::on_btnCash_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        if (num > ui->leFinalAmount->asInt()) {
            message_error(tr("Incorrect amount"));
            return;
        }
        ui->leCard->setInt(num);
        calcCash();
    }
}

void DlgPayment::on_btnPrintTax_clicked()
{
    exit(0);
}

void DlgPayment::on_leDiscount_returnPressed()
{

    if (ui->leCouponNumber->asInt() > 0) {
        message_error(tr("Cannot discount with coupon"));
        return;
    }
    QString code = ui->leDiscount->text().replace("?", "").replace(";", "");
    ui->leDiscount->setText(code);
    DatabaseResult dr;
    fDbBind[":f_card"] = code;
    dr.select(fDb, "select f_id, f_name, f_mode, f_model from d_car_client where f_card=:f_card", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Invalid card"));
        return;
    }
    int m = dr.value("f_model").toInt();
    ui->leCardHolder->setText(dr.value("f_name").toString());
    ui->leCardHolder->fHiddenText = dr.value("f_id").toString();
    QStringList mode = dr.value("f_mode").toString().split(";", QString::SkipEmptyParts);
    if (mode.count() == 0) {
        message_error(tr("Card params is not defined"));
        return;
    }
    DatabaseResult dri;
    fDbBind[":f_header"] = fOrder;
    fDbBind[":f_state"] = DISH_STATE_READY;
    dri.select(fDb, "select f_id, f_dish, f_qty, f_price, f_total from o_dish where f_header=:f_header and f_state=:f_state", fDbBind);
    QString visit = mode.at(1);
    QString value = mode.at(2);
    QStringList items = mode.at(3).split(",", QString::SkipEmptyParts);
    bool disc = false;
    double totalDisc = 0;
    switch (m) {
    case 1:
    case 2: {
        DatabaseResult drv;
        fDbBind[":f_costumer"] = dr.value("f_id");
        fDbBind[":f_state"] = ORDER_STATE_CLOSED;
        drv.select(fDb, "select count(h.f_id) as visits from o_header_payment h "
                   "left join o_header o on o.f_id=h.f_id where f_costumer=:f_costumer and o.f_state=:f_state", fDbBind);
        int v = drv.value("visits").toInt();
        int cur = 0;
        if (visit.toInt() > 0) {
            cur = (v + 1) % (visit.toInt());
        }
        ui->lbVisit->setText(QString::number(cur));
        if ((dr.value("visits").toInt() > 0) && (cur == 0)) {
            disc = true;
        }
        if (disc) {
        for (int i = 0; i < dri.rowCount(); i++) {
                if (items.contains(dri.value(i, "f_dish").toString(), Qt::CaseInsensitive) || items.at(0) == "*") {
                    double newPrice = dri.value(i, "f_price").toDouble() - (dri.value(i, "f_price").toDouble() * (value.toDouble() / 100));
                    totalDisc += dri.value(i, "f_total").toDouble() - (dri.value(i, "f_qty").toDouble() * newPrice);
                    fDbBind[":f_price"] = newPrice;
                    fDbBind[":f_total"] = newPrice * dri.value(i, "f_qty").toDouble();
                    //fDbBind[":f_totalUSD"] = newPrice * def_usd;
                    fDb.update("o_dish", fDbBind, where_id(ap(dri.value(i, "f_id").toString())));
                }
            }
        }
        break;
    }
    case 3: {
        if (value.toDouble() < 0.001) {
            disc = false;
            return;
        }
        disc = true;
        double balance = value.toDouble();
        if (disc) {
            for (int i = 0; i < dri.rowCount(); i++) {
                if (balance < 0.01) {
                    continue;
                }
                if (items.contains(dri.value(i, "f_dish").toString(), Qt::CaseInsensitive) || items.at(0) == "*") {
                    double itemTotal = dri.value(i, "f_qty").toDouble() * dri.value(i, "f_price").toDouble();
                    if (balance >= itemTotal) {
                        ;balance -= itemTotal;
                        totalDisc += itemTotal;
                        itemTotal = 0;
                    } else {
                        itemTotal -= balance;
                        totalDisc += balance;
                        balance = 0;
                    }
                    fDbBind[":f_price"] = itemTotal / dri.value(i, "f_qty").toDouble();
                    fDbBind[":f_total"] = itemTotal;
                    //fDbBind[":f_totalUSD"] = newPrice * def_usd;
                    fDb.update("o_dish", fDbBind, where_id(ap(dri.value(i, "f_id").toString())));
                }
            }
            if (balance > 0.001) {
                if (ui->leServiceValue->asDouble() < balance) {
                    balance -= ui->leServiceValue->asDouble();
                    totalDisc += ui->leServiceValue->asDouble();
                } else {
                    balance = 0;
                }
            }
            fDbBind[":f_mode"] = QString("%1;%2;%3;%4;").arg("1").arg("0").arg(QString::number(balance,'f', 0)).arg(items.at(0));
            fDb.update("d_car_client", fDbBind, where_id(dr.value(0).toInt()));
        }
    }
        break;
    }
    if (!disc) {
        return;
    } else {

            DatabaseResult drDisc;
            fDbBind[":f_id"] = fOrder;
            drDisc.select(fDb, "select * from o_temp_disc where f_id=:f_id", fDbBind);
            if (drDisc.rowCount() > 0) {
                ui->leDiscount->clear();
                message_error(tr("Discount already used"));
                return;
            }
    }

    fDbBind[":f_total"] = totalDisc;
    fDbBind[":f_id"] = fOrder;
    fDb.select("update o_header set f_total=f_total-:f_total where f_id=:f_id", fDbBind, fDbRows);
    ui->leFinalAmount->setDouble(ui->leFinalAmount->asDouble() - totalDisc);
    ui->leCash->setText(ui->leFinalAmount->text());
    ui->leCouponAmount->setText("0");
    ui->leCard->setText("0");
    ui->leCouponNumber->clear();
    ui->leCouponSerial->clear();
    ui->leCouponSerial->fHiddenText.clear();
    ui->leDiscountAmount->setDouble(totalDisc);
}
