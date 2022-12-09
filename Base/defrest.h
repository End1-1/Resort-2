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
#define dr_s5_ip "s5ip"
#define dr_s5_port "s5port"
#define dr_s5_user "s5user"
#define dr_s5_pass "s5pass"
#define dr_branch "branch"

class DefRest : public Base {
public:
    DefRest(const QString &comp);
    static QString v(const QString &key);
    static QMap<QString, QString> fData;
};

#endif // DEFREST_H
