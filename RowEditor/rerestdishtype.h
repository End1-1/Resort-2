#ifndef RERESTDISHTYPE_H
#define RERESTDISHTYPE_H

#include "roweditordialog.h"
#include "dwselectordishpart.h"

namespace Ui {
class RERestDishType;
}

class RERestDishType : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestDishType(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestDishType();
protected:
    virtual bool isDataCorrect();
private slots:
    void dishPart(CI_RestDishPart *d);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnBgColor_clicked();
    void on_btnTextColor_clicked();
private:
    Ui::RERestDishType *ui;
    DWSelectorDishPart *fDockPart;
};

#endif // RERESTDISHTYPE_H
