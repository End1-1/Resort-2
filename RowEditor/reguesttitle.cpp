#include "reguesttitle.h"
#include "ui_reguesttitle.h"
#include "cacheguesttitle.h"
#include "wreportgrid.h"

REGuestTitle::REGuestTitle(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_GUEST_TITLE, parent),
    ui(new Ui::REGuestTitle)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "f_guests_title";
    fCacheId = cid_guest_title;
}

REGuestTitle::~REGuestTitle()
{
    delete ui;
}

void REGuestTitle::openEditor()
{
    QList<int> widths;
    widths << 100
           << 100;
    QStringList fields;
    fields << "f_id"
           << "f_en";
    QStringList titles;
    titles << tr("Code")
           << tr("Name");
    QString title = tr("Partners groups");
    QString icon = ":/images/guests.png";
    QString query = "select f_id, f_en from f_guests_title";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<REGuestTitle>(widths, fields, titles, title, icon, query);
}

void REGuestTitle::on_btnCancel_clicked()
{
    reject();
}

void REGuestTitle::on_btnSave_clicked()
{
    save();
}
