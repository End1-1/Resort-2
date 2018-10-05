#ifndef FEVENTS_H
#define FEVENTS_H

#include "wfilterbase.h"

namespace Ui {
class FEvents;
}

class FEvents : public WFilterBase
{
    Q_OBJECT

public:
    explicit FEvents(QWidget *parent = 0);
    ~FEvents();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void open();
private:
    Ui::FEvents *ui;
    QString fQuery;
private slots:
    void removeEvent();
    void eliminate();
};

#endif // FEVENTS_H
