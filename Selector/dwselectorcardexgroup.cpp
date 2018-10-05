#include "dwselectorcardexgroup.h"

DWSelectorCardexGroup::DWSelectorCardexGroup(QWidget *parent) :
    DWSelector(cid_cardex_group, parent)
{
    setWindowTitle(tr("Cardex group"));
}

void DWSelectorCardexGroup::configure()
{
    simpleTable<CI_CardexGroup, CacheCardexGroup>();
}

void DWSelectorCardexGroup::select(const QVariant &value)
{
    emit cardexGroup(value.value<CI_CardexGroup*>());
}

void DWSelectorCardexGroup::selectorFocusOut()
{
    CI_CardexGroup *c = 0;
    if (fLineSelector) {
        c = CacheCardexGroup::instance()->get(fLineSelector->text());
    }
    emit cardexGroup(c);
}
