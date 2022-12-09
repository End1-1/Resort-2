#ifndef DLGCOUPONSERVICEPAYMENT_H
#define DLGCOUPONSERVICEPAYMENT_H

#include <QDialog>

namespace Ui {
class DlgCouponServicePayment;
}

class ReportQuery;

class DlgCouponServicePayment : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCouponServicePayment(ReportQuery *rq, QWidget *parent = nullptr);
    ~DlgCouponServicePayment();
    virtual void keyPressEvent(QKeyEvent *e);

private slots:
    void on_btnCancel_clicked();
    void on_btnSave_clicked();

private:
    Ui::DlgCouponServicePayment *ui;
};

#endif // DLGCOUPONSERVICEPAYMENT_H
