#ifndef DLGGROUPPARAMS_H
#define DLGGROUPPARAMS_H

#include "baseextendeddialog.h"
#include "dwselectorcardex.h"
#include "dwselectorguest.h"

namespace Ui {
class DlgGroupParams;
}

class DlgGroupParams : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgGroupParams(QWidget *parent = 0);
    ~DlgGroupParams();
    void addRoom(const QString &room);
    void setGroupId(int group);
    void setDate(const QDate &d1, const QDate &d2);
    QStringList checkedRooms();
    bool roomChecked(const QString &room);
    bool date(QDate &d1, QDate &d2);
    bool cardex(QVariant &car);
    bool arrangement(QVariant &arr);
    bool guest(QVariant &guest);
    bool remarks(QVariant &rem);
    void prepareForCreate();
private slots:
    void on_btnCancel_clicked();
    void on_chAllRoom_clicked(bool checked);
    void on_btnSave_clicked();

    void on_btnAddGuest_clicked();

    void on_leGuest_returnPressed();

    void on_chAllCheckBox_clicked(bool checked);

    void on_leGuest_textChanged(const QString &arg1);

    void on_leCardexCode_textChanged(const QString &arg1);

    void on_cbArrangement_currentIndexChanged(int index);

    void on_teRemarks_textChanged();

private:
    Ui::DlgGroupParams *ui;
    DWSelectorCardex *fDockCardex;
    DWSelectorGuest *fDockGuest;
};

#endif // DLGGROUPPARAMS_H
