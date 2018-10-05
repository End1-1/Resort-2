#ifndef DLGCARDEX_H
#define DLGCARDEX_H

#include "roweditordialog.h"
#include "dwselectorvatmode.h"
#include "dwselectorcityledger.h"
#include "dwselectorcardexgroup.h"

namespace Ui {
class DlgCardex;
}

class DlgCardex : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit DlgCardex(QList<QVariant> &values, QWidget *parent = 0);
    ~DlgCardex();
    virtual void setValues();
protected:
    virtual bool isDataCorrect();
private slots:
    void cardexGroup(CI_CardexGroup *c);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

    void on_btnRemove_clicked();

private:
    Ui::DlgCardex *ui;
    DWSelectorVATMode *fDockVAT;
    DWSelectorCardexGroup *fDockGroup;
};

#endif // DLGCARDEX_H
