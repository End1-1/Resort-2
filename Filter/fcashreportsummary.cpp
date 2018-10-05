#include "fcashreportsummary.h"
#include "ui_fcashreportsummary.h"
#include "wreportgrid.h"

FCashReportSummary::FCashReportSummary(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCashReportSummary)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Cash report common"), ":/images/credit-card.png");

    fQuery = "select  p.f_en, m.f_paymentComment, "
             "sum(m.f_amountAmd), sum(m.f_amountAmd / m.f_amountUsd) "
             "from m_register m "
             "left join users u on u.f_id=m.f_user "
             "left join f_payment_type p on p.f_id=m.f_paymentMode "
             "where m.f_wdate between :f_wdate1 and :f_wdate2 and m.f_canceled=0 "
             "and m.f_finance=1 and m.f_paymentMode in (1,2,3,4) and (((m.f_source in :source) :orbr) :orch)"
            "group by 1, 2 "
             "order by m.f_paymentMode ";
}

FCashReportSummary::~FCashReportSummary()
{
    delete ui;
}

void FCashReportSummary::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Type"))
            .setColumn(200, "", tr("Comment"))
            .setColumn(100, "", tr("Amount, AMD"))
            .setColumn(100, "", tr("Amount, USD"));
    QString query = fQuery;
    query = query.replace(":f_wdate1", ui->deStart->dateMySql()).replace(":f_wdate2", ui->deEnd->dateMySql());
    QString h = "'RV','AV'";
    QString r = "'PS', 'PE'";
    QString t;
    if (ui->chHotel->isChecked()) {
        t += h;
        query = query.replace(":orch",
        " or (m.f_source='CH' and m.f_inv='' and m.f_itemCode<>" + fPreferences.getDb(def_auto_breakfast_id).toString() + ")"
        "or (m.f_itemCode in ("
                              + fPreferences.getDb(def_noshowfee_code).toString()
                              + ", " + fPreferences.getDb(def_cancelfee_code).toString() + "))");
    } else {
        query = query.replace(":orch", "");
    }
    if (ui->chRestaurant->isChecked()) {
        if (!t.isEmpty()) {
            t += ",";
        }
        t += r;
        query = query.replace(":orbr", QString(" or m.f_itemCode=%1 ").arg(fPreferences.getDb(def_auto_breakfast_id).toInt()));
    } else {
        query = query.replace(":orbr", "");
    }
    if (t.isEmpty()) {
        message_error_tr("No report type is selected");
        return;
    }
    t = "(" + t + ") ";
    query.replace(":source", t);
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> colTotal;
    colTotal << 2 << 3;
    QList<double> valTotal;
    rg->fModel->sumOfColumns(colTotal, valTotal);
    rg->setTblTotalData(colTotal, valTotal);
    rg->fModel->insertSubTotals(0, colTotal);
}

QWidget *FCashReportSummary::firstElement()
{
    return ui->deStart;
}

QString FCashReportSummary::reportTitle()
{
    QString text;
    if (ui->chHotel->isChecked()) {
        text += tr("Hotel") + ",";
    }
    if (ui->chRestaurant->isChecked()) {
        text += tr("Restaurant") + ",";
    }
    text.remove(text.length() - 1, 1);
    return QString("%1 From %2 To %3 For %4")
            .arg(tr("Cash report, summary"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text())
            .arg(text);
}

void FCashReportSummary::on_btnBack_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FCashReportSummary::on_btnForward_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}
