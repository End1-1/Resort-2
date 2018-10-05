#ifndef FCALLHISTORY_H
#define FCALLHISTORY_H

#include "wfilterbase.h"

namespace Ui {
class FCallHistory;
}

class FCallHistory : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCallHistory(QWidget *parent = 0);
    ~FCallHistory();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void open();
private slots:
    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

private:
    Ui::FCallHistory *ui;
    QString fQuery;
};

#endif // FCALLHISTORY_H
