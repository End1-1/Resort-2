#ifndef DLGCL_H
#define DLGCL_H

#include "basedialog.h"
#include "dwselectorcityledger.h"

namespace Ui {
class DlgCL;
}

class DlgCL : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgCL(QWidget *parent = 0);
    ~DlgCL();
    static bool getCL(QString &code, QString &name);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgCL *ui;
};

#endif // DLGCL_H
