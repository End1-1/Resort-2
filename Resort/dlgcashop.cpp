#include "dlgcashop.h"
#include "ui_dlgcashop.h"
#include "cachecashdoc.h"

DlgCashOp::DlgCashOp(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgCashOp)
{
    ui->setupUi(this);
    ui->wAcc->enable();
    connect(ui->wAcc, &WCashEntry::accepted, [this]() {
        QString err;
        if (!ui->wAcc->isCorrent(err)) {
            message_error(err);
            return;
        }
        ui->wAcc->save();
        accept();
    });
    connect(ui->wAcc, &WCashEntry::rejected, [this]() {
        reject();
    });
}

DlgCashOp::~DlgCashOp()
{
    delete ui;
}

void DlgCashOp::cash(int doc)
{
    DlgCashOp *d = new DlgCashOp(fPreferences.getDefaultParentForMessage());
    if (doc > 0) {
        d->load(doc);
    }
    d->exec();
    delete d;
}

void DlgCashOp::cashWithParams(int docType, int partner)
{
    DlgCashOp *d = new DlgCashOp(fPreferences.getDefaultParentForMessage());
    d->ui->wAcc->docType(docType);
    d->ui->wAcc->partner(partner);
    d->exec();
    delete d;
}

void DlgCashOp::load(int doc)
{
    ui->wAcc->load(doc);
}
