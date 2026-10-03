#ifndef RERESTBRANCH_H
#define RERESTBRANCH_H

#include "roweditordialog.h"

namespace Ui {
class RERestBranch;
}

class RERestBranch : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestBranch(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestBranch();

protected:
    virtual void valuesToWidgets();
    virtual void save();

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    void loadMenuTable();
    void saveBranchMenus(int branchId);

    Ui::RERestBranch *ui;
};

#endif // RERESTBRANCH_H
