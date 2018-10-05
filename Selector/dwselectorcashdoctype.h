#ifndef DWSELECTORCASHDOCTYPE_H
#define DWSELECTORCASHDOCTYPE_H

#include "dwtemplateselector.h"
#include "cachecashdoc.h"

class DWSelectorCashDocType : public DWTemplateSelector<CI_CashDoc, CacheCashDoc>
{
    Q_OBJECT
public:
    DWSelectorCashDocType(QWidget *parent = 0);
};

#endif // DWSELECTORCASHDOCTYPE_H
