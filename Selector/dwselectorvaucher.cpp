#include "dwselectorvaucher.h"

DWSelectorVaucher::DWSelectorVaucher(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Voucher type"));
}

void DWSelectorVaucher::configure()
{
    simpleTable<CI_Vaucher, CacheVaucher>();
}
