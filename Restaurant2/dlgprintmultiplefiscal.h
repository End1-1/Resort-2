#ifndef DLGPRINTMULTIPLEFISCAL_H
#define DLGPRINTMULTIPLEFISCAL_H

#include "basedialog.h"

namespace Ui {
class DlgPrintMultipleFiscal;
}

class DlgPrintMultipleFiscal : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgPrintMultipleFiscal(QWidget *parent = nullptr);
    ~DlgPrintMultipleFiscal();

private slots:
    void on_btnCancel_clicked();
    void on_btnPrint_clicked();
    void on_btnCard_clicked(bool checked);
    void on_btnCash_clicked(bool checked);
    void on_cbFiscalMachine_currentTextChanged(const QString &arg1);

private:
    Ui::DlgPrintMultipleFiscal *ui;
    QMap<QString, QMap<QString, QVariant> > fFiscalMachines;
};

#endif // DLGPRINTMULTIPLEFISCAL_H
