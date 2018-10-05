#ifndef DLGPOSTBREAKFAST_H
#define DLGPOSTBREAKFAST_H

#include "baseextendeddialog.h"
#include "dwselectorresttable.h"
#include "dwselectorrestfullmenu.h"


namespace Ui {
class DlgPostBreakfast;
}

class DlgPostBreakfast : public BaseExtendedDialog
{
    Q_OBJECT

public:
    enum POST_TYPE {ptNone = 0, ptBreakfast, ptMinibar};
    explicit DlgPostBreakfast(QWidget *parent = 0);
    ~DlgPostBreakfast();
    static bool postBreakfast(const QString &invoice, POST_TYPE pt = ptNone);
    virtual void selector(int selectorNumber, const QVariant &value);
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

    void on_btnPrintTax_clicked();

    void on_leQty_textChanged(const QString &arg1);

    void on_leAmount_textChanged(const QString &arg1);

private:
    Ui::DlgPostBreakfast *ui;
    DWSelectorRestTable *fDockTable;
    DWSelectorRestFullMenu *fDockDish;
};

#endif // DLGPOSTBREAKFAST_H
