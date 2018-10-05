#ifndef RERESTHALL_H
#define RERESTHALL_H

#include "roweditordialog.h"
#include "dwselectorrestmenu.h"

namespace Ui {
class RERestHall;
}

class RERestHall : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestHall(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestHall();
protected:
    virtual bool isDataCorrect();
private slots:
    void menu(CI_RestMenu *m);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
private:
    Ui::RERestHall *ui;
    DWSelectorRestMenu *fDockMenu;
};

#endif // RERESTHALL_H
