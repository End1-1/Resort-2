#ifndef DLGCHANGECLOFVAUCHER_H
#define DLGCHANGECLOFVAUCHER_H

#include "basedialog.h"
#include "dwselectorcityledger.h"

namespace Ui {
class DlgChangeCLOfVaucher;
}

class DlgChangeCLOfVaucher : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgChangeCLOfVaucher(QWidget *parent = 0);
    ~DlgChangeCLOfVaucher();
    static bool changeCL(const QString &vaucher, const QString &oldCL, const QString &name, const QString &amount);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgChangeCLOfVaucher *ui;
};

#endif // DLGCHANGECLOFVAUCHER_H
