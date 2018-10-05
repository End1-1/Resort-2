#ifndef STOREDOC_H
#define STOREDOC_H

#include "basewidget.h"
#include "dwselectorreststore.h"
#include "dwselectorstoredocstate.h"
#include "dwselectorstoredoctype.h"
#include "dwselectorstorepartners.h"
#include "dwselectordish.h"

namespace Ui {
class StoreDoc;
}

class StoreDoc : public BaseWidget
{
    Q_OBJECT

public:
    explicit StoreDoc(QWidget *parent = 0);
    ~StoreDoc();
    void loadDoc(const QString &id);
    void copyDoc(const QString &id);
    virtual void setup();
    virtual void selector(int number, const QVariant &value);
    static void openStoreDocument(const QString &id);
private:
    Ui::StoreDoc *ui;
    DWSelectorRestStore *fDockStore;
    DWSelectorRestStore *fDockStore2;
    DWSelectorStoreDocState *fDockState;
    DWSelectorStoreDocType *fDockType;
    DWSelectorStorePartners *fDockPartner;
    DWSelectorDish *fDockDish;
    QWidget *fFirstTab;
    QWidget *fGoodsTab;
    QWidget *fAccTab;
    void prepareDoc();
    void countTotal();
    void saveDoc(int docState);
private slots:
    void qtyChange(const QString &arg1);
    void priceChange(const QString &arg1);
    void totalChange(const QString &arg1);
    void store(CI_RestStore *c);
    void store2(CI_RestStore *c);
    void docType(CI_StoreDocType *c);
    void on_btnSaveDoc_clicked();
    void on_btnNewMaterial_clicked();
    void newGoods(CI_Dish *c);
    void on_btnAddMaterial_clicked();
    void on_btnDraftDoc_clicked();
    void on_btnRemoveDoc_clicked();
    void on_btnAddMaterialByScan_clicked();
    void on_btnRemoveMaterial_clicked();
    void on_deDate_textChanged(const QString &arg1);
    void on_leTotal_textChanged(const QString &arg1);
    void on_leComments_textChanged(const QString &arg1);
    void on_btnPrintDoc_clicked();
    void on_btnExcel_clicked();
};

#endif // STOREDOC_H
