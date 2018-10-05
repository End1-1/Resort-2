#ifndef DLGTAXREFERENCE_H
#define DLGTAXREFERENCE_H

#include "baseextendeddialog.h"
#include <QMenu>

namespace Ui {
class DlgTaxReference;
}

class DlgTaxReference : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTaxReference(int itemId, QWidget *parent = 0);
    ~DlgTaxReference();
    void setHeader(const QString &room, const QString &guest, const QString &invoice);
    void setItem (const QString &name, const QString &amount, int recId);
private slots:
    void tblContextMenu(const QPoint &p);
    void deleteRow();
    void on_leSearch_textEdited(const QString &arg1);

    void on_tblData_doubleClicked(const QModelIndex &index);

    void on_rbAllInvoice_clicked();

    void on_rbThisInvoice_clicked();

private:
    Ui::DlgTaxReference *ui;
    int fItemId;
    int fRecId;
    QMenu fTaxMenu;
    void filterRows();
};

#endif // DLGTAXREFERENCE_H
