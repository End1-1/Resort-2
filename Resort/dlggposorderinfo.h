#ifndef DLGGPOSORDERINFO_H
#define DLGGPOSORDERINFO_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgGPOSOrderInfo;
}

class DlgGPOSOrderInfo : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgGPOSOrderInfo(QWidget *parent = 0);
    ~DlgGPOSOrderInfo();
    void setOrder(const QString &id);
    void setVaucher(const QString &id);
private slots:
    void showDishHistory(int tag);
    void on_btnOk_clicked();
    void on_btnSave_clicked();
    void on_tblData_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);
    void on_btnPrint_clicked();
    void on_btnPrintTax_clicked();
    void on_btnTracking_clicked();

private:
    Ui::DlgGPOSOrderInfo *ui;
    void countTotal();
};

#endif // DLGGPOSORDERINFO_H
