#include "recarclient.h"
#include "ui_recarclient.h"
#include "wreportgrid.h"
#include "database2.h"
#include "fgiftcart.h"

#define SEL_CAR 1
#define SEL_DISC_TYPE 2

RECarClient::RECarClient(QList<QVariant>& values, QWidget *parent) :
    RowEditorDialog(values, TRACK_CAR_CLIENT, parent),
    ui(new Ui::RECarClient)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leCardcode, "Card code");
    addWidget(ui->leInfo, "Info");
    addWidget(ui->lePrice, "Price");
    addWidget(ui->leFiscal, "Fiscal");
    fTable = "d_gift_cart";
    //fCacheId = 0;
}

RECarClient::~RECarClient()
{
    delete ui;
}

void RECarClient::setValues()
{
    RowEditorDialog::setValues();
    Database2 db;

    if(!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }

    db[":f_code"] = ui->leCardcode->text();
    db.exec("select count(f_id)-1 as f_count, sum(f_amount) as f_amount from d_gift_cart_use where f_code=:f_code");
    ui->leBalance->setText("0");

    if(db.next()) {
        ui->leVisits->setDouble(db.doubleValue("f_count"));
        ui->leBalance->setDouble(db.doubleValue("f_amount"));
    }
}

void RECarClient::openReport()
{
    WReportGrid *r = addTab<WReportGrid>();
    FGiftCart *f = new FGiftCart(r);
    r->addFilterWidget(f);
    f->apply(r);
}

void RECarClient::openReport1()
{
    openReport();
}

void RECarClient::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    DatabaseResult dr;
}

void RECarClient::on_btnSave_clicked()
{
    save();
}

void RECarClient::on_btnReject_clicked()
{
    reject();
}
