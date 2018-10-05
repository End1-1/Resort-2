#ifndef DLGCASHOP_H
#define DLGCASHOP_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgCashOp;
}

class DlgCashOp : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgCashOp(QWidget *parent = 0);
    ~DlgCashOp();
    static void cash(int doc);
    static void cashWithParams(int docType, int partner);
private:
    Ui::DlgCashOp *ui;
private:
    void load(int doc);
};

#endif // DLGCASHOP_H
