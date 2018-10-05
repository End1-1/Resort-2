#ifndef FCOMMONFILTERBYDATE_H
#define FCOMMONFILTERBYDATE_H

#include "wfilterbase.h"

namespace Ui {
class FCommonFilterByDate;
}

class FCommonFilterByDate : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCommonFilterByDate(int sqlId, QWidget *parent = 0);
    ~FCommonFilterByDate();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void open(int id);
private:
    Ui::FCommonFilterByDate *ui;
    QString fTitle;
    QString fQuery;
    QList<int> fSumColumns;
};

#endif // FCOMMONFILTERBYDATE_H
