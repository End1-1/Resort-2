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

bool RNumbers::getNumber(float &number, float max, QWidget *parent)
{
    bool result = false;
    RNumbers *n = new RNumbers(parent);
    n->ui->leText->setValidator(new QDoubleValidator(-999999, 999999, 1));
    n->fMax = max;
    if (n->exec() == QDialog::Accepted) {
        number = str_float(n->ui->leText->text());
        if (number > 0) {
            result = true;
        }
    }
    delete n;
    return result;
}

bool RNumbers::getString(QString &str, QWidget *parent)
{
    bool result = false;
    RNumbers *n = new RNumbers(parent);
    n->fMax = -1000;
    if (n->exec() == QDialog::Accepted) {
        str = n->ui->leText->text();
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
    if (str_float(ui->leText->text()) < 0.01) {
        message_error(tr("Quantity must be greater than 0"));
        return;
    }
    accept();
}
