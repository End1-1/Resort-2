#include "ftaxreturn.h"
#include "ui_ftaxreturn.h"
#include "wreportgrid.h"
#include "dlggposorderinfo.h"

FTaxReturn::FTaxReturn(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FTaxReturn)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("HDM return"), ":/images/refund.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
}

FTaxReturn::~FTaxReturn()
{
    delete ui;
}

QString FTaxReturn::reportTitle()
{
    return tr("HDM return %1 - %2").arg(ui->wd->dss1(), ui->wd->dss2());
}

QWidget *FTaxReturn::firstElement()
{
    return ui->wd->fw();
}

void FTaxReturn::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(140, "", tr("Date/time"))
            .setColumn(80, "", tr("Order"))
            .setColumn(100, "", tr("Returned receipt"))
            .setColumn(100, "", tr("CRN"))
            .setColumn(100, "", tr("Return fiscal"))
            .setColumn(100, "", tr("Amount"))
            .setColumn(100, "", tr("Order date"))
            .setColumn(120, "", tr("Gov number"))
            .setColumn(120, "", tr("Table"))
            .setColumn(200, "", tr("Error"));

    const QString jsonIn = QStringLiteral("CAST(FROM_BASE64(tl.f_in) AS CHAR CHARACTER SET utf8mb4)");
    const QString query = QString(
                              "select tl.f_time, "
                              "tl.f_order, "
                              "cast(json_value(%1, '$.returnTicketId') as unsigned) as f_return_ticket_id, "
                              "json_value(%1, '$.crn') as f_crn, "
                              "json_value(tl.f_out, '$.fiscal') as f_return_fiscal, "
                              "json_value(tl.f_out, '$.total') as f_total, "
                              "h.f_datecash, "
                              "c.f_govnumber, "
                              "rt.f_name as f_table, "
                              "tl.f_err "
                              "from o_tax_log tl "
                              "left join o_header h on h.f_id=tl.f_order "
                              "left join o_car c on c.f_order=tl.f_order "
                              "left join r_table rt on rt.f_id=h.f_table "
                              "where date(tl.f_time) between %2 and %3 "
                              "and cast(json_value(%1, '$.returnTicketId') as unsigned) > 0 "
                              "order by tl.f_time desc")
                              .arg(jsonIn, ui->wd->ds1(), ui->wd->ds2());

    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    rg->fTableView->resizeColumnsToContents();
}

void FTaxReturn::doubleClickOnRow(const QList<QVariant> &row)
{
    if(row.size() < 2) {
        return;
    }

    const int orderId = row.at(1).toInt();

    if(orderId <= 0) {
        return;
    }

    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(QString::number(orderId));
    d->exec();
    delete d;
}
