#ifndef CACHERIGHTS_H
#define CACHERIGHTS_H

#include <QObject>

#define check_permission(x) UserPermssions::checkpermission(x)

#define pr_view_revenue 1
#define pr_remove_order 2
#define pr_cashbox 3
#define pr_attendance 4
#define pr_edit_store_doc 5
#define pr_store_doc_journal 6
#define pr_make_store_output_of_sale 7
#define pr_goods_list 8
#define pr_edit_goods_list 9
#define pr_coupon_sale  10
#define pr_edit_attendance_list 11
#define pr_coupon_view 12
#define pr_indexes 13
#define pr_global_config 14
#define pr_edit_cash_doc 15
#define pr_hall_manager 16
#define pr_partners 17

class UserPermssions: public QObject
{
    Q_OBJECT
public:
    UserPermssions(int group);

    static void init(int group);

    static bool checkpermission(int permission);

private:
    int fGroup;

    QList<int> fPermissions;

    static UserPermssions* fInstance;
};

#endif // CACHERIGHTS_H
