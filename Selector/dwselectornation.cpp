#include "dwselectornation.h"

DWSelectorNation::DWSelectorNation(QWidget *parent) :
    DWSelector(cid_nation, parent)
{
    setWindowTitle(tr("Nation"));
}

void DWSelectorNation::configure()
{
    simpleTable<CI_Nation, CacheNation>();
}

void DWSelectorNation::select(const QVariant &value)
{
    emit nation(value.value<CI_Nation*>());
}

void DWSelectorNation::selectorFocusOut()
{
    CI_Nation *c = 0;
    if (fLineSelector) {
        c = CacheNation::instance()->get(fLineSelector->text());
    }
    emit nation(c);
}
