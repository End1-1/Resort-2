#include "recarclient.h"
#include "ui_recarclient.h"
#include "wreportgrid.h"
#include "dlggposorderinfo.h"
#include "database2.h"

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
    QList<int> widths;
    widths << 0
           << 100
           << 300
           << 100
           << 100
           << 150
           << 150
           << 150
           ;
    QStringList fields;
    fields << "f_id"
           << "f_code"
           << "f_num"
           << "f_info"
           << "f_initialamount"
           << "f_fiscal"
           << "f_spent"
           << "f_status";
    QStringList titles;
    titles << tr("Code")
           << "Կոդ"
           << "Համար"
           << "Մեկնաբանություն"
           << "Գումար"
           << "ՀԴՄ"
           << "Սպառում"
           << "Վիճակ";
    QString title = "Նվեր քարտեր";
    QString icon = ":/images/car.png";
    //QString query = "select c.f_id, c.f_code, c.f_info, c.f_initialamount, c.f_fiscal from d_gift_cart c ";
    QString query = R"(
SELECT c.f_id, c.f_code, right(c.f_code,4) as f_num, c.f_info, f_initialamount, f_fiscal, u.f_spent,
if(c.f_fiscal=-88, 'Հին անհայտ', if (f_initialamount=f_spent AND f_fiscal IS null, 'Չվաճառված',
if (f_spent=0, 'Սպառված', if(f_fiscal IS NOT NULL,  'Վաճառված', 'Ոչ ակտիվ'))))
FROM d_gift_cart c
LEFT JOIN (SELECT f_code, SUM(f_amount) AS f_spent FROM d_gift_cart_use GROUP BY 1) u ON u.f_code=c.f_code
)";
    WReportGrid *r = addTab<WReportGrid>();
    RECarClient *rc = r->fullSetup<RECarClient>(widths, fields, titles, title, icon, query);
    connect(r, SIGNAL(doubleClickOnRow(QList<QVariant>())), rc, SLOT(doubleClickOnRow(QList<QVariant>())));
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

void RECarClient::doubleClickOnRow(const QList<QVariant>& v)
{
    if(v.count() == 0) {
        return;
    }

    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(v.at(0).toString());
    d->exec();
    delete d;
}
