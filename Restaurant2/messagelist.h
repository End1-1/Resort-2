#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QString>

#define op_login 1
#define op_get_hall_list 2
#define op_get_table_list 3
#define op_get_dish_part1_list 4
#define op_get_dish_part2_list 5
#define op_get_dish_dish_list 6
#define op_login_passhash 7
#define op_open_table 8
#define op_unlock_table 9
#define op_car_list 10
#define op_search_platenumber 11
#define op_create_order_header 12
#define op_set_car 13
#define op_open_order 14
#define op_get_car 15
#define op_dish_menu 16
#define op_add_dish_to_order 17
#define op_remove_dish_from_order 18
#define op_modify_order_dish 19
#define op_print_service 20
#define op_login_pin 21
#define op_update_tables 22

namespace MessageList {
    const short hello = 1;
    const short coordinate = 2;
    const short yandex_map_key = 3;
    const short silent_auth = 4;
    const short register_phone = 5;
    const short register_sms = 6;
    const short balance_history = 7;
    const short monitor = 8;
    const short car_near = 9;
    const short dll_op = 10;
    const short dll_plugin = 11;

    const short srv_connections_count = 100;
    const short srv_device_position = 101;

    const QString waiterclientp = "8b90e61a-1385-4fb4-85ce-f23849045e69";
}

#endif // MESSAGELIST_H
