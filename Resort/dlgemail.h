#ifndef DLGEMAIL_H
#define DLGEMAIL_H

#include "basedialog.h"

namespace Ui {
class DlgEmail;
}

class DlgEmail : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgEmail(QWidget *parent = 0);
    ~DlgEmail();
    static void sendDialog();
private slots:
    void on_btnSend_clicked();
private:
    Ui::DlgEmail *ui;
};

#endif // DLGEMAIL_H
