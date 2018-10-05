#include "dwselectorroomstate.h"

DWSelectorRoomState::DWSelectorRoomState(QWidget *parent) :
    DWSelector(cid_room_state, parent)
{
    setWindowTitle(tr("Room state"));
}

void DWSelectorRoomState::configure()
{
    simpleTable<CI_RoomState, CacheRoomState>();
}

void DWSelectorRoomState::select(const QVariant &value)
{
    emit roomState(value.value<CI_RoomState*>());
}

void DWSelectorRoomState::selectorFocusOut()
{
    CI_RoomState *c = 0;
    if (fLineSelector) {
        c = CacheRoomState::instance()->get(fLineSelector->text());
    }
    emit roomState(c);
}
