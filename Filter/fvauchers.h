#ifndef FVAUCHERS_H
#define FVAUCHERS_H

#include "wfilterbase.h"
#include "dwselectorvaucher.h"

namespace Ui {
class FVauchers;
}

class FVauchers : public WFilterBase
{
    Q_OBJECT

public:
    explicit FVauchers(QWidget *parent = 0);
    ~FVauchers();
    static void open();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    static void correctCL(Database &db);
    static void correctCLBack(Database &db);
private:
    QToolButton *fBtnRevive;
    QToolButton *fBtnRemove;
    Ui::FVauchers *ui;
    QString fQuery;
    DWSelectorVaucher *fDockVaucher;
    virtual void selector(int selectorNumber, const QVariant &value);
private slots:
    void clickOnRow(int row);
    void eliminateVoucher();
    void cancelation();
    void openInvoice();
    void reviveVaucher();
    void correctCLNames();
    void correctCLNamesBack();
    void editRawData();
    void edit();
    void recover();
    void printVaucher();
    void doubleClickOnRow(const QList<QVariant> &values);
    void on_chCanceled_clicked(bool checked);
    void on_toolButton_2_clicked();
    void on_toolButton_clicked();
};

#endif // FVAUCHERS_H
