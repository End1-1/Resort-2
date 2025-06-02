#include "dlgpassword.h"
#include "ui_dlgpassword.h"
#include <QCryptographicHash>
#include <QDoubleValidator>

DlgPassword::DlgPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPassword)
{
    ui->setupUi(this);
    fMax = 0;
}

DlgPassword::~DlgPassword()
{
    delete ui;
}

bool DlgPassword::getQty(const QString &title, double& qty, double& min)
{
    DlgPassword *d = new DlgPassword(0);
    d->fMax = qty;
    d->fMin = min;
    d->ui->label->setText(title);
    d->ui->lePassword->setEchoMode(QLineEdit::Normal);
    d->ui->lePassword->setMaxLength(2);
    bool result = d->exec() == QDialog::Accepted;
    qty = d->ui->lePassword->text().toDouble();
    delete d;
    return result;
}

void DlgPassword::on_pushButton_clicked()
{
    click("1");
}

void DlgPassword::click(const QString &text)
{
    if(text == ".") {
        if(ui->lePassword->text().contains(".")) {
            return;
        }
    }

    ui->lePassword->setText(ui->lePassword->text() + text);
}

void DlgPassword::on_pushButton_2_clicked()
{
    click("2");
}

void DlgPassword::on_pushButton_3_clicked()
{
    click("3");
}

void DlgPassword::on_pushButton_4_clicked()
{
    click("4");
}

void DlgPassword::on_pushButton_5_clicked()
{
    click("5");
}

void DlgPassword::on_pushButton_6_clicked()
{
    click("6");
}

void DlgPassword::on_pushButton_7_clicked()
{
    click("7");
}

void DlgPassword::on_pushButton_8_clicked()
{
    click("8");
}

void DlgPassword::on_pushButton_9_clicked()
{
    click("9");
}

void DlgPassword::on_pushButton_10_clicked()
{
    click("0");
}

void DlgPassword::on_pushButton_11_clicked()
{
    reject();
}

void DlgPassword::on_pushButton_12_clicked()
{
    if(ui->lePassword->text().toDouble()  < fMin) {
        return;
    }

    accept();
}

void DlgPassword::on_lePassword_returnPressed()
{
    on_pushButton_12_clicked();
}

void DlgPassword::on_lePassword_textChanged(const QString &arg1)
{
    if(fMax > 0.01) {
        if(arg1.toDouble() > fMax) {
            ui->lePassword->setText(QString::number(fMax, 'f',
                                                    2).remove(QRegularExpression("\\.0+$")).remove(QRegularExpression("\\.$")));
        }
    }
}

void DlgPassword::on_btnClear_clicked()
{
    ui->lePassword->clear();
}

void DlgPassword::on_pushButton_13_clicked()
{
    if(!ui->lePassword->text().contains(".")) {
        click(".");
    }
}
