#ifndef VOUCHER_AV_H
#define VOUCHER_AV_H

#include "basevoucher.h"

class voucher_av : public BaseVoucher
{
public:
    voucher_av();
    virtual void saveVoucher();
protected:
    virtual bool loadAdditionalData();
};

#endif // VOUCHER_AV_H
