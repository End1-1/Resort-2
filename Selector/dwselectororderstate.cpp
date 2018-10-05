#include "dwselectororderstate.h"

DWSelectorOrderState::DWSelectorOrderState(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Order state"));
}

void DWSelectorOrderState::configure()
{
    simpleTable<CI_OrderState, CacheOrderState>();
}
