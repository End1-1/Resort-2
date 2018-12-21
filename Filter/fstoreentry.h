#ifndef FSTOREENTRY_H
#define FSTOREENTRY_H

#include "wfilterbase.h"
#include "dwselectorreststore.h"

namespace Ui {
class FStoreEntry;
}

class FStoreEntry : public WFilterBase
{
    Q_OBJECT

public:
    explicit FStoreEntry(QWidget *parent = 0);
    ~FStoreEntry();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FStoreEntry *ui;
    DWSelectorRestStore *fDockStore;
private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
    void removeDoc();
    void store(CI_RestStore *c);
};

#endif // FSTOREENTRY_H
