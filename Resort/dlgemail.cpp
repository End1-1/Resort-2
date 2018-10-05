#include "dlgemail.h"
#include "ui_dlgemail.h"
#include "smtp.h"

DlgEmail::DlgEmail(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgEmail)
{
    ui->setupUi(this);
}

DlgEmail::~DlgEmail()
{
    delete ui;
}

void DlgEmail::on_btnSend_clicked()
{
    Smtp *s = new Smtp("end1_1@mail.ru", "PoshelHaXuy", "smtp.mail.ru", 465);
    s->sendMail("end1_1@mail.ru", ui->leTo->text(), ui->leSubject->text(), ui->ptBody->toPlainText());
}

void DlgEmail::sendDialog()
{
    DlgEmail *d = new DlgEmail(fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}
