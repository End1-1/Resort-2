#ifndef DLGCOUPONSERVICEFIRSTNUMBER_H
#define DLGCOUPONSERVICEFIRSTNUMBER_H

#include <QDialog>

namespace Ui {
class DlgCouponServiceFirstNumber;
}

class DlgCouponServiceFirstNumber : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCouponServiceFirstNumber(QWidget *parent = nullptr);
    ~DlgCouponServiceFirstNumber();
    static bool getFirstNumber(int type, QString &first, QString &last, double &qty, QWidget *parent);

private slots:
    void on_tbl_doubleClicked(const QModelIndex &index);

private:
    Ui::DlgCouponServiceFirstNumber *ui;
    void setup(int type);
    int fRow;
};

#endif // DLGCOUPONSERVICEFIRSTNUMBER_H
