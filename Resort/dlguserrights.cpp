#include "dlguserrights.h"
#include "ui_dlguserrights.h"
#include "database2.h"



DlgUserRights::DlgUserRights(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgUserRights)
{
    ui->setupUi(this);
}

DlgUserRights::~DlgUserRights()
{
    delete ui;
}

void DlgUserRights::setup(QList<QVariant> &values)
{
    fGroupId = values.at(0).toInt();
    fDbBind[":f_group"] = fGroupId;
    fDb.select("select f_right, f_flag from users_rights where f_group=:f_group", fDbBind, fDbRows);
    foreach_rows {
        check(this, it->at(1).toInt() == 1, it->at(0).toInt());
    }
}

void DlgUserRights::on_btnCancel_clicked()
{
    reject();
}

void DlgUserRights::on_btnOk_clicked()
{
    fDb.fDb.transaction();
    fDbBind[":f_group"] = fGroupId;
    fDb.select("delete from users_rights where f_group=:f_group", fDbBind, fDbRows);
    QString query = "insert into users_rights (f_group, f_right, f_flag) values ";
    bool first = true;
    checkQuery(this, query, first);
    fDb.queryDirect(query);
    fDb.fDb.commit();
    accept();
}

void DlgUserRights::on_chCheckUncheck_clicked(bool checked)
{
    checkAll(this, checked);
}

void DlgUserRights::checkAll(QObject *parent, bool checked)
{
    QObjectList ol = parent->children();
    for (QObject *o: ol) {
        if (o->children().count() > 0) {
            checkAll(o, checked);
        }
        QCheckBox *c = dynamic_cast<QCheckBox*>(o);
        if (c) {
            c->setChecked(checked);
        }
    }
}

void DlgUserRights::check(QObject *parent, bool checked, int tag)
{
    QObjectList ol = parent->children();
    for (QObject *o: ol) {
        if (o->children().count() > 0) {
            check(o, checked, tag);
        }
        QCheckBox *c = dynamic_cast<QCheckBox*>(o);
        if (c) {
            if (c->property("tag").toInt() == tag) {
                c->setChecked(checked);
                return;
            }
        }
    }
}

void DlgUserRights::checkQuery(QObject *parent, QString &query, bool &first)
{
    QObjectList ol = parent->children();
    for (QObject *o: ol) {
        if (o->children().count() > 0) {
            checkQuery(o, query, first);
        }
        QCheckBox *c = dynamic_cast<QCheckBox*>(o);
        if (c) {
            if (c->property("tag").toInt() > 0) {
                if (first) {
                    first = false;
                } else {
                    query += ",";
                }
                query += QString("(%1, %2, %3)")
                        .arg(fGroupId)
                        .arg(c->property("tag").toInt())
                        .arg(c->isChecked() ? "1" : "0");
            }
        }
    }
}


