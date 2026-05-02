#ifndef DLGCALC_H
#define DLGCALC_H

#include <QDialog>

namespace Ui {
class DlgCalc;
}

class DlgCalc : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCalc(int dish, QWidget *parent = nullptr);
    ~DlgCalc();

private slots:
    void on_btnClose_clicked();

private:
    Ui::DlgCalc *ui;
};

#endif // DLGCALC_H
