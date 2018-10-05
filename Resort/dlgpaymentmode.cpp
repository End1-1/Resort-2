#include "dlgpaymentmode.h"
#include "ui_dlgpaymentmode.h"
#include "paymentmode.h"
#include "message.h"

DlgPaymentMode::DlgPaymentMode(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgPaymentMode)
{
    ui->setupUi(this);
    fDockPayment = new DWSelectorPaymentMode(this);
    fDockPayment->configure();
    fDockPayment->setSelector(ui->lePaymentMode);
    connect(fDockPayment, &DWSelectorPaymentMode::paymentMode, [this](CI_PaymentMode *ci){
        paymentMode(ci);
    });
    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCL);
    connect(fDockCL, &DWSelectorCityLedger::cityLedger, [this](CI_CityLedger *ci) {
       dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCL, ci);
    });
    fDockCard = new DWSelectorCreditCard(this);
    fDockCard->configure();
    fDockCard->setSelector(ui->leCardType);
    connect(fDockCard, &DWSelectorCreditCard::creditCard, [this](CI_CreditCard *ci){
       dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardType, ci);
    });
}

DlgPaymentMode::~DlgPaymentMode()
{
    delete ui;
}

bool DlgPaymentMode::getPayment(int &paymentMode, QString &comment, int &cl)
{
    DlgPaymentMode *d = new DlgPaymentMode(fPreferences.getDefaultParentForMessage());
    bool result = d->exec() == QDialog::Accepted;
    if (result) {
        cl = 0;
        paymentMode = d->ui->lePaymentMode->fHiddenText.toInt();
        switch (paymentMode) {
        case PAYMENT_CARD:
            comment = d->ui->leCardType->text();
            break;
        case PAYMENT_CL:
            cl = d->ui->leCL->fHiddenText.toInt();
            comment = d->ui->leCL->text();
            break;
        }
    }
    delete d;
    return result;
}

void DlgPaymentMode::paymentMode(CI_PaymentMode *ci)
{
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentMode, ci);
    ci = CachePaymentMode::instance()->get(ui->lePaymentMode->fHiddenText);
    ui->leCardType->setEnabled(false);
    ui->leCL->setEnabled(false);
    ui->leRoom->setEnabled(false);
    if (ci) {
        switch (ci->fCode.toInt()) {
        case PAYMENT_CASH:
            break;
        case PAYMENT_CARD:
            ui->leCardType->setEnabled(true);
            break;
        case PAYMENT_CL:
            ui->leCL->setEnabled(true);
            break;
        case PAYMENT_BANK:
            break;
        case PAYMENT_ROOM:
            break;
        default:
            message_error_tr("This payment mode is not available in this case.");
            break;
        }
    }
}

void DlgPaymentMode::on_btnCancel_clicked()
{
    reject();
}

void DlgPaymentMode::on_btnOk_clicked()
{
    CI_PaymentMode *pm = CachePaymentMode::instance()->get(ui->lePaymentMode->fHiddenText);
    if (!pm) {
        message_error_tr("Payment mode is not selected");
        return;
    }
    accept();
}
