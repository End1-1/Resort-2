#ifndef WSYNCHRONIZE_H
#define WSYNCHRONIZE_H

#include "basewidget.h"

namespace Ui {
class WSynchronize;
}

class WSynchronize : public BaseWidget
{
    Q_OBJECT

public:
    explicit WSynchronize(QWidget *parent = 0);
    ~WSynchronize();
    virtual void setupTab();
private:
    Ui::WSynchronize *ui;
    QString fQuery;
    void hotelReport();
    void restaurantReport();
    void vauchersReport();
    void noShowReport();
    void checkinReservedReport();
    void eventReport();
    bool goHotel();
    bool goRestaurant();
    bool goVauchers();
    bool goNoShow();
    bool goCheckinReserved();
    void clearRestaurant();
    void clearVauchers();
    void clearNoShow();
private slots:
    void dateStartChanged(const QDate &date);
    void totalHScroll(int value);
    void on_btnRefresh_clicked();
    void on_btnGo_clicked();
    void on_chSelectAll_clicked(bool checked);
    void on_rbRestaurant_clicked();
    void on_rbHotel_clicked();
    void on_rbVauchers_clicked();
    void on_btnNextDate_clicked();
    void on_btnPrevDate_clicked();
    void on_btnRemoveSync_clicked();
    void on_btnExcel_clicked();
};

#endif // WSYNCHRONIZE_H
