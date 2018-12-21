#ifndef RERESTDISH_H
#define RERESTDISH_H

#include "roweditordialog.h"
#include "dwselectorrestprinter.h"
#include "dwselectorreststore.h"
#include "dwselectordishmod.h"
#include "dwselectordishtype.h"
#include "dwselectorreststore.h"
#include "dwselectordish.h"

namespace Ui {
class RERestDish;
}

class RERestDish : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestDish(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestDish();
    virtual void selector(int number, const QVariant &value);
protected:
    virtual bool isDataCorrect();
    virtual void valuesToWidgets();
    virtual void clearWidgets();
    virtual void save();
    virtual void hide();
private slots:
    void checkComplex(bool v);
    void tabPageIndexChanged(int index);
    void printLineEditFocusIn();
    void printLineEditFocusOut();
    void printer(CI_RestPrinter *p);
    void store(CI_RestStore *s);
    void store2(CI_RestStore *s);
    void dishMod(CI_RestDishMod *m);
    void dishType(CI_RestDishType *c);
    void storeLineEditFocusIn();
    void storeLineEditFocusOut();
    void btnRemoveModifier(int row);
    void on_btnTextColor_clicked();
    void on_btnBgColor_clicked();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnAddModifier_clicked();
    void on_btnLoadImage_clicked();
    void on_btnAppendRecipe_clicked();
    void on_btnRemoveRecipeRow_clicked();
    void on_btnClearRecipe_clicked();
    void on_chContainItself_clicked(bool checked);
    void on_btnTrack_clicked();
    void on_leReadScancode_returnPressed();

    void on_btnRemove_clicked();

private:
    Ui::RERestDish *ui;
    bool fImageChanged;
    bool fImageLoaded;
    DWSelectorDishType *fDockType;
    DWSelectorRestPrinter *fDockPrint;
    DWSelectorRestStore *fDockStore;
    DWSelectorDishMod *fDockMod;
    DWSelectorDish *fDockDish;
    QStringList fRemovedModifiers;
    DWSelectorRestStore *fRestStore;
    EQLineEdit *createLineEdit(int row, int column);
    inline QString cellValue(int row, int column);
    inline void setCellValue(int row, int column, const QString &value);
    void addDishRow(const QString &code, const QString &name, double qty);
};

#endif // RERESTDISH_H
