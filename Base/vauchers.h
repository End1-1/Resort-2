#ifndef VAUCHERS_H
#define VAUCHERS_H

#include "defines.h"
#include "cachecityledger.h"
#include "cachecreditcard.h"
#include <QString>

#define VAUCHER_ADVANCE 2
#define VAUCHER_RECEIPT 9
#define VAUCHER_RESERVATION 13
#define VAUCHER_ROOM_RATE 3
#define VAUCHER_DISCOUNT 11
#define VAUCHER_CHECKOUT 7
#define VAUCHER_TRANSFER_AMOUNT 5
#define VAUCHER_POSTCHARGE 10
#define VAUCHER_POINT_SALE 4
#define VAUCHER_REFUND 15
#define VAUCHER_EVENT 14
#define VAUCHER_RECOVER 16
#define VAUCHER_CALL 17
#define VAUCHER_INVOICE 18
#define VAUCHER_TRANSFER_CL 80

#define VAUCHER_ADVANCE_N "AV"
#define VAUCHER_RECEIPT_N "RV"
#define VAUCHER_ROOMING_N "RM"
#define VAUCHER_RESERVATION_N "RS"
#define VAUCHER_ROOM_RATE_N "RC"
#define VAUCHER_DISCOUNT_N "DS"
#define VAUCHER_CHECKOUT_N "CO"
#define VAUCHER_TRANSFER_AMOUNT_N "TR"
#define VAUCHER_POSTCHARGE_N "CH"
#define VAUCHER_POINT_SALE_N "PS"
#define VAUCHER_REFUND_N "RF"
#define VAUCHER_EVENT_N "PE"
#define VAUCHER_RECOVER_N "RR"
#define VAUCHER_CALL_N "CM"
#define VAUCHER_INVOICE_N "IN"
#define VAUCHER_TRANSFER_CL_N "TC"
#define VAUCHER_CL_INIT_N "CR"

QString vaucherPaymentName(int code, const QString &cardcode, const QString &clcode);
QString vaucherCode(int code, const QString &prefix);
bool isTaxPay(const QString &code);
bool isTaxPrepay(const QString &code);
bool openVaucherInvoice(const QString &vaucherId);
bool openInvoiceWithId(const QString &invoice);
void openVaucher(const QString &vaucher, const QString &id);
bool removeVaucher(const QString &id, const QString &reason);

#endif // VAUCHERS_H
