#ifndef FSTOREENTRY_H
#define FSTOREENTRY_H

#include "wfilterbase.h"

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
private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
    void removeDoc();
};

#endif // FSTOREENTRY_H
