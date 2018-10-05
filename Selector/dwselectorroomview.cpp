#include "dwselectorroomview.h"

DWSelectorRoomView::DWSelectorRoomView(QWidget *parent) :
    DWSelector(cid_room_view, parent)
{
    setWindowTitle(tr("Room view"));
}

void DWSelectorRoomView::configure()
{
    simpleTable<CI_RoomView, CacheRoomView>();
}

void DWSelectorRoomView::select(const QVariant &value)
{
    emit roomView(value.value<CI_RoomView*>());
}

void DWSelectorRoomView::selectorFocusOut()
{
    emit roomView(0);
}
