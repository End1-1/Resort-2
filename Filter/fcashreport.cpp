#include "fcashreport.h"
#include "ui_fcashreport.h"
#include "wreportgrid.h"

FCashReport::FCashReport(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCashReport)
{
    ui->setupUi(this);
    ui->chHotel->setChecked(true);
    ui->deFrom->setDate(WORKING_DATE.addDays(-1));
    ui->deTo->setDate(WORKING_DATE.addDays(-1));
    fReportGrid->setupTabTextAndIcon(tr("Cache report / detailed"), ":/images/credit-card.png");
    fHotelQuery = " select u.f_username, m.f_rdate, m.f_room, "
                  "m.f_id, m.f_guest, m.f_finalName, p.f_en, m.f_paymentComment, "
                  "m.f_amountAmd, m.f_amountAmd / m.f_amountUsd,  m.f_remarks, m.f_source "
                  "from m_register m "
                  "left join users u on u.f_id=m.f_user "
                  "left join f_payment_type p on p.f_id=m.f_paymentMode "
                  "where m.f_wdate between :f_wdate1 and :f_wdate2 and m.f_canceled=0 "
                  "and m.f_finance=1 and "
            "(m.f_source in ('RV', 'AV') "
                  "or (m.f_source='CH' and m.f_inv='' and m.f_itemCode<>" + fPreferences.getDb(def_auto_breakfast_id).toString() + ") "
                    "and m.f_paymentMode in (1, 2, 3, 4) "
            "or (m.f_itemCode in (" + fPreferences.getDb(def_noshowfee_code).toString() + ", " + fPreferences.getDb(def_cancelfee_code).toString() + ") )) ";
    fRestaurantQuery = " select u.f_username, mr.f_rdate, mr.f_room, "
                       "mr.f_id, mr.f_guest, mr.f_finalName, p.f_en, mr.f_paymentComment, "
                       "mr.f_amountAmd, mr.f_amountAmd / mr.f_amountUsd,  mr.f_remarks, mr.f_source "
                       "from m_register mr "
                       "left join users u on u.f_id=mr.f_user "
                       "left join f_payment_type p on p.f_id=mr.f_paymentMode "
                       "where mr.f_wdate between :f_wdate1 and :f_wdate2 and mr.f_canceled=0 "
                       "and mr.f_finance=1 "
                       " and ((mr.f_source in ('PS', 'PE') and mr.f_paymentMode in (1, 2, 3, 4)) :orbreak) ";
}

FCashReport::~FCashReport()
{
    delete ui;
}

void FCashReport::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(30, "", tr("Op"))
            .setColumn(80, "", tr("Date"))
            .setColumn(60, "", tr("Room"))
            .setColumn(80, "", tr("JV"))
            .setColumn(300, "", tr("Guest"))
            .setColumn(200, "", tr("Payment"))
            .setColumn(100, "", tr("Type"))
            .setColumn(200, "", tr("Comment"))
            .setColumn(100, "", tr("Amount AMD"))
            .setColumn(100, "", tr("Amount USD"))
            .setColumn(300, "", tr("Remarks"))
            .setColumn(0, "", "")
            ;
    QString query;
    if (ui->chHotel->isChecked()) {
        query = fHotelQuery;
    }

    if (ui->chRestaurant->isChecked()) {
        if (ui->chHotel->isChecked()) {
            query += " union all ";
        }
        query += fRestaurantQuery;
        query = query.replace(":orbreak", QString(" or mr.f_itemCode=%1 ").arg(fPreferences.getDb(def_auto_breakfast_id).toInt()));
    }

    if (query.isEmpty()) {
        message_info_tr("No report type was selected");
        return;
    }
    query = query.replace(":f_wdate1", ui->deFrom->dateMySql()).replace(":f_wdate2", ui->deTo->dateMySql());
    query += "order by 7, 6";
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 8 << 9;
    QList<double> vals;
    fReportGrid->fModel->sumOfColumns(cols, vals);
    fReportGrid->setTblTotalData(cols, vals);
    QList<int> subtotalCols;
    subtotalCols <<  8 << 9;
    fReportGrid->fModel->insertSubTotals(6, subtotalCols);
}

QWidget *FCashReport::firstElement()
{
    return ui->deFrom;
}

void FCashReport::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCashReport *clb = new FCashReport(rg);
    rg->addFilterWidget(clb);
    clb->apply(rg);
}

QString FCashReport::reportTitle()
{
    QString result =  tr("Cash report for ");;
    if (ui->chHotel->isChecked()) {
        result += tr("Hotel");
    }
    if (ui->chRestaurant->isChecked()) {
        if (ui->chHotel->isChecked()) {
            result += " and ";
        }
        result += tr("Restaurant");
    }
    result += QString("     %1 - %2").arg(ui->deFrom->text()).arg(ui->deTo->text());
    return result;
}

void FCashReport::on_toolButton_2_clicked()
{
    ui->deFrom->setDate(ui->deFrom->date().addDays(1));
    ui->deTo->setDate(ui->deTo->date().addDays(1));
    apply(fReportGrid);
}

void FCashReport::on_toolButton_clicked()
{
    ui->deFrom->setDate(ui->deFrom->date().addDays(-1));
    ui->deTo->setDate(ui->deTo->date().addDays(-1));
    apply(fReportGrid);
}
