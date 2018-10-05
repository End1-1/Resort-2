#ifndef DLGBACK_H
#define DLGBACK_H

#include "basedialog.h"

namespace Ui {
class DlgBack;
}

class DlgBack : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgBack(QWidget *parent = 0);
    ~DlgBack();

private slots:
    void on_btnStart_clicked();

private:
    Ui::DlgBack *ui;
    void log(const QString &text, bool append = true);
};

#endif // DLGBACK_H
