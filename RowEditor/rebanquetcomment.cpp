#include "rebanquetcomment.h"
#include "ui_rebanquetcomment.h"
#include "wreportgrid.h"

REBanquetComment::REBanquetComment(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_BANQUET_COMMENT, parent),
    ui(new Ui::REBanquetComment)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "r_banquet_comment";
}

REBanquetComment::~REBanquetComment()
{
    delete ui;
}

void REBanquetComment::openBanquet()
{
    QList<int> widths;
    widths << 100
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_name";
    QStringList titles;
    titles << tr("Code")
           << tr("Name");
    QString title = tr("Banquet comments");
    QString icon = ":/images/cutlery.png";
    QString query = "select f_id, f_name from r_banquet_comment";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<REBanquetComment>(widths, fields, titles, title, icon, query);
}

void REBanquetComment::on_btnOk_clicked()
{
    save();
}

void REBanquetComment::on_btnCancel_clicked()
{
    reject();
}
