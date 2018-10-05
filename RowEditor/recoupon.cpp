#include "recoupon.h"
#include "ui_recoupon.h"
#include "cachecoupontype.h"
#include "wreportgrid.h"
#include "dwselectorcouponseria.h"

#define SEL_COUPON_SERIA 1

RECoupon::RECoupon(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_COUPONS, parent),
    ui(new Ui::RECoupon)
{
    ui->setupUi(this);
    DWSelectorCouponSeria *dockSeria = new DWSelectorCouponSeria(this);
    dockSeria->configure();
    dockSeria->setSelector(ui->leSerial);
    dockSeria->setDialog(this, SEL_COUPON_SERIA);
}

RECoupon::~RECoupon()
{
    delete ui;
}

void RECoupon::openReport()
{
    QList<int> widths;
    widths << 100
           << 250
           << 100
           << 100
           << 120
           << 120
           << 120
           << 120
           << 200
           << 80;
    QStringList fields;
    fields << "f_id"
           << "f_name"
           << "f_type"
           << "f_state"
           << "f_dateRealese"
           << "f_dateEnd"
           << "f_serial"
           << "f_number"
           << "f_value"
           << "f_used"
              ;
    QStringList titles;
    titles << tr("Code")
           << tr("Destination")
           << tr("Type")
           << tr("State")
           << tr("Realesed")
           << tr("Valid to")
           << tr("Serial")
           << tr("Number")
           << tr("Value")
           << tr("Used");
    QString title = tr("Coupons");
    QString icon = ":/images/discount.png";
    QString query = "select c.f_id, sp.f_name, ct.f_name, cs.f_name, c.f_daterelease, c.f_dateEnd, cps.f_name as f_seria, c.f_number, c.f_value, f_used "
            "from d_coupon c "
            "left join r_partners sp on c.f_partner=sp.f_id "
            "left join d_coupon_seria cps on cps.f_id=c.f_seria "
            "left join d_type ct on ct.f_id=c.f_type "
            "left join d_state cs on cs.f_id=c.f_state ";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RECoupon>(widths, fields, titles, title, icon, query);
}

void RECoupon::selector(int selectorNumber, const QVariant &value)
{
    switch(selectorNumber) {
    case SEL_COUPON_SERIA: {
        CI_CouponSeria *ci = value.value<CI_CouponSeria*>();
        dockResponse<CI_CouponSeria, CacheCouponSeria>(ui->leSerial, ui->leSeriaName, ci);
        if (ci) {
            ui->leValue->setDouble(ci->fPrice);
        }
        break;
    }
    }
}

void RECoupon::on_btnCancel_clicked()
{
    reject();
}

void RECoupon::on_btnSave_clicked()
{
    if (ui->leSerial->isEmpty()) {
        message_error(tr("The serial cannot be empty"));
        return;
    }
    if (ui->leEnd->asInt() < 1) {
        message_error(tr("The end number cannot be less the 1"));
        return;
    }
    if (ui->leStart->asInt() > ui->leEnd->asInt()) {
        message_error(tr("The start number is biffer then end number"));
        return;
    }
    if (ui->leValue->asDouble() < 0.1) {
        message_error(tr("Amount for coupons is not defined"));
        return;
    }
    int strlen = 0, max = ui->leEnd->asInt();
    do {
        strlen++;
        max /= 10;
    } while (max > 0);

    for (int i = ui->leStart->asInt(); i != ui->leEnd->asInt(); i++) {
        fDbBind[":f_partner"] = 0;
        fDbBind[":f_dateRelease"] = QDate::currentDate();
        fDbBind[":f_dateEnd"] = ui->deValid->date();
        fDbBind[":f_seria"] = ui->leSerial->text();
        fDbBind[":f_number"] = QString("%1").arg(i, strlen, 10, QChar('0'));
        fDbBind[":f_type"] = ui->cbType->currentData();
        fDbBind[":f_state"] = COUPON_STATE_NEW;
        fDbBind[":f_value"] = ui->leValue->text();
        fDb.insert("d_coupon", fDbBind);
    }

    for (int i = 0; i < 9; i++) {
        fValues << "";
    }
    message_info(tr("Saved"));
    accept();
}
