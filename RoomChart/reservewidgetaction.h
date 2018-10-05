#ifndef RESERVEWIDGETACTION_H
#define RESERVEWIDGETACTION_H

#include "basedialog.h"
#include "cachereservation.h"

namespace Ui {
class ReserveWidgetAction;
}

class ReserveWidgetAction : public BaseDialog
{
    Q_OBJECT

public:
    explicit ReserveWidgetAction(QWidget *parent = 0);
    ~ReserveWidgetAction();
    int fResult;
    void setAuthor(const QString &text);
    void setRemarks(const QString &text);
    void setReservation(CI_Reservation *r);
    void lock(int lock);
    QString remarks();
private slots:
    void on_btnCancel_clicked();

    void on_btnEdit_clicked();

    void on_btnLock_clicked();

    void on_btnUnlock_clicked();

    void on_btnRemove_clicked();

    void on_btnOk_clicked();

    void on_btnOutOfOrder_clicked();

    void on_btnOutOfInventory_clicked();

private:
    Ui::ReserveWidgetAction *ui;
    void result(int r);
    int fLock;
    CI_Reservation *fReservation;
};

#endif // RESERVEWIDGETACTION_H
