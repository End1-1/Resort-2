#ifndef DLGGIFTCARTSTATUS_H
#define DLGGIFTCARTSTATUS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgGiftCartStatus;
}

class DlgGiftCartStatus : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgGiftCartStatus(int cardId, const QString &cardCode = QString(), QWidget *parent = nullptr);
    ~DlgGiftCartStatus();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    bool loadCard();
    void populateStatuses();

    Ui::DlgGiftCartStatus *ui;
    int fCardId;
    QString fCardCode;
    int fCurrentStatus;
    bool fSold;
};

#endif // DLGGIFTCARTSTATUS_H
