#ifndef DLGCOUPONSERVICEBACK_H
#define DLGCOUPONSERVICEBACK_H

#include <QDialog>

namespace Ui
{
class DlgCouponServiceBack;
}

class ReportQuery;

class DlgCouponServiceBack : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCouponServiceBack(ReportQuery *rq, QWidget *parent = nullptr);
    ~DlgCouponServiceBack();
    void openDoc(int id);
    virtual void keyPressEvent(QKeyEvent *e);

private slots:
    void on_leCode_returnPressed();

    void on_btnSave_clicked();

private:
    Ui::DlgCouponServiceBack *ui;
    void countAmount();
};

#endif // DLGCOUPONSERVICEBACK_H
