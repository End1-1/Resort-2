#ifndef FCASH_H
#define FCASH_H

#include "wfilterbase.h"

namespace Ui {
class FCash;
}

class FCash : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCash(QWidget *parent = 0);
    ~FCash();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
    virtual void selector(int selectorNumber, const QVariant &value);
private:
    Ui::FCash *ui;
private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
    void removeDoc();
};

#endif // FCASH_H
