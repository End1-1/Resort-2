#ifndef FCARDEXSALES_H
#define FCARDEXSALES_H

#include "wfilterbase.h"
#include "dwselectorcardex.h"

namespace Ui {
class FCardexSales;
}

class FCardexSales : public WFilterBase
{
    Q_OBJECT

    typedef struct {
        QString fTitle;
        int fWidth;
        QString fItems;
    } Field;

public:
    explicit FCardexSales(QWidget *parent = 0);
    ~FCardexSales();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void open();
protected:
    virtual void finalPrint(PPrintScene *ps, int top);
private:
    Ui::FCardexSales *ui;
    QString fQuery;
    DWSelectorCardex *fDockCardex;
    QMap<int, Field> fItems;
    void guestReport(WReportGrid *rg);
    void cardexReport(WReportGrid *rg);
private slots:
    void openInvoice();
    void cardex(CI_Cardex *c);
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_btnHelp_clicked();
    void on_cbReportType_currentIndexChanged(int index);
    void on_chNoVat_clicked();
    void on_chCheckin_clicked();
};

#endif // FCARDEXSALES_H
