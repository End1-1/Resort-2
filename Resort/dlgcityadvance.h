#ifndef DLGCITYADVANCE_H
#define DLGCITYADVANCE_H

#include "baseextendeddialog.h"
#include "dwselectorcityledger.h"
#include "dwselectorcladvance.h"
#include "dwselectoractiveroom.h"
#include "dwselectorredreservation.h"

namespace Ui {
class DlgCityAdvance;
}

class DlgCityAdvance : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgCityAdvance(QWidget *parent = 0);
    ~DlgCityAdvance();
    virtual void selector(int selectorNumber, const QVariant &value);
    static void cityAdvance(const QString &id, const QString &invoice, int mode);
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

    void on_btnPrint_clicked();

    void on_rbToCityLedger_clicked();

    void on_rbToReservation_clicked();

    void on_rbAdvanceToRoom_clicked();

private:
    Ui::DlgCityAdvance *ui;
    DWSelectorCityLedger *fDockCL;
    DWSelectorCLAdvance *fDockAdvance;
    DWSelectorActiveRoom *fDockRoom;
    DWSelectorActiveRoom *fDockDstRoom;
    DWSelectorRedReservation *fDockRed;
    void load(const QString &id);
    void loadInvoice(const QString &invoice);
};

#endif // DLGCITYADVANCE_H
