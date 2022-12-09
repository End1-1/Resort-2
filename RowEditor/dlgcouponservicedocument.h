#ifndef DLGCOUPONSERVICEDOCUMENT_H
#define DLGCOUPONSERVICEDOCUMENT_H

#include <QDialog>

class ReportQuery;

namespace Ui {
class DlgCouponServiceDocument;
}

class DlgCouponServiceDocument : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCouponServiceDocument(ReportQuery *rq, QWidget *parent = nullptr);
    ~DlgCouponServiceDocument();
    void openDocument(int id);

private slots:
    void on_btnPlus_clicked();
    void on_btnSave_clicked();
    void on_btnPrint_clicked();
    void on_btnMinus_clicked();
    void on_lePaid_textChanged(const QString &arg1);

private:
    Ui::DlgCouponServiceDocument *ui;
    ReportQuery *fReportQuery;
    void countAmount();
};

#endif // DLGCOUPONSERVICEDOCUMENT_H
