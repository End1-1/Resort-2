#include "recreditcard.h"
#include "ui_recreditcard.h"
#include "cachecreditcard.h"

RECreditCard::RECreditCard(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_CREDIT_CARD, parent),
    ui(new Ui::RECreditCard)
{
    ui->setupUi(this);
    fTable = "f_credit_card";
    fCacheId = cid_credit_card;
    addWidget(ui->leCode, "Code")
            .addWidget(ui->leName, "Name");
}

RECreditCard::~RECreditCard()
{
    delete ui;
}

void RECreditCard::on_btnCancel_clicked()
{
    reject();
}

void RECreditCard::on_btnOk_clicked()
{
    save();
}
