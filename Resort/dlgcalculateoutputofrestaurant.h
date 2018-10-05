#ifndef DLGCALCULATEOUTPUTOFRESTAURANT_H
#define DLGCALCULATEOUTPUTOFRESTAURANT_H

#include "basedialog.h"

namespace Ui {
class DlgCalculateOutputOfRestaurant;
}

class DlgCalculateOutputOfRestaurant : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgCalculateOutputOfRestaurant(QWidget *parent = 0);
    ~DlgCalculateOutputOfRestaurant();
    static void openDialog();
private slots:
    void on_btnCancel_clicked();
    void on_btnGo_clicked();

private:
    Ui::DlgCalculateOutputOfRestaurant *ui;
    QMap<int, double> fMiddlePrices;
    QMap<int, double> fLastPrices;
    void calculateForDate(const QDate &date);
    void calculateForStore(int store, const QDate &date);
};

#endif // DLGCALCULATEOUTPUTOFRESTAURANT_H
