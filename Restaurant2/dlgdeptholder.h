#ifndef DLGDEPTHOLDER_H
#define DLGDEPTHOLDER_H

#include "baseextendeddialog.h"
#include <QListWidgetItem>

namespace Ui {
class DlgDeptHolder;
}

class DlgDeptHolder : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgDeptHolder(QWidget *parent = 0);
    ~DlgDeptHolder();
    static bool getHolder(int &id, QString &name);
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

    void on_btnCreateNew_clicked();

    void on_lstHolders_itemClicked(QListWidgetItem *item);

private:
    Ui::DlgDeptHolder *ui;
    QString fHolderName;
    int fHolderId;
    bool fEditName;
};

#endif // DLGDEPTHOLDER_H
