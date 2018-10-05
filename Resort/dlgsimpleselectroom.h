#ifndef DLGSIMPLESELECTROOM_H
#define DLGSIMPLESELECTROOM_H

#include "baseextendeddialog.h"
#include "cacheroom.h"

namespace Ui {
class DlgSimpleSelectRoom;
}

class DlgSimpleSelectRoom : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgSimpleSelectRoom(QWidget *parent = 0);
    ~DlgSimpleSelectRoom();
    int getRoom(const QString &cat, const QDate &d1, const QDate &d2, QStringList exclude, const QString &bed, bool v);
    double price();
    CI_Room *fRoom;
    void getRoomsList(QList<CI_Room*> &rooms);
    bool fSingleSelection;
private slots:
    void on_tblData_doubleClicked(const QModelIndex &index);
    void on_tblSmoke_clicked(const QModelIndex &index);
    void on_tblFloor_clicked(const QModelIndex &index);
    void on_tblBed_clicked(const QModelIndex &index);
    void on_btnClearRoomFilter_clicked();
    void on_btnMultiSelect_clicked();
    void on_tblData_clicked(const QModelIndex &index);

private:
    QString fBed;
    int fFloor;
    int fSmoke;
    QDate fDate1;
    QDate fDate2;
    QStringList fExclude;
    Ui::DlgSimpleSelectRoom *ui;
    void filter();
    void filter2();
};

#endif // DLGSIMPLESELECTROOM_H
