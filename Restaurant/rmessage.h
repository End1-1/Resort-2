#ifndef RMESSAGE_H
#define RMESSAGE_H

#include "baseextendeddialog.h"

#define message_error(msg) RMessage::showError(msg, this)
#define message_question(msg) RMessage::showQuestion(msg, this)
#define message_info(msg) RMessage::showInfo(msg, this)
#define message_yesnocancel(msg) RMessage::showYesNoCancel(msg, this)

#define RESULT_NO 0
#define RESULT_YES 1
#define RESULT_CANCEL 2

namespace Ui {
class RMessage;
}

class RMessage : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RMessage(QWidget *parent = 0);
    ~RMessage();
    static void showError(const QString &message, QWidget *parent);
    static bool showQuestion(const QString &message, QWidget *parent);
    static void showInfo(const QString &message, QWidget *parent);
    static int showYesNoCancel(const QString &message, QWidget *parent);
private slots:
    void on_btnCancel_clicked();
    void on_btnYES_clicked();
    void on_btnOK_clicked();

    void on_btnCancel_2_clicked();

private:
    Ui::RMessage *ui;
};

#endif // RMESSAGE_H
