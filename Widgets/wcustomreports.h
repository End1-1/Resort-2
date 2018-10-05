#ifndef WCUSTOMREPORTS_H
#define WCUSTOMREPORTS_H

#include "basewidget.h"

class WReportGrid;

namespace Ui {
class WCustomReports;
}

class WCustomReports : public BaseWidget
{
    Q_OBJECT

public:
    explicit WCustomReports(QWidget *parent = 0);
    ~WCustomReports();
    virtual void setup();
private slots:
    void executeSQL();
    void on_btnShowMenu_clicked();
    void on_trMenu_clicked(const QModelIndex &index);

private:
    Ui::WCustomReports *ui;
    WReportGrid *fReportGrid;
    QString fFilterSQL;
    QList<QWidget*> fFilterWidgets;
    QMap<QWidget*, QString> fFilterFields;
    QMap<QWidget*, QString> fFilterBuilds;
    QMap<QWidget*, QString> fFilterDefExpr;
    QString fSumCols;
    QString fSubtotal;
};

#endif // WCUSTOMREPORTS_H
