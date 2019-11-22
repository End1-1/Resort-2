#ifndef WSALARY_H
#define WSALARY_H

#include "wfilterbase.h"

namespace Ui {
class WSalary;
}

class WSalary : public WFilterBase
{
    Q_OBJECT

public:
    explicit WSalary(QWidget *parent = nullptr);
    ~WSalary();
    void apply(WReportGrid *rg) override;
    QString reportTitle() override;
    QWidget *firstElement() override;

private:
    Ui::WSalary *ui;
};

#endif // WSALARY_H
