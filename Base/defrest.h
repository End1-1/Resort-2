#ifndef DEFREST_H
#define DEFREST_H

#include "base.h"

#define defrest(x) DefRest::v(x)
#define dr_open_table_after_run "open table after run"
#define dr_first_receipt_printer "first receipt printer"
#define dr_second_receipt_printer "second receipt printer"
#define dr_discount_20 "discount 20"
#define dr_discount_50 "discount 50"
#define dr_order_number_prefix "order number prefix"

class DefRest : public Base {
public:
    DefRest(const QString &comp);
    static QString v(const QString &key);
    static QMap<QString, QString> fData;
};

#endif // DEFREST_H
