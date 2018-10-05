#ifndef RERESTTABLE_H
#define RERESTTABLE_H

#include "roweditordialog.h"
#include "dwselectorhall.h"

namespace Ui {
class RERestTable;
}

class RERestTable : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestTable(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestTable();
protected:
    virtual bool isDataCorrect();
private slots:
    void hall(CI_RestHall *h);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
private:
    Ui::RERestTable *ui;
    DWSelectorHall *fDockHall;
};

#endif // RERESTTABLE_H
