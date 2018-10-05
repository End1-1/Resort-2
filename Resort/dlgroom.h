#ifndef DLGROOM_H
#define DLGROOM_H

#include "roweditordialog.h"
#include "dwselectorroomcategory.h"
#include "dwselectorbed.h"
#include "dwselectorroomview.h"

namespace Ui {
class DlgRoom;
}

class DlgRoom : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit DlgRoom(QList<QVariant> &values, QWidget *parent = 0);
    ~DlgRoom();
    virtual void setValues();
private slots:
    void roomCategory(CI_RoomCategory *c);
    void roomView(CI_RoomView *c);
    void bed(CI_Bed *c);
    void on_btnCancel_clicked();
    void on_btnOK_clicked();

private:
    Ui::DlgRoom *ui;
    DWSelectorRoomCategory *fDockCat;
    DWSelectorBed *fDockBed;
    DWSelectorRoomView *fDockView;

};

#endif // DLGROOM_H
