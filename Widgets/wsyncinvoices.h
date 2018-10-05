#ifndef WSYNCINVOICES_H
#define WSYNCINVOICES_H

#include "basewidget.h"
#include "databaseresult.h"
#include "dwselectorusers.h"

namespace Ui {
class WSyncInvoices;
}

class WSyncInvoices : public BaseWidget
{
    Q_OBJECT

public:
    explicit WSyncInvoices(QWidget *parent = 0);
    ~WSyncInvoices();
    static void open();
private slots:
    void on_leInvoice_returnPressed();

    void on_chFinance_clicked(bool checked);

    void on_chNoFinance_clicked(bool checked);

    void on_chPNoFinance_clicked(bool checked);

    void on_chPFinance_clicked(bool checked);

    void on_tblPData_clicked(const QModelIndex &index);

    void on_deDate_returnPressed();

    void on_btnSearchInvoice_clicked();

    void on_tblSearch_doubleClicked(const QModelIndex &index);

    void on_btnSaveCopy_clicked();

    void on_btnExportSingleVaucher_clicked();

    void on_tblPData_doubleClicked(const QModelIndex &index);

    void on_btnCloseSearch_clicked();

    void on_btnNextDate_clicked();

    void on_btnPrevDate_clicked();

    void on_btnExcel_clicked();

private:
    Ui::WSyncInvoices *ui;
    Database fSDb;
    DatabaseResult fReserv;
    DatabaseResult fReservGuests;
    DatabaseResult fGuests;
    DatabaseResult fVauchers;
    void loadInvoice(const QString &id);
    void makeInvoiceCopy();
    void exportInvoice();
    void countBalance();
    void countPBalance();
};

#endif // WSYNCINVOICES_H
