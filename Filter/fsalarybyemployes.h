#ifndef FSALARYBYEMPLOYES_H
#define FSALARYBYEMPLOYES_H

#include "wfilterbase.h"

namespace Ui {
class FSalaryByEmployes;
}

class FSalaryByEmployes : public WFilterBase
{
    Q_OBJECT

public:
    explicit FSalaryByEmployes(QWidget *parent = nullptr);
    ~FSalaryByEmployes();
    virtual void apply(WReportGrid *rg) override;
    virtual QWidget *firstElement() override;
    virtual QString reportTitle() override;

private:
    Ui::FSalaryByEmployes *ui;
};

#endif // FSALARYBYEMPLOYES_H
