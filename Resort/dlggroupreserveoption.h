#ifndef DLGGROUPRESERVEOPTION_H
#define DLGGROUPRESERVEOPTION_H

#include "basedialog.h"

namespace Ui {
class DlgGroupReserveOption;
}

class DlgGroupReserveOption : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgGroupReserveOption(QWidget *parent = 0);
    ~DlgGroupReserveOption();
    static void addRemove(const QString &id);
    virtual void selector(int selectorNumber, const QVariant &value);
private slots:
    void on_btnAddToGroup_clicked();

    void on_bgtnRemoveFromGroup_clicked();

private:
    Ui::DlgGroupReserveOption *ui;
};

#endif // DLGGROUPRESERVEOPTION_H
