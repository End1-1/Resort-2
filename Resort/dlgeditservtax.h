#ifndef DLGEDITSERVTAX_H
#define DLGEDITSERVTAX_H

#include "basedialog.h"

namespace Ui {
class DlgEditServTax;
}

class DlgEditServTax : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgEditServTax(QWidget *parent = 0);
    ~DlgEditServTax();
    void setParams(int id, const QString &comp, int active, const QString &host, const QString &port, const QString &pass);
private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgEditServTax *ui;
    int fId;
};

#endif // DLGEDITSERVTAX_H
