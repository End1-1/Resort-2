#include "recarclient.h"
#include "ui_recarclient.h"
#include "wreportgrid.h"
#include "dwselectorcar.h"
#include "dlggposorderinfo.h"
#include "dwselectordiscounttype.h"

#define SEL_CAR 1
#define SEL_DISC_TYPE 2

RECarClient::RECarClient(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_CAR_CLIENT, parent),
    ui(new Ui::RECarClient)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leDiscTypeCode, "Model");
    addWidget(ui->leCargovnumber, "Gov. number");
    addWidget(ui->leCostumer, "Costumer");
    addWidget(ui->leInfo, "Info");
    addWidget(ui->leCardcode, "Card code");
    fTable = "d_car_client";
    //fCacheId = 0;

    DWSelectorDiscountType *d = new DWSelectorDiscountType(this);
    d->configure();
    d->setSelector(ui->leDiscTypeCode);
    d->setDialog(this, SEL_DISC_TYPE);
}

RECarClient::~RECarClient()
{
    delete ui;
}

void RECarClient::selector(int number, const QVariant &value)
{
    switch (number) {
    case SEL_CAR:
        break;
    case SEL_DISC_TYPE: {
        dockResponse<CI_DiscountType, CacheDiscountType>(ui->leDiscTypeCode, ui->leDiscTypeName, value.value<CI_DiscountType*>());
        break;
    }
    }
}

void RECarClient::openReport(const QString &num)
{
    QList<int> widths;
    widths << 100
           << 200
           << 100
           << 300
           << 300
           << 150;
    QStringList fields;
    fields << "f_id"
           << "f_model"
           << "dname"
           << "f_name"
           << "f_info"
           << "f_card";
    QStringList titles;
    titles << tr("Code")
           << tr("Model")
           << tr("Name")
           << tr("Name")
           << tr("Info")
           << tr("Card code");
    QString title = tr("Costumers");
    switch (num.toInt()) {
    case 1:
        title = tr("Costumers, dep");
        break;
    case 2:
        title = tr("Costumers, one");
        break;
    case 3:
        title = tr("Costumers, presend");
        break;
    }
    QString icon = ":/images/car.png";
    QString query = "select c.f_id, c.f_model, d.f_name as dname, c.f_name, c.f_info, c.f_card from d_car_client c "
            "left join f_discount_type d on d.f_id=c.f_model where c.f_model= " + num;
    WReportGrid *r = addTab<WReportGrid>();
    RECarClient *rc = r->fullSetup<RECarClient>(widths, fields, titles, title, icon, query);
    connect(r, SIGNAL(doubleClickOnRow(QList<QVariant>())), rc, SLOT(dobleClickOnRow(QList<QVariant>())));
}

void RECarClient::openReport1()
{
    openReport("1");
}

void RECarClient::openReport2()
{
    openReport("2");
}

void RECarClient::openReport3()
{
    openReport("3");
}

void RECarClient::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    dockResponse<CI_Car, CacheCar>(ui->leDiscTypeCode, ui->leDiscTypeName, 0);
    DatabaseResult dr;
    fDbBind[":f_id"] = ui->leCode->text();
    dr.select(fDb, "select f_mode from d_car_client where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        QStringList s = dr.value("f_mode").toString().split(";");
        switch(s.at(0).toInt()) {
        case 1:
            ui->rbMode1->setChecked(true);
            break;
        case 2:
            ui->rbMode2->setChecked(true);
            break;
        case 3:
            ui->rbMode3->setChecked(true);
            break;
        }
        ui->leVisits->setText(s.at(1));
        ui->leValue->setText(s.at(2));
        ui->leItems->setText(s.at(3));
    }
}

void RECarClient::on_btnSave_clicked()
{
    QString mode;
    if (ui->rbMode1->isChecked()) {
        mode = "1;";
    }  else if (ui->rbMode2->isChecked()) {
        mode = "2;";
    } else if (ui->rbMode3->isChecked()) {
        mode = "3;";
    } else {
        message_error(tr("Mode of discount was not selected"));
        return;
    }
    if (ui->leDiscTypeCode->asInt() == 0) {
        message_error(tr("Discount type was not selected"));
        return;
    }
    if (ui->leItems->isEmpty()) {
        ui->leItems->setText("*");
    }
    if (ui->leValue->isEmpty()) {
        ui->leValue->setText("0");
    }
    mode += ui->leVisits->text() + ";";
    mode += ui->leValue->text() + ";";
    mode += ui->leItems->text() + ";";

    saveOnly();
    fDbBind[":f_mode"] = mode;
    fDb.update("d_car_client", fDbBind, where_id(ui->leCode->asInt()));
    message_info(tr("Saved"));
}

void RECarClient::on_btnReject_clicked()
{
    reject();
}

void RECarClient::doubleClickOnRow(const QList<QVariant> &v)
{
    if (v.count() == 0) {
        return;
    }
    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(v.at(0).toString());
    d->exec();
    delete d;
}
