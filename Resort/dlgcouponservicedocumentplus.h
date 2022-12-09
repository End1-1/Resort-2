#ifndef DLGCOUPONSERVICEDOCUMENTPLUS_H
#define DLGCOUPONSERVICEDOCUMENTPLUS_H

#include <QDialog>

namespace Ui {
class DlgCouponServiceDocumentPlus;
}

class ReportQuery;

class DlgCouponServiceDocumentPlus : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCouponServiceDocumentPlus(ReportQuery *rq, QWidget *parent = nullptr);
    ~DlgCouponServiceDocumentPlus();
    void getResult(QString &couponType, QString &first, QString &last, double &qty, double &price, double &discount, double &total);
    virtual void keyPressEvent(QKeyEvent *e);

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_leQty_textChanged(const QString &arg1);
    void on_btnFirstNumber_clicked();
    void on_cbType_currentIndexChanged(int index);
    void on_leDiscount_textChanged(const QString &arg1);
    void on_leStartNumber_returnPressed();

private:
    Ui::DlgCouponServiceDocumentPlus *ui;

    QString fLastNumber;
};

#endif // DLGCOUPONSERVICEDOCUMENTPLUS_H
