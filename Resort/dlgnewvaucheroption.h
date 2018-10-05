#ifndef DLGNEWVAUCHEROPTION_H
#define DLGNEWVAUCHEROPTION_H

#include "basedialog.h"

namespace Ui {
class DlgNewVaucherOption;
}

#define VO_RV 1
#define VO_PS 2
#define VO_MANUAL 3

class DlgNewVaucherOption : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgNewVaucherOption(QWidget *parent = 0);
    ~DlgNewVaucherOption();
    static int vaucherOption();
private slots:
    void on_btnCancel_clicked();

    void on_btnRV_clicked();

    void on_btnPostCharge_clicked();

    void on_btnManual_clicked();

private:
    Ui::DlgNewVaucherOption *ui;
};

#endif // DLGNEWVAUCHEROPTION_H
