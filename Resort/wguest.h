#ifndef WGUEST_H
#define WGUEST_H

#include "roweditordialog.h"
#include "dwselectorguesttitle.h"
#include "dwselectornation.h"

namespace Ui {
class WGuest;
}

class WGuest : public RowEditorDialog
{
    Q_OBJECT

public:
    static WGuest *guest(QList<QVariant> &values, bool noCheckNation = true);
    ~WGuest();
private slots:
    void nation(CI_Nation *c);
    void guestTitle(CI_GuestTitle *gt);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

    void on_btnRemove_clicked();

private:
    explicit WGuest(QList<QVariant> &values, QWidget *parent = 0);
    Ui::WGuest *ui;
    bool fNoCheckNation;
    DWSelectorGuestTitle *fDockTitle;
    DWSelectorNation *fDockNation;
};

#endif // WGUEST_H
