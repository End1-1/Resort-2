#ifndef WSTOREENTRY_H
#define WSTOREENTRY_H

#include "basewidget.h"
#include "dwselectordish.h"
#include "dwselectorreststore.h"

namespace Ui {
class WStoreEntry;
}

class WStoreEntry : public BaseWidget
{
    Q_OBJECT

public:
    explicit WStoreEntry(QWidget *parent = 0);
    ~WStoreEntry();
    virtual void setup();
    virtual void selector(int selectorNumber, const QVariant &value);
    static void openDoc(int doc);
private:
    Ui::WStoreEntry *ui;
    DWSelectorDish *fDockDish;
    DWSelectorRestStore *fDockStore;
    void load(int doc);
    void newGoods(CI_Dish *c);
    void countTotal();
private slots:
    void qtyChange(const QString &arg1);
    void priceChange(const QString &arg1);
    void totalChange(const QString &arg1);
    void store(CI_RestStore *c);
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_btnSave_clicked();
    void on_btnRemove_clicked();
    void on_btnCalculate_clicked();
    void on_btnPrint_clicked();
    void on_leSearch_textChanged(const QString &arg1);
    void on_btnExcel_clicked();
};

#endif // WSTOREENTRY_H
