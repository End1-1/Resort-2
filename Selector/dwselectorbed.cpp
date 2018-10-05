#include "dwselectorbed.h"

DWSelectorBed::DWSelectorBed(QWidget *parent) :
    DWSelector(cid_bed, parent)
{
    setWindowTitle(tr("Bed"));
}

void DWSelectorBed::configure()
{
    simpleTable<CI_Bed, CacheBed>();
}

void DWSelectorBed::select(const QVariant &value)
{
    emit bed(value.value<CI_Bed*>());
}

void DWSelectorBed::selectorFocusOut()
{
    emit bed(0);
}
