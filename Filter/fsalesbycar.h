#ifndef FSALESBYCAR_H
#define FSALESBYCAR_H

#include "wfilterbase.h"

namespace Ui {
class FSalesByCar;
}

class FSalesByCar : public WFilterBase
{
    Q_OBJECT

public:
    explicit FSalesByCar(QWidget *parent = 0);
    ~FSalesByCar();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FSalesByCar *ui;

private slots:
    void branchEditDoubleClick(bool v);
    void hallEditDoubleClick(bool v);
};

#endif // FSALESBYCAR_H
