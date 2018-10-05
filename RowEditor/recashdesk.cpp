#include "recashdesk.h"
#include "ui_recashdesk.h"
#include "cachecashdesk.h"
#include "wreportgrid.h"

RECashDesk::RECashDesk(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_CASH_DESK, parent),
    ui(new Ui::RECashDesk)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    addWidget(ui->leComment, "Comment");
    fTable = "c_cash_names";
    fCacheId = cid_cash_desk;
}

RECashDesk::~RECashDesk()
{
    delete ui;
}

void RECashDesk::openReport()
{
    QList<int> widths;
    widths << 100
           << 300
           << 150
              ;
    QStringList fields;
    fields << "f_id"
           << "f_name"
           << "f_comment"
              ;
    QStringList titles;
    titles << tr("Code")
           << tr("Name")
           << tr("Comment")
              ;
    QString title = tr("Partners");
    QString icon = ":/images/discount.png";
    QString query = "select f_id, f_name, f_comment from c_cash_names";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RECashDesk>(widths, fields, titles, title, icon, query);
}

void RECashDesk::on_btnCancel_clicked()
{
    reject();
}

void RECashDesk::on_btnSave_clicked()
{
    save();
}
