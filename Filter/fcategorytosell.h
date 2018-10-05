#ifndef FCATEGORYTOSELL_H
#define FCATEGORYTOSELL_H

#include "wfilterbase.h"

namespace Ui {
class FCategoryToSell;
}

class FCategoryToSell : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCategoryToSell(QWidget *parent = 0);
    ~FCategoryToSell();
    virtual void setup();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
private:
    Ui::FCategoryToSell *ui;
};

#endif // FCATEGORYTOSELL_H
