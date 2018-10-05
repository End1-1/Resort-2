#ifndef DLGOPENGROUPORSINGLE_H
#define DLGOPENGROUPORSINGLE_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgOpenGroupOrSingle;
}

class DlgOpenGroupOrSingle : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgOpenGroupOrSingle(QWidget *parent = 0);
    ~DlgOpenGroupOrSingle();
    static int choice();
private slots:
    void on_btnOpenSingle_clicked();

    void on_btnOpenGroup_clicked();

private:
    Ui::DlgOpenGroupOrSingle *ui;
};

#endif // DLGOPENGROUPORSINGLE_H
