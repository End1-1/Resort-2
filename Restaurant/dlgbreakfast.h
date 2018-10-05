#ifndef DLGBREAKFAST_H
#define DLGBREAKFAST_H

#include "baseextendeddialog.h"
#include "cacheinvoiceitem.h"
#include "user.h"

namespace Ui {
class DlgBreakfast;
}

class DlgBreakfast : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgBreakfast(User *user, QWidget *parent = 0);
    ~DlgBreakfast();

private:
    Ui::DlgBreakfast *ui;
    CI_InvoiceItem *fBreakfast;
    User *fUser;

private slots:
    void btnPClick(int tag);
    void btnMClick(int tag);
    void btnPrint(int tag);
    void countRow(int row);
    void saveRow(int row);
    void on_btnExit_clicked();
    void on_btnSave_clicked();
    void on_btnUp_clicked();
    void on_btnDown_clicked();
};

#endif // DLGBREAKFAST_H
