#ifndef FEXPORTRESERVATION_H
#define FEXPORTRESERVATION_H

#include "wfilterbase.h"

namespace Ui {
class FExportReservation;
}

class FExportReservation : public WFilterBase
{
    Q_OBJECT

public:
    explicit FExportReservation(QWidget *parent = 0);
    ~FExportReservation();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private slots:
    void openInvoice();
    void openReservation();
    void on_btnStartExport_clicked();

private:
    Ui::FExportReservation *ui;
    void exportReservation(const QString &id);
};

#endif // FEXPORTRESERVATION_H
