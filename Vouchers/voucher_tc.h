#ifndef VOUCHER_TC_H
#define VOUCHER_TC_H

#include "basevoucher.h"

class voucher_tc : public BaseVoucher
{
public:
    voucher_tc();
    virtual void saveVoucher();
protected:
    virtual bool loadAdditionalData();
};

#endif // VOUCHER_TC_H
