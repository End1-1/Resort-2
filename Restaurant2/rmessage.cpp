#include "rmessage.h"
#include "ui_rmessage.h"

#define html QString("<html><body style=\"background-color:%1\">" \
    "<p align=\"center\"><img src=\"%2\" /></p>" \
    "<p style=\"font-size:18px;color:%3;\" align=\"center\">%4</p></body></html>")

RMessage::RMessage(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RMessage)
{
    ui->setupUi(this);
    ui->btnCancel_2->setVisible(false);
}

RMessage::~RMessage()
{
    delete ui;
}

void RMessage::showError(const QString &message, QWidget *parent)
{
    RMessage *m = new RMessage(parent);
    QString htmlStr = html.arg("#333e46").arg(":/images/error.png").arg("red").arg(message);
    m->ui->teMessage->setHtml(htmlStr);
    m->ui->btnYES->setVisible(false);
    m->ui->btnCancel->setVisible(false);
    m->exec();
    delete m;
}

bool RMessage::showQuestion(const QString &message, QWidget *parent)
{
    bool result = false;
    RMessage *m = new RMessage(parent);
    QString htmlStr = html.arg("darkblue").arg(":/images/info.png").arg("white").arg(message);
    m->ui->teMessage->setHtml(htmlStr);
    m->ui->btnOK->setVisible(false);
    result = m->exec() == QDialog::Accepted;
    delete m;
    return result;
}

void RMessage::showInfo(const QString &message, QWidget *parent)
{
    RMessage *m = new RMessage(parent);
    QString htmlStr = html.arg("darkblue").arg(":/images/info.png").arg("white").arg(message);
    m->ui->teMessage->setHtml(htmlStr);
    m->ui->btnYES->setVisible(false);
    m->ui->btnCancel->setVisible(false);
    m->exec();
    delete m;
}

int RMessage::showYesNoCancel(const QString &message, QWidget *parent)
{
    RMessage *m = new RMessage(parent);
    QString htmlStr = html.arg("darkblue").arg(":/images/info.png").arg("white").arg(message);
    m->ui->teMessage->setHtml(htmlStr);
    m->ui->btnYES->setVisible(true);
    m->ui->btnOK->setVisible(false);
    m->ui->btnCancel_2->setVisible(true);
    int result = m->exec();
    delete m;
    return result;
}

void RMessage::on_btnCancel_clicked()
{
    done(RESULT_NO);
}

void RMessage::on_btnYES_clicked()
{
    done(RESULT_YES);
}

void RMessage::on_btnOK_clicked()
{
    accept();
}

void RMessage::on_btnCancel_2_clicked()
{
    done(RESULT_CANCEL);
}
