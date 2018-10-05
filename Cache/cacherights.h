#ifndef CACHERIGHTS_H
#define CACHERIGHTS_H

#include "cachebase.h"

#define cid_user_rights 42
#define RIGHT(x, y) CacheRights::instance()->get(x, y)
#define r__(x) RIGHT(WORKING_USERGROUP, x)

#define cr__room_chart 1
#define cr__edit_reservation 2
#define cr__reservations 3
#define cr__menu_reservation 4
#define cr__menu_application 5
#define cr__users_groups 6
#define cr__users 7
#define cr_trackin_changes 8
#define cr__menu_bookkeeping 9
#define cr__menu_restaurant 10
#define cr__menu_direcotory 11
#define cr__contacts 12
#define cr__partners 13
#define cr__guests 14
#define cr__credit_cards 15
#define cr__menu_cityledger 16
#define cr__none
#define cr__cityledger_balance 18
#define cr__invoice_cancelation_previouse_date 19
#define cr__invoice_cancelation_current_date 20
#define cr__global_config 21
#define cr__bookkeeper_sync 22
#define cr__unlock_locked_reservation 23
#define cr__re_checkin 24
#define cr__services 25
#define cr__update_program 26
#define cr__super_correction 27
#define cr__reception 28
#define cr__menu_cashier 29
#define cr__report_daily_movement 30
#define cr__report_monthly 31
#define cr__report_restaurant 32
#define cr__report_checkout_invoices 33
#define cr__room_editor 34
#define cr__setup_tax 35
#define cr__invoice_items 36
#define cr__report_vauchers 37
#define cr__currencies 38
#define cr__advance_vaucher 39
#define cr__receipt_vaucher 40
#define cr__postchage_vaucher 41
#define cr__transfer_vaucher 42
#define cr__discount_vaucher 43
#define cr__report_cash 44
#define cr__reserve_cancelation 45
#define cr__checkout 46
#define cr__calls_history 47
#define cr__notes 48
#define cr__report_guest 49
#define cr__o_movement 50
#define cr__o_cancelation 51
#define cr__o_print_unlimited_receipt 52
#define cr__o_print_reports 53
#define cr__o_complimentary 54
#define cr__o_breakfast 55
#define cr__o_recover_order 56
#define cr__o_banquet 57
#define cr__o_58
#define cr__o_59
#define cr__eod 60
#define cr__analytics_menu 61
#define cr__call_in 62
#define cr__call_out 63
#define cr__call_int 64
#define cr__call_tout 65
#define cr__call_tin 66
#define cr__cardex_analysis 67
#define cr__print_reports_any_day 68
#define cr__airpickup_birthday 69
#define cr__export_event_etc 70
#define cr__export_active_reservations 71
#define cr__remarks 72
#define cr__print_receipt_by_umber 73
#define cr__hotel_status 74
#define cr__reports_set 75
#define cr__remove_restaurant 76
#define cr__export_single_vauchers 77
#define cr__print_any_tax 78
#define cr__online_restaurant 79
#define cr__move_entry_checkout_invoice 80
#define cr__menu_restaurant_reports 81
#define cr__reservation_cancelation_no_show 82
#define cr__reservation_changes_state_room 83
#define cr__reception_room_inventory 84
#define cr__cityledger_avaiable_amounts 85
#define cr__storehouse_all_items 86
#define cr__directory_hotel_room_inventory 87
#define cr__directory_hotel_room_inventory_state 88
#define cr__invoice_export_to_excel 89
#define cr__reservation_group_reservation 90
#define cr__cashier_advance_report 91
#define cr__reseption_password_need 92
#define cr__rv_change_date_bank 93
#define cr__rv_change_date_cash 94
#define cr__rv_change_date_cl 95
#define cr__rv_change_date_card 96
#define cr__tracking_chnages_individual 97
#define cr__list_of_checking_guests 98
#define cr__reserv_lenght_of_stay 99
#define cr__email_confirmation 100
#define cr__cashier_refund 101
#define cr__cashier_transfer_any_direction 102
#define cr__reservation_category_to_sell 103
#define cr__reservation_forecast_occupation 104
#define cr__reservatoin_list_of_source 105
#define cr__reservation_avaiable_room 106
#define cr__bookeeping_guest_tray_ledger 107
#define cr__bookeeping_inhouse_detailed 108
#define cr__bookeeping_yearly_financial_report 109
#define cr__analytics_monthly_occupancy_perc 110
#define cr__analytics_guest_by_nationality 111
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
#define cr__1
typedef struct : CI_Base {
    bool fFlag;
} CI_UserRights;
Q_DECLARE_METATYPE(CI_UserRights*)

class CacheRights : public CacheBase<CI_UserRights>
{
public:
    static CacheRights *instance();
    virtual void load();
    virtual void clear();
    bool get(int group, int code);
private:
    CI_UserRights fEmpty;
    static CacheRights *fInstance;
    CacheRights();
    ~CacheRights();
    CI_UserRights *get(const QString &code);
    CI_UserRights *get(int code);
    static QMap<int, QMap<int, int> > fCacheRights;
};

#endif // CACHERIGHTS_H
