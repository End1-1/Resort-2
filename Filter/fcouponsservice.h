#ifndef FCOUPONSSERVICE_H
#define FCOUPONSSERVICE_H

#include "wfilterbase.h"

namespace Ui {
class FCouponsService;
}

class FCouponsService : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCouponsService(QWidget *parent = nullptr);
    ~FCouponsService();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();

private:
    Ui::FCouponsService *ui;
};

#endif // FCOUPONSSERVICE_H
