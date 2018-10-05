#ifndef DLGLIST_H
#define DLGLIST_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgList;
}

class DlgList : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgList(QWidget *parent = 0);
    ~DlgList();
    static bool getValue(const QString &caption, QString &name, QVariant &result, const QString &sql);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgList *ui;
    QString fName;
    QVariant fResult;
    void build(const QString &sql);
};

#endif // DLGLIST_H
