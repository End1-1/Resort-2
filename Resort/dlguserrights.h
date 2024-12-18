#ifndef DLGUSERRIGHTS_H
#define DLGUSERRIGHTS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgUserRights;
}

class DlgUserRights : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgUserRights(QWidget *parent = 0);
    ~DlgUserRights();
    void setup(QList<QVariant> &values);
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_chCheckUncheck_clicked(bool checked);

private:
    Ui::DlgUserRights *ui;
    int fGroupId;
    void checkAll(QObject *parent, bool checked);
    void check(QObject *parent, bool checked, int tag);
    void checkQuery(QObject *parent, QString &query, bool &first);
};

#endif // DLGUSERRIGHTS_H
