#ifndef DLGUSERGROUP_H
#define DLGUSERGROUP_H

#include "roweditordialog.h"

namespace Ui {
class DlgUserGroup;
}

class DlgUserGroup : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit DlgUserGroup(QList<QVariant> &values, QWidget *parent = 0);
    ~DlgUserGroup();
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
private:
    Ui::DlgUserGroup *ui;
};

#endif // DLGUSERGROUP_H
