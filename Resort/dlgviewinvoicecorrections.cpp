#include "dlgviewinvoicecorrections.h"
#include "ui_dlgviewinvoicecorrections.h"

DlgViewInvoiceCorrections::DlgViewInvoiceCorrections(const QString &invoice, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgViewInvoiceCorrections)
{
    ui->setupUi(this);
    fDbBind[":f_inv"] = invoice;
    fDb.select("select m.f_id, f_wdate, u1.f_username, f_finalName, f_amountAmd, u2.f_username, f_cancelDate, f_cancelReason "
               "from m_register m "
               "left join users u1 on u1.f_id=f_user "
               "left join users u2 on u2.f_id=f_cancelUser "
               "where m.f_inv=:f_inv and f_canceled=1", fDbBind, fDbRows);
    ui->tblData->setColumnCount(8);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  QString("Voucher"), QString("Date"),  QString("Operator"),
                                  QString("Description"), QString("Amount"), QString("Cancel by"),
                                  QString("Cancel date"), QString("Reason"));
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                80, 100, 150, 200, 80, 150, 140, 200);
    Utils::fillTableWithData(ui->tblData, fDbRows);
}

DlgViewInvoiceCorrections::~DlgViewInvoiceCorrections()
{
    delete ui;
}
