#include "restorepartner.h"
#include "ui_restorepartner.h"
#include "cachestorepartners.h"
#include "wreportgrid.h"

REStorePartner::REStorePartner(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_STORE_PARTNERS, parent),
    ui(new Ui::REStorePartner)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leTaxCode, "Tax code");
    addWidget(ui->leName, "Name");
    addWidget(ui->lePhone, "Phone");
    addWidget(ui->leComments, "Comments");
    fTable = "r_partners";
    fCacheId = cid_store_partners;
}

REStorePartner::~REStorePartner()
{
    delete ui;
}

void REStorePartner::openStorePartners()
{
    QList<int> widths;
    widths << 100
           << 100
           << 300
           << 200
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_taxcode"
           << "f_name"
           << "f_phone"
           << "f_comments";
    QStringList titles;
    titles << tr("Code")
           << tr("Tax code")
           << tr("Name")
           << tr("Phone")
           << tr("Comments");
    QString title = tr("Partners");
    QString icon = ":/images/partner.png";
    QString query = "select f_id, f_taxcode, f_name, f_phone, f_comment from r_partners";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<REStorePartner>(widths, fields, titles, title, icon, query);
}

void REStorePartner::on_btnCancel_clicked()
{
    reject();
}

void REStorePartner::on_btnSave_clicked()
{
    save();
}
