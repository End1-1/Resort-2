#include "rnumbers.h"
#include "ui_rnumbers.h"
#include "rmessage.h"

RNumbers::RNumbers(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RNumbers)
{
    ui->setupUi(this);

}

RNumbers::~RNumbers()
{
    delete ui;
}

bool RNumbers::getFloat(float &number, float max, const QString &title, QWidget *parent)
{
    bool result = false;
    RNumbers *n = new RNumbers(parent);
    n->ui->leText->setValidator(new QDoubleValidator(-999999, 999999, 1));
    n->fMax = max;
    n->ui->label->setText(title);
    n->ui->label->setVisible(title.isEmpty() == false);
    if (n->exec() == QDialog::Accepted) {
        number = str_float(n->ui->leText->text());
        if (number > 0) {
            result = true;
        }
    }
    delete n;
    return result;
}

bool RNumbers::getInt(int &num, const QString &title, QWidget *parent)
{
    bool result = false;
    RNumbers *n = new RNumbers(parent);
    n->ui->leText->setValidator(new QIntValidator());
    n->fMax = 99999999;
    n->ui->label->setText(title);
    n->ui->label->setVisible(title.isEmpty() == false);
    if (n->exec() == QDialog::Accepted) {
        num = n->ui->leText->text().toInt();
        if (num > 0) {
            result = true;
        }
    }
    delete n;
    return result;
}

bool RNumbers::getString(QString &str, const QString &title, QWidget *parent)
{
    bool result = false;
    RNumbers *n = new RNumbers(parent);
    n->fMax = -1000;
    n->ui->label->setText(title);
    n->ui->label->setVisible(title.isEmpty() == false);
    if (n->exec() == QDialog::Accepted) {
        str = n->ui->leText->text().replace(";", "").replace("?", "");
        result = !str.isEmpty();
    }
    delete n;
    return result;
}

bool RNumbers::getPassword(QString &str, const QString &title, QWidget *parent)
{
    bool result = false;
    RNumbers *n = new RNumbers(parent);
    n->fMax = -1000;
    n->ui->leText->setEchoMode(QLineEdit::Password);
    n->ui->label->setText(title);
    n->ui->label->setVisible(title.isEmpty() == false);
    if (n->exec() == QDialog::Accepted) {
        str = n->ui->leText->text().replace(";", "").replace("?", "");
        result = !str.isEmpty();
    }
    delete n;
    return result;
}

void RNumbers::btnNumClick()
{
    QPushButton *b = static_cast<QPushButton*>(sender());
    if (b->text() == "0" && fMax != -1000) {
        if (ui->leText->text().toFloat() < 0.01) {
            return;
        }
    }
    ui->leText->setText(ui->leText->text().append(b->text()));
    if (fMax > 0.01) {
        if (ui->leText->text().toFloat() > fMax) {
            ui->leText->setText(float_str(fMax, 1));
        }
    }
}

void RNumbers::on_btnClear_clicked()
{
    ui->leText->setText(ui->leText->text().remove(ui->leText->text().length() - 1, 1));
}

void RNumbers::on_btnReject_clicked()
{
    reject();
}

void RNumbers::on_btnAccept_clicked()
{
    if (fMax != -1000) {
        if (str_float(ui->leText->text()) < 0.01) {
            message_error(tr("Quantity must be greater than 0"));
            return;
        }
    }
    accept();
}

void RNumbers::on_leText_returnPressed()
{
    on_btnAccept_clicked();
}
