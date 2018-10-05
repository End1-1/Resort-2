#include "dwselectorresttable.h"

DWSelectorRestTable::DWSelectorRestTable(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Tables"));
}

void DWSelectorRestTable::configure()
{
    simpleTable<CI_RestTable, CacheRestTable>();
}
