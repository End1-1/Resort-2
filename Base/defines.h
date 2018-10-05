#ifndef DEFINES_H
#define DEFINES_H

#include <QObject>
#include <QDate>
#include <QHostInfo>

#define def_home_path "def_home_path"
#define def_preferences_file "def_preferences_file"
#define def_last_db_combo_index "def_last_db_combo_index"
#define def_last_username "def_last_username"
#define def_last_datebase "def_last_database"
#define def_preferences_password "def_preferences_password"
#define def_working_conn_name "def_working_conn_name"
#define def_working_username "def_user_full_name"
#define def_working_user_id "def_working_user_id"
#define def_working_user_group "def_working_user_group"
#define def_app_font "def_app_font"
#define def_tax_address "def_tax_address"
#define def_tax_port "def_tax_port"
#define def_tax_password "def_tax_password"
#define def_tax_adg "def_tax_adg"
#define def_vaucher_font_name "Arial"
#define def_vaucher_font_size 25
#define def_breakfast_default_hall "default hall for breakfast"
#define def_breakfast_default_table "default table for breakfast"
#define def_breakfast_default_dish "default dish for breakfast"
#define def_minibar_default_hall "default hall for minibar"
#define def_minibar_default_table "default table for minibar"
#define def_minibar_default_dish "default dish for minibar"
#define def_passport_required "default passport required"
#define def_rooming_list "rooming_list"

#define def_no_tracking_changes "No tracking changes"
#define def_working_day "Working date"
#define def_default_extra_bed "Default extra bed"
#define def_room_charge_id "Room charge id"
#define def_default_cardex "Default cardex"
#define def_default_menu "Default menu"
#define def_default_hall "Default hall"
#define def_slogan "Slogan"
#define def_earyly_checkin "Early checkin before"
#define def_earyly_checkin_id "Early checkin id"
#define def_late_checkout "Late checkout after"
#define def_late_checkout_id "Late checkout id"
#define def_airport_pickup_id "Airport pickup id"
#define def_day_use_id "Day use id"
#define def_vouchers_digits "Vouchers number digits"
#define def_vouchers_right_header "Vouchers right header"
#define def_vouchers_invoice_footer "Vouchers invoice footer"
#define def_auto_breakfast_id "Auto breakfast id"
#define def_invoice_default_discount_id "Invoice, default dicouont id"
#define def_invoice_default_positive_transfer_id "Invoice, default positive transfer id"
#define def_invoice_default_negative_transfer_id "Invoice, default negative transfer id"
#define def_invoice_default_refund_id "Invoice, default refund id"
#define def_filter_manual_tax "Manula tax available items"
#define def_welcome_rest_mode "Welcome restaurant mode"
#define def_cancelfee_code "Cancelation fee code"
#define def_noshowfee_code "No show fee code"
#define def_rest_hall_for_reception "Restaurant halls for reception"
#define def_room_arrangement "Default room arrangement"
#define def_id_generator_style "ID generator style"
#define def_receip_vaucher_id "Receipt vaucher id"
#define def_reservation_voucher_id "Reservation voucher id"
#define def_room_rate_change_id "Room rate change voucher id"
#define def_advance_voucher_id "Advance voucher id"
#define def_daily_movement_items "Daily movement items"
#define def_daily_movement_total_side "Daily movement total side"
#define def_daily_movement_order "Daily movement order"
#define def_show_logs "show_logs"

#define INVOICE_HOTEL 1
#define DATAGRAM_PORT 33110
#define HOSTNAME QHostInfo().localHostName()

#define VAT_INCLUDED 1
#define VAT_WITHOUT 2
#define VAT_NOVAT 3

#define RESERVE_NONE 0
#define RESERVE_CHECKIN 1
#define RESERVE_RESERVE 2
#define RESERVE_CHECKOUT 3
#define RESERVE_OUTOFROOM 4
#define RESERVE_DIRTY 5
#define RESERVE_REMOVED 6
#define RESERVE_SERVICE 7
#define RESERVE_SERVICE_REMOVED 8
#define RESERVE_OUTOFINVENTORY 9

#define CONFIRM_CONFIRM 1
#define CONFIRM_GUARANTEED 2
#define CONFIRM_TENTATIVE 3
#define CONFIRM_WAITING 4
#define CONFIRM_BLOCK 5

#define ROOM_STATE_NONE 0
#define ROOM_STATE_CHECKIN 1
#define ROOM_STATE_DIRTY 2
#define ROOM_STATE_OUTOF 3
#define ROOM_STATE_HOUSE_USER 4
#define ROOM_STATE_COMPLIMENTARY 5
#define ROOM_STATE_OUTINVENTORY 6

#define INVOICE_OPEN 1

#define ORDER_STATE_OPENED 1
#define ORDER_STATE_CLOSED 2
#define ORDER_STATE_REMOVED 3
#define ORDER_STATE_MOVED 4
#define ORDER_STATE_EMPTY 5

#define DISH_STATE_READY 1
#define DISH_STATE_REMOVED_STORE 2
#define DISH_STATE_REMOVED_NOSTORE 3
#define DISH_STATE_MOVED 4
#define DISH_STATE_EMPTY 5

#define R_REMOVE_PRINTER_ITEM 1
#define R_MOVE_TABLE 2
#define R_MOVE_ITEM 3

#define IG_POSTING_CHARGES 1
#define IG_PAYMENT 2
#define IG_REFUND 3
#define IG_DISCOUNT 4
#define IG_POSITIVE_BALANCE 5
#define IG_NEGATIVE_BALANCE 6

#define ARRANGEMENT_BO 1
#define ARRANGEMENT_BB 2
#define ARRANGEMENT_HB 3
#define ARRANGEMENT_FB 4

//#define IT_PAYMENT  30
//#define IT_SETTLEMENT 31
//#define IT_RECEIPT 39
//#define IT_ADVANCE 33
//#define IT_DISCOUNT 40
//#define IT_RESERVATION 32
//#define IT_ROOM_RATE_CHANGED 34
//#define IT_TRANSFER 36
//#define IT_TRANSFER_POS 27
//#define IT_TRANSFER_NEG 28
//#define IT_CHECKOUT 38
//#define IT_RECOVER 41
//#define IT_RECEIPT_ADVANCE
//#define IT_REFUND 42
//#define IT_TRANSFER_AMOUNT 951

#define REF_RESERVATION_MEAL 1

extern QString def_date_time_format;
extern QString def_date_format;
extern QString def_time_format;
#define def_mysql_date_format "yyyy-MM-dd"
#define def_mysql_datetime_format "yyyy-MM-dd HH:mm:ss"
extern QString def_lang;
extern double def_vat;
extern double def_usd;
extern QDate def_date;
extern QString def_station;

#endif // DEFINES_H
