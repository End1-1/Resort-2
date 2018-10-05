#ifndef RECITYLEDGER_H
#define RECITYLEDGER_H

#include "roweditordialog.h"

namespace Ui {
class RECityLedger;
}

class RECityLedger : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECityLedger(QList<QVariant> &values, QWidget *parent = 0);
    ~RECityLedger();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_btnRemove_clicked();

    void on_btnInitialBalance_clicked();

private:
    Ui::RECityLedger *ui;
};

#endif // RECITYLEDGER_H
