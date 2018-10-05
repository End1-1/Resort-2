#ifndef WWELCOME_H
#define WWELCOME_H

#include "basewidget.h"
#include <QTime>

namespace Ui {
class WWelcome;
}

class WWelcome : public BaseWidget
{
    Q_OBJECT

public:
    explicit WWelcome(QWidget *parent = 0);
    ~WWelcome();
    void setSlogan(const QString &slogan);
    virtual void handleBroadcast(const QMap<QString, QVariant> &data);
protected:
    virtual void setupTab();
    virtual void showEvent(QShowEvent *event);
private slots:
    void on_btnRoomChart_clicked();

    void on_btnAdvance_clicked();

    void on_btnPostCharge_clicked();

    void on_btnReceipt_clicked();

    void on_btnDiscount_clicked();

    void on_btnTransfer_clicked();

    void on_btnNewReservation_clicked();

    void on_btnArrivalDeparture_clicked();

    void on_btnNewGroupResevation_clicked();

    void on_btnReservations_clicked();

    void on_btnInhouseGuest_clicked();

    void on_btnNotes_clicked();

    void on_btnContacts_clicked();

    void on_btnCallHistory_clicked();

    void on_btnCityLedgerDetailedBalance_clicked();

    void on_btnCheckoutInvoices_clicked();

    void on_btnDailyMovement_clicked();

    void on_btnMonthlyMovement_clicked();

    void on_btnCashReportSummary_clicked();

    void on_btnCashReportDetailed_clicked();

    void on_btnCardexAnalysis_clicked();

    void on_btnVauchers_clicked();

    void on_btnExportInvoices_clicked();

    void on_btnExportRestaurant_clicked();

    void on_btnExportActiveReservation_clicked();

    void on_btnButtonsConfig_clicked();

    void on_btnRestaurant_clicked();

    void on_btnRemarks_clicked();

private:
    Ui::WWelcome *ui;
    QTimer fTimer;
    void configureLabels();
private slots:
    void loadInfo();
    void on_btnHotelStatus_clicked();
    void on_btnReportsSet_clicked();
    void on_btnOnlineInvoice_clicked();
    void on_btnStatistic_clicked();
};

#endif // WWELCOME_H
