#ifndef FSTOREDOCS_H
#define FSTOREDOCS_H

#include "wfilterbase.h"
#include "dwselectorstoredocstate.h"
#include "dwselectorstoredoctype.h"
#include "dwselectorreststore.h"
#include "dwselectorstorepartners.h"

namespace Ui {
class FStoreDocs;
}

class FStoreDocs : public WFilterBase
{
    Q_OBJECT

public:
    explicit FStoreDocs(QWidget *parent = 0);
    ~FStoreDocs();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    void selector(int number, const QVariant &value);
    static void openReport();
private slots:
    void store(CI_RestStore *c);
    void copyDoc();
    void doubleClicked(const QList<QVariant> &row);
    void on_btnDateLeft_clicked();

    void on_btnDateRight_clicked();

private:
    DWSelectorStoreDocState *fDockState;
    DWSelectorStoreDocType *fDockType;
    DWSelectorStorePartners *fDockPartner;
    DWSelectorRestStore *fDockStore;
    Ui::FStoreDocs *ui;
};

#endif // FSTOREDOCS_H
