#ifndef DLGROOMVIEW_H
#define DLGROOMVIEW_H

#include "roweditordialog.h"

namespace Ui {
class DlgRoomView;
}

class DlgRoomView : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit DlgRoomView(QList<QVariant> &values, QWidget *parent = 0);
    ~DlgRoomView();
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
private:
    Ui::DlgRoomView *ui;
};

#endif // DLGROOMVIEW_H
