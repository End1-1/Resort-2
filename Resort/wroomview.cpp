#include "wroomview.h"
#include "dlgroomview.h"

WRoomView::WRoomView(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(0, "f_id", tr("Code"))
            .setColumn(50, "f_short", tr("Title"))
            .setColumn(tr_col("am", 200), "f_am", tr("Description"))
            .setColumn(tr_col("en", 200), "f_en", tr("Description"))
            .setColumn(tr_col("ru", 200), "f_ru", tr("Description"));

    fModel->setSqlQuery("select f_id, f_short, f_am, f_en, f_ru from f_room_view");
    fModel->apply(this);

    fRowEditorDialog = new DlgRoomView(fRowValues, this);
    setBtnNewVisible();
}

void WRoomView::setupTab()
{
    setupTabTextAndIcon(tr("Room view"), ":/images/window.png");
}
