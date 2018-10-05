#ifndef DLGREMOTINVOICES_H
#define DLGREMOTINVOICES_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgRemotInvoices;
}

class DlgRemotInvoices : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgRemotInvoices(bool local, QWidget *parent = 0);
    ~DlgRemotInvoices();
    QString fResult;
private slots:
    void on_leSearch_textChanged(const QString &arg1);

    void on_tblData_doubleClicked(const QModelIndex &index);

private:
    Ui::DlgRemotInvoices *ui;
};

#endif // DLGREMOTINVOICES_H
