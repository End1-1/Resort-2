#include "dwselectorresttable.h"

DWSelectorRestTable::DWSelectorRestTable(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Tables"));
    fHall = 0;
}

void DWSelectorRestTable::configure()
{
    simpleTable<CI_RestTable, CacheRestTable>();
}
