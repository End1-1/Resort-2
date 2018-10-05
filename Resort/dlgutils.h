#ifndef DLGUTILS_H
#define DLGUTILS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgUtils;
}

class DlgUtils : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgUtils(QWidget *parent = 0);
    ~DlgUtils();

private slots:
    void on_btnGenNumbers_clicked();

private:
    Ui::DlgUtils *ui;
};

#endif // DLGUTILS_H
