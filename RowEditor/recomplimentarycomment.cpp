#include "recomplimentarycomment.h"
#include "ui_recomplimentarycomment.h"
#include "wreportgrid.h"

REComplimentaryComment::REComplimentaryComment(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_COMPLIMENTARY_COMMENT, parent),
    ui(new Ui::REComplimentaryComment)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "r_complimentary_comment";
}

REComplimentaryComment::~REComplimentaryComment()
{
    delete ui;
}

void REComplimentaryComment::openComplimentaryComment()
{
    QList<int> widths;
    widths << 100
           << 200
              ;
    QStringList fields;
    fields << "f_id"
           << "f_name"
              ;
    QStringList titles;
    titles << tr("Code")
             << tr("Name")
                ;
    QString title = tr("Complimentary comments");
    QString icon = ":/images/gift.png";
    QString query = "select f_id, f_name from r_complimentary_comment";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<REComplimentaryComment>(widths, fields, titles, title, icon, query);
}

void REComplimentaryComment::on_btnCancel_clicked()
{
    reject();
}

void REComplimentaryComment::on_btnOk_clicked()
{
    save();
}
