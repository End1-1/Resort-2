#ifndef DLGSEARCHINVOICE_H
#define DLGSEARCHINVOICE_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgSearchInvoice;
}

class DlgSearchInvoice : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgSearchInvoice(const QDate &start, const QDate &end, QWidget *parent = 0);
    ~DlgSearchInvoice();
    int fInvoice;
private slots:
    void on_btnClearFilter_clicked();

    void on_btnRefresh_clicked();

    void on_tblData_doubleClicked(const QModelIndex &index);

private:
    Ui::DlgSearchInvoice *ui;
    void filter();
};

#endif // DLGSEARCHINVOICE_H
