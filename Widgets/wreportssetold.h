#ifndef WREPORTSSETOLD_H
#define WREPORTSSETOLD_H

#include "basewidget.h"
#include <QButtonGroup>

class WReportGrid;

namespace Ui {
class WReportsSetOld;
}

class WReportsSetOld : public BaseWidget
{
    Q_OBJECT

public:
    explicit WReportsSetOld(QWidget *parent = 0);
    ~WReportsSetOld();
    virtual void setup();
    virtual QString title();
private slots:
    void rbClicked();
    void on_btnGo_clicked();

private:
    Ui::WReportsSetOld *ui;
    QList<QWidget*> fFilterWidgets;
    QMap<QWidget*, QString> fFilterFields;
    QMap<QWidget*, QString> fFilterBuilds;
    QMap<QWidget*, QString> fFilterDefExpr;
    QButtonGroup fBtnGroup;
    QButtonGroup fBtnMonth;
};

#endif // WREPORTSSETOLD_H
