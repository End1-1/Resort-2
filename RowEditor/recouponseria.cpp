#include "recouponseria.h"
#include "ui_recouponseria.h"
#include "cachecouponseria.h"
#include "wreportgrid.h"

RECouponSeria::RECouponSeria(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_COUPON_SERIA, parent),
    ui(new Ui::RECouponSeria)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    addWidget(ui->lePrice, "Price");
    addWidget(ui->leItems, "Items");
    fTable = "d_coupon_seria";
    fCacheId = cid_coupon_seria;
}

RECouponSeria::~RECouponSeria()
{
    delete ui;
}

void RECouponSeria::openReport()
{
    QList<int> widths;
    widths << 100
           << 300
           << 150
           << 200
              ;
    QStringList fields;
    fields << "f_id"
           << "f_name"
           << "f_price"
           << "f_items"
              ;
    QStringList titles;
    titles << tr("Code")
           << tr("Name")
           << tr("Price")
           << tr("Items")
              ;
    QString title = tr("Partners");
    QString icon = ":/images/discount.png";
    QString query = "select f_id, f_name, f_price, f_items from d_coupon_seria";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RECouponSeria>(widths, fields, titles, title, icon, query);
}

void RECouponSeria::on_btnCancel_clicked()
{
    reject();
}

void RECouponSeria::on_btnSave_clicked()
{
    save();
}
