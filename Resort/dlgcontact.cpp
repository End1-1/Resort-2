#include "dlgcontact.h"
#include "ui_dlgcontact.h"

DlgContact::DlgContact(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values,TRACK_CONTACT, parent),
    ui(new Ui::DlgContact)
{
    ui->setupUi(this);
}

DlgContact::~DlgContact()
{
    delete ui;
}

void DlgContact::setValues()
{
    ui->leId->clear();
    ui->leName->clear();
    ui->leTel1->clear();
    ui->leTel2->clear();
    ui->leFax->clear();
    ui->leEmail->clear();
    ui->teNotes->clear();
    if (fValues.count() > 0) {
        ui->leId->setText(fValues.at(0).toString());
        ui->leName->setText(fValues.at(1).toString());
        ui->leTel1->setText(fValues.at(2).toString());
        ui->leTel2->setText(fValues.at(3).toString());
        ui->leFax->setText(fValues.at(4).toString());
        ui->leEmail->setText(fValues.at(5).toString());
        ui->teNotes->setPlainText(fValues.at(6).toString());
        isNew = false;
    }
}

void DlgContact::on_buttonBox_accepted()
{
    int code =ui->leId->text().toInt();
    QMap<QString, QVariant> v;
    v[":f_name"] = ui->leName->text();
    v[":f_tel1"] = ui->leTel1->text();
    v[":f_tel2"] = ui->leTel2->text();
    v[":f_fax"] = ui->leFax->text();
    v[":f_email"] = ui->leEmail->text();
    v[":f_notes"] = ui->teNotes->toPlainText();
    if (code == 0) {
        code = fDb.insert("f_contacts", v);
        ui->leId->setText(QString::number(code));
    } else {
        fDb.update("f_contacts", v, " where f_id=" + ui->leId->text());
    }
    fValues.clear();
    fValues << ui->leId->text()
           << ui->leName->text()
           << ui->leTel1->text()
           << ui->leTel2->text()
           << ui->leFax->text()
           << ui->leEmail->text()
           << ui->teNotes->toPlainText();
    accept();
}
