#ifndef DWSELECTORCASHDESK_H
#define DWSELECTORCASHDESK_H

#include "dwtemplateselector.h"
#include "cachecashdesk.h"

class DWSelectorCashDesk : public DWTemplateSelector<CI_CashDesk, CacheCashDesk>
{
    Q_OBJECT
public:
    DWSelectorCashDesk(QWidget *parent = 0);
};

#endif // DWSELECTORCASHDESK_H
