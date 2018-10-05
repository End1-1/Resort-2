#ifndef FSTOREMOVEMENT_H
#define FSTOREMOVEMENT_H

#include "wfilterbase.h"

namespace Ui {
class FStoreMovement;
}

class FStoreMovement : public WFilterBase
{
    Q_OBJECT

public:
    explicit FStoreMovement(QWidget *parent = 0);
    ~FStoreMovement();
    virtual void selector(int number, const QVariant &value);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FStoreMovement *ui;
private slots:
    void doubleClicked(const QList<QVariant> &row);
    void on_btnDateLeft_clicked();
    void on_btnDateRight_clicked();
};

#endif // FSTOREMOVEMENT_H
