#ifndef RECASHDESK_H
#define RECASHDESK_H

#include "roweditordialog.h"

namespace Ui {
class RECashDesk;
}

class RECashDesk : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECashDesk(QList<QVariant> &values, QWidget *parent = 0);
    ~RECashDesk();
    static void openReport();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RECashDesk *ui;
};

#endif // RECASHDESK_H
