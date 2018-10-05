#ifndef MESSAGE_H
#define MESSAGE_H

#include "base.h"
#include <QDialog>

#define message_error_tr(text) message::error(QObject::tr(text))
#define message_error(text) message::error(text)
#define message_error_lst(text) message::error(text)
#define message_confirm(text) message::confirm(text)
#define message_confirm_tr(text) message::confirm(QObject::tr(text))
#define message_info(text) message::info(text)
#define message_info_tr(text) message::info(QObject::tr(text))
#define message_yesnocancel(text) message::yesnocancel(text)

#define RESULT_YES 1
#define RESULT_NO 2
#define RESULT_CANCEL 3
#define RESULT_OK 4
#define RESULT_NOALL 5

namespace Ui {
class message;
}

class message : public QDialog, public Base
{
    Q_OBJECT

public:
    explicit message(QWidget *parent = 0);
    ~message();
    static void error(const QString &text);
    static void error(const QStringList &text);
    static int confirm(const QString &text);
    static void info(const QString &text);
    static int yesnocancel(const QString &text);
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnNo_clicked();
    void on_btnYes_clicked();

    void on_btnCopy_clicked();

    void on_btnNoAll_clicked();

private:
    Ui::message *ui;
};

#endif // MESSAGE_H
