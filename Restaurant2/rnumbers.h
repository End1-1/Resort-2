#ifndef RNUMBERS_H
#define RNUMBERS_H

#include "baseextendeddialog.h"

namespace Ui {
class RNumbers;
}

class RNumbers : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RNumbers(QWidget *parent = 0);
    ~RNumbers();
    static bool getNumber(float &number, float max, QWidget *parent = 0);
private:
    Ui::RNumbers *ui;
    float fMax;
private slots:
    void btnNumClick();
    void on_btnClear_clicked();
    void on_btnReject_clicked();
    void on_btnAccept_clicked();
};

#endif // RNUMBERS_H
