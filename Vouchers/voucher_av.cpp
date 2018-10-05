#include "voucher_av.h"
#include "broadcastthread.h"
#include "cacheredreservation.h"

voucher_av::voucher_av()
{

}

void voucher_av::saveVoucher()
{
    save();
    BroadcastThread::cmdRefreshCache(cid_red_reservation, get("f_res").toString());
}

bool voucher_av::loadAdditionalData()
{
    return true;
}
