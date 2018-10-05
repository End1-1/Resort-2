#include "rlogin.h"
#include "ui_rlogin.h"

RLogin::RLogin(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RLogin)
{
    ui->setupUi(this);
}

RLogin::~RLogin()
{
    delete ui;
}

bool RLogin::getLogin(QString &login, const QString &title, QWidget *parent)
{
    bool result = false;
    RLogin *l = new RLogin(parent);
    l->ui->lbWhy->setText(title);
    if (l->exec() == QDialog::Accepted) {
        login =l->ui->lePass->text();
        if (login.length() > 0)
            result = true;
    }
    delete l;
    return result;
}

void RLogin::returnPressed()
{
    on_btnOk_clicked();
}

void RLogin::escapePressed()
{
    reject();
}

void RLogin::numberButtonClicked()
{
    QPushButton *b = static_cast<QPushButton*>(sender());
    QString text = b->text();
    ui->lePass->setText(ui->lePass->text() + text);
}

void RLogin::on_btnOk_clicked()
{
    accept();
}

void RLogin::on_btnCancel_clicked()
{
    reject();
}

void RLogin::on_btnClear_clicked()
{
    ui->lePass->clear();
}
