#include "message.h"
#include "ui_message.h"
#include "preferences.h"
#include <QClipboard>
#include <QTextDocument>

#define html QString("<html><body style=\"background-color:%1\"><p align=\"center\"><img src=\"%2\"/></p>"\
    "<p align=\"center\" style=\"font-size:%3px; color:%4;\">%5</p></body></html>")

message::message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::message)
{
    ui->setupUi(this);
    ui->btnYes->setVisible(false);
    ui->btnNo->setVisible(false);
    ui->btnOk->setVisible(false);
    ui->btnCancel->setVisible(false);
    ui->btnCopy->setVisible(false);
    ui->btnNoAll->setVisible(false);
}

message::~message()
{
    delete ui;
}

void message::error(const QString &text)
{
    message m(fPreferences.getDefaultParentForMessage());
    m.ui->btnCopy->setVisible(true);
    m.ui->btnOk->setVisible(true);
    m.ui->lbMessage->setText(html
                           .arg("#070707")
                           .arg(":/images/error.png")
                           .arg("18")
                           .arg("black")
                           .arg(text));
    m.adjustSize();
    m.exec();
}

void message::error(const QStringList &text)
{
    QString err;
    foreach (QString s, text) {
        err += s + "<br>";
    }
    error(err);
}

int message::confirm(const QString &text)
{
    message m(fPreferences.getDefaultParentForMessage());
    m.ui->btnYes->setVisible(true);
    m.ui->btnNo->setVisible(true);
    m.ui->lbMessage->setText(html
                           .arg("darkblue")
                           .arg(":/images/info.png")
                           .arg("18")
                           .arg("black")
                           .arg(text));
    m.adjustSize();
    return m.exec();
}

void message::info(const QString &text)
{
    message m(fPreferences.getDefaultParentForMessage());
    m.ui->btnOk->setVisible(true);
    m.ui->lbMessage->setText(html
                           .arg("darkblue")
                           .arg(":/images/info.png")
                           .arg("18")
                           .arg("black")
                           .arg(text));
    m.adjustSize();
    m.exec();
}

int message::yesnocancel(const QString &text)
{
    message m(fPreferences.getDefaultParentForMessage());
    m.ui->btnYes->setVisible(true);
    m.ui->btnNo->setVisible(true);
    m.ui->btnCancel->setVisible(true);
    m.ui->btnNoAll->setVisible(true);
    m.ui->lbMessage->setText(html
                           .arg("darkblue")
                           .arg(":/images/info.png")
                           .arg("18")
                           .arg("black")
                           .arg(text));
    m.adjustSize();
    return m.exec();
}

void message::on_btnCancel_clicked()
{
    done(RESULT_CANCEL);
}

void message::on_btnOk_clicked()
{
    done(RESULT_OK);
}

void message::on_btnNo_clicked()
{
    done(RESULT_NO);
}

void message::on_btnYes_clicked()
{
    done(RESULT_YES);
}

void message::on_btnCopy_clicked()
{
    QTextDocument doc;
    doc.setHtml(ui->lbMessage->text());
    qApp->clipboard()->setText(doc.toPlainText());
}

void message::on_btnNoAll_clicked()
{
    done(RESULT_NOALL);
}
