#ifndef FMENUREVIEW_H
#define FMENUREVIEW_H

#include "wfilterbase.h"

namespace Ui
{
class FMenuReview;
}

class FMenuReview : public WFilterBase
{
    Q_OBJECT
public:
    explicit FMenuReview(QWidget *parent = nullptr);
    ~FMenuReview();
    virtual QString reportTitle();
    virtual QWidget* firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FMenuReview* ui;
    QString fMenu;
};

#endif // FMENUREVIEW_H
