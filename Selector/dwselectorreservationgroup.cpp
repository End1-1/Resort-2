#include "dwselectorreservationgroup.h"

DWSelectorReservationGroup::DWSelectorReservationGroup(QWidget *parent) :
    DWSelector(cid_reserve_group, parent)
{
    setWindowTitle(tr("Reservation group"));
}

void DWSelectorReservationGroup::configure()
{
    simpleTable<CI_ReserveGroup, CacheReserveGroup>();
}

void DWSelectorReservationGroup::select(const QVariant &value)
{
    emit reserveGroup(value.value<CI_ReserveGroup*>());
}

void DWSelectorReservationGroup::selectorFocusOut()
{
    CI_ReserveGroup *ci = 0;
    if (fLineSelector) {
        ci = CacheReserveGroup::instance()->get(fLineSelector->text());
    }
    emit reserveGroup(ci);
}
