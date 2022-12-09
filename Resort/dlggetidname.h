#ifndef DLGGETIDNAME_H
#define DLGGETIDNAME_H

#include "baseextendeddialog.h"

#define idname_branch 1

namespace Ui {
class DlgGetIDName;
}

class DlgGetIDName : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgGetIDName(QWidget *parent = nullptr);
    ~DlgGetIDName();
    static bool get(QString &id, QString &name, int table, QWidget *parent = nullptr);

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_btnRefresh_clicked();

    void on_leFilter_textChanged(const QString &arg1);

    void on_tbl_cellDoubleClicked(int row, int column);

private:
    Ui::DlgGetIDName *ui;
    int fTable;
    QString fId;
    QString fName;
    void getData();
    static QHash<int, QString> fQueries;
    static QHash<int, DlgGetIDName*> fDialogs;
};

#endif // DLGGETIDNAME_H
