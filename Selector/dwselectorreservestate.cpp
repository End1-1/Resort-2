#include "dwselectorreservestate.h"

DWSelectorReserveState::DWSelectorReserveState(QWidget *parent) :
    DWSelector(cid_reserve_state, parent)
{
    setWindowTitle(tr("State"));
}

void DWSelectorReserveState::configure()
{
    simpleTable<CI_ReserveState, CacheReserveState>();
}

void DWSelectorReserveState::select(const QVariant &value)
{
    emit reserveState(value.value<CI_ReserveState*>());
}

void DWSelectorReserveState::selectorFocusOut()
{
    emit reserveState(0);
}
