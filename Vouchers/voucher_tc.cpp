#include "voucher_tc.h"
#include "basevoucher.h"
#include "broadcastthread.h"
#include "cachecladvance.h"

voucher_tc::voucher_tc() :
    BaseVoucher()
{

}

void voucher_tc::saveVoucher()
{
    save();
    BroadcastThread::cmdRefreshCache(cid_cache_cl_advance, fValues["f_cityLedger"].toString());
}

bool voucher_tc::loadAdditionalData()
{
    return true;
}
