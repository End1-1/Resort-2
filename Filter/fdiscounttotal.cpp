#include "fdiscounttotal.h"
#include "ui_fdiscounttotal.h"
#include "wreportgrid.h"

FDiscountTotal::FDiscountTotal(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FDiscountTotal)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Discount, total"), ":/images/discount.png");
}

FDiscountTotal::~FDiscountTotal()
{
    delete ui;
}

QString FDiscountTotal::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Discount, total"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FDiscountTotal::firstElement()
{
    return ui->deStart;
}

void FDiscountTotal::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(0, "", tr("Costumer"))
            .setColumn(300, "", tr("Costumer"))
            .setColumn(200, "", tr("Gov. num"))
            .setColumn(120, "", tr("Phone"))            
            .setColumn(80, "", tr("Qty"))
            .setColumn(80, "", tr("Amount"))
            .setColumn(200, "", tr("Numbers"));
    QString sql = "select h.f_datecash, oc.f_costumer, dh.f_name, oc.f_govNumber, dh.f_info, count(h.f_id), sum(h.f_total), g.f_gov "
                  "from o_car oc "
                  "left join o_debt_holder dh on dh.f_id=oc.f_costumer "
                  "left join (select f_holder, group_concat(f_govNumber) as f_gov from o_debt_holder_car group by 1) g on g.f_holder=dh.f_id "
                  "left join o_header h on h.f_id=oc.f_order "
                  "where h.f_dateCash between :date1 and :date2 and h.f_state=2  "
                  "group by 1, 2, 3, 4 "
                  ":min ";
    sql.replace(":date1", ui->deStart->dateMySql());
    sql.replace(":date2", ui->deEnd->dateMySql());
    QString having;
    if (ui->leMin->asInt() == 0) {

    }  else {
        if (having.isEmpty()) {
            having = "having count(h.f_id)>=" + ui->leMin->text();
        } else {
            having += " and count(h.f_id)>=" + ui->leMin->text();
        }
    }
    if (ui->leMinAmount->asInt() > 0) {
        if (having.isEmpty()) {
            having = "having sum(h.f_total)>=" + ui->leMinAmount->text();
        } else {
            having += " and sum(h.f_total)>=" + ui->leMinAmount->text();
        }
    }
    sql.replace(":min", having);
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 5 << 6;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}
