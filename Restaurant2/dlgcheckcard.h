#ifndef DLGCHECKCARD_H
#define DLGCHECKCARD_H

#include <QDialog>

namespace Ui {
class DlgCheckCard;
}

class DlgCheckCard : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCheckCard(QWidget *parent = nullptr);
    ~DlgCheckCard();

private:
    Ui::DlgCheckCard *ui;
};

#endif // DLGCHECKCARD_H
