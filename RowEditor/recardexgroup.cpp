#include "recardexgroup.h"
#include "ui_recardexgroup.h"
#include "cachecardexgroup.h"
#include "wreportgrid.h"

RECardexGroup::RECardexGroup(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_MARKET_SIGMENT, parent),
    ui(new Ui::RECardexGroup)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    addWidget(ui->leCounter, "Counter");
    addWidget(ui->leDescription, "Description");
    ui->leCounter->setReadOnly(!r__(cr__super_correction));
    fTable = "f_cardex_group";
    fCacheId = cid_cardex_group;
}

RECardexGroup::~RECardexGroup()
{
    delete ui;
}

void RECardexGroup::openEditor()
{
    QList<int> widths;
    widths << 100
           << 100
           << 100
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_code"
           << "f_max"
           << "f_description";
    QStringList titles;
    titles << tr("Code")
           << tr("Name")
           << tr("Counter")
           << tr("Description");
    QString title = tr("Partners groups");
    QString icon = ":/images/partner.png";
    QString query = "select f_id, f_code, f_max, f_description from f_cardex_group";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RECardexGroup>(widths, fields, titles, title, icon, query);
}

void RECardexGroup::on_btnCancel_clicked()
{
    reject();
}

void RECardexGroup::on_btnSave_clicked()
{
    save();
}
