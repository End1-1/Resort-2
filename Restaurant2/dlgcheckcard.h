#ifndef DLGCHECKCARD_H
#define DLGCHECKCARD_H

#include "basedialog.h"

namespace Ui {
class DlgCheckCard;
}

class DlgCheckCard : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgCheckCard(QWidget *parent = nullptr);
    ~DlgCheckCard();

private slots:
    void on_leCode_returnPressed();

    void on_btnClose_clicked();

private:
    Ui::DlgCheckCard *ui;
};

#endif // DLGCHECKCARD_H
