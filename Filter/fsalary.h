#ifndef FSALARY_H
#define FSALARY_H

#include "wfilterbase.h"

namespace Ui {
class FSalary;
}

class FSalary : public WFilterBase
{
    Q_OBJECT

public:
    explicit FSalary(QWidget *parent = 0);
    ~FSalary();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FSalary *ui;
};

#endif // FSALARY_H
