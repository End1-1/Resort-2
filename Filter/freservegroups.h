#ifndef FRESERVEGROUPS_H
#define FRESERVEGROUPS_H

#include "wfilterbase.h"

namespace Ui {
class FReserveGroups;
}

class FReserveGroups : public WFilterBase
{
    Q_OBJECT

public:
    explicit FReserveGroups(QWidget *parent = 0);
    ~FReserveGroups();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void openReport();
private:
    Ui::FReserveGroups *ui;
private slots:
    void doubleClick(const QList<QVariant> &row);
    void removeGroup();
};

#endif // FRESERVEGROUPS_H
