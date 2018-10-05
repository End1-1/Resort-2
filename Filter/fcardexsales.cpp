#include "fcardexsales.h"
#include "ui_fcardexsales.h"
#include "wreportgrid.h"
#include "wreportgrid.h"
#include "dlghelp.h"
#include "vauchers.h"
#include "ptextrect.h"

FCardexSales::FCardexSales(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCardexSales)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Cardex analisis"), ":/images/balance.png");
    /*fReportGrid->addToolBarButton(":/images/invoice.png", tr("Open invoice"), SLOT(openInvoice()), this)
            ->setFocusPolicy(Qt::ClickFocus);
            */

    fDockCardex = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardex);
    connect(fDockCardex, SIGNAL(cardex(CI_Cardex*)), this, SLOT(cardex(CI_Cardex*)));
    //fReportGrid->setHelp("cardexanalysis");
    DatabaseResult dr;
    dr.select(fDb, "select f_id, f_title, f_width, f_items from serv_cardex_analysis", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        Field f;
        f.fTitle = dr.value(i, "f_title").toString();
        f.fWidth = dr.value(i, "f_width").toInt();
        f.fItems = dr.value(i, "f_items").toString();
        fItems[dr.value(i, "f_id").toInt()] = f;
    }
}

FCardexSales::~FCardexSales()
{
    delete ui;
}

QWidget *FCardexSales::firstElement()
{
    return ui->deFrom;
}

QString FCardexSales::reportTitle()
{
    QString title = QString("%1 %2 %3 %4 - %5")
            .arg(tr("Cardex analysis by "))
            .arg((ui->cbReportType->currentText()))
            .arg(ui->leCardex->text())
            .arg(ui->deFrom->text())
            .arg(ui->deTo->text());
    return title;
}

void FCardexSales::apply(WReportGrid *rg)
{
    switch (ui->cbReportType->currentIndex()) {
    case 0:
        guestReport(rg);
        break;
    case 1:
        cardexReport(rg);
        break;
    }
}

void FCardexSales::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCardexSales *cs = new FCardexSales(rg);
    rg->addFilterWidget(cs);
    cs->apply(rg);
}

void FCardexSales::finalPrint(PPrintScene *ps, int top)
{
    DatabaseResult dr;
    PTextRect prTempl;
    prTempl.setWrapMode(QTextOption::NoWrap);
    prTempl.setFont(QFont(qApp->font().family(), 17));
    prTempl.setBorders(false, false, false, false);
    prTempl.setTextPen(QPen(Qt::black, Qt::SolidLine));
    for (QMap<int, Field> ::const_iterator it = fItems.begin(); it != fItems.end(); it++) {
        dr.select(fDb, "select f_en from f_invoice_item where f_id in (" + it.value().fItems + ")", fDbBind);
        QString name;
        for (int i = 0; i < dr.rowCount(); i++) {
            name += " " + dr.value(i, "f_en").toString();
        }
        if (dr.rowCount() > 0) {
            top += ps->addTextRect(10, top, 2000, 60, it.value().fTitle + ": " + name, &prTempl)->textHeight();
        }
    }
}

void FCardexSales::guestReport(WReportGrid *rg)
{
    fReportGrid->fModel->clearColumns();
    fReportGrid->fModel->setColumn(250, "", tr("Guest"))
            .setColumn(60, "", tr("Room"))
            .setColumn(80, "", tr("In"))
            .setColumn(80, "", tr("Out"))
            .setColumn(50, "", tr("Acc"))
            .setColumn(50, "", tr("Pax"))
            .setColumn(80, "", tr("Inv"))
            .setColumn(80, "", tr("Nation"))
            .setColumn(80, "", tr("Nights"))
            .setColumn(fItems[1].fWidth, "", fItems[1].fTitle)
            .setColumn(fItems[2].fWidth, "", fItems[2].fTitle)
            .setColumn(fItems[3].fWidth, "", fItems[3].fTitle)
            .setColumn(fItems[4].fWidth, "", fItems[4].fTitle)
            .setColumn(fItems[5].fWidth, "", fItems[5].fTitle)
            .setColumn(fItems[6].fWidth, "", fItems[6].fTitle)
            .setColumn(80, "", tr("Total"));
    fQuery = "select distinct(concat(g.f_firstName, ' ', g.f_lastName)), r.f_room, r.f_startDate, r.f_endDate,\
            ra.f_en, pax.total,r.f_invoice, g.f_nation, n.nights, \
            coalesce(s1.amount, 0), coalesce(s2.amount, 0), coalesce(s3.amount, 0), coalesce(s4.amount,0), coalesce(s5.amount, 0), coalesce(s6.amount, 0),\
            coalesce(s1.amount, 0) + coalesce(s2.amount, 0) + coalesce(s3.amount, 0) + coalesce(s4.amount,0) + coalesce(s5.amount, 0) + coalesce(s6.amount, 0) \
            from f_reservation r \
            left join f_guests g on g.f_id=r.f_guest \
            left join f_room_arrangement ra on ra.f_id=r.f_arrangement \
            left join m_register mm on mm.f_inv=r.f_invoice \
            left join (select m.f_inv, sum(r.f_man+r.f_woman+r.f_child) as total  from m_register m left join f_reservation r on r.f_invoice=m.f_inv where :side m.f_wdate between :f_date1 and :f_date2 and m.f_canceled=0 and f_finance=1 and m.f_itemCode in (1,66,69,14,15) group by 1) pax on pax.f_inv=r.f_invoice \
            left join (select f_inv, count(f_id) as nights from m_register where f_wdate between :f_date1 and :f_date2 and f_itemCode in (1,66,69,14,15) and f_canceled=0 group by 1) n on n.f_inv=r.f_invoice \
            left join (select m.f_inv, sum((m.f_amountAmd :novat) :curr) as amount from m_register m where :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item1) and m.f_wdate between :f_date1 and :f_date2 group by 1) s1 on s1.f_inv=r.f_invoice \
            left join (select m.f_inv, sum((m.f_amountAmd :novat) :curr) as amount from m_register m where :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item2) and m.f_wdate between :f_date1 and :f_date2 group by 1) s2 on s2.f_inv=r.f_invoice \
            left join (select m.f_inv, sum((m.f_amountAmd :novat) :curr) as amount from m_register m where :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item3) and m.f_wdate between :f_date1 and :f_date2 group by 1) s3 on s3.f_inv=r.f_invoice \
            left join (select m.f_inv, sum((m.f_amountAmd :novat) :curr) as amount from m_register m where :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item4) and m.f_wdate between :f_date1 and :f_date2 group by 1) s4 on s4.f_inv=r.f_invoice \
            left join (select m.f_inv, sum((m.f_amountAmd :novat) :curr) as amount from m_register m where :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item5) and m.f_wdate between :f_date1 and :f_date2 group by 1) s5 on s5.f_inv=r.f_invoice \
            left join (select m.f_inv, sum((m.f_amountAmd :novat) :curr) as amount from m_register m where :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item6) and m.f_wdate between :f_date1 and :f_date2 group by 1) s6 on s6.f_inv=r.f_invoice \
            where :f_cardex (r.f_state=3 :checkin) and mm.f_wdate between :f_date1 and :f_date2"
            ;
    if (!ui->chCompanyPart->isChecked() && !ui->chGuestPart->isChecked()) {
        message_error_tr("Select guest or company side first");
        return;
    }
    QString query = fQuery;
    query.replace(":f_date1", ui->deFrom->dateMySql()).replace(":f_date2", ui->deTo->dateMySql());
    if (!ui->leCardex->fHiddenText.isEmpty()) {
        query.replace(":f_cardex", QString("r.f_cardex='%1' and").arg(ui->leCardex->fHiddenText));
    } else {
        query.replace(":f_cardex", "");
    }
    if (ui->rbAmd->isChecked()) {
        query.replace(":curr", "");
    } else {
        query.replace(":curr", " / f_amountUsd");
    }
    QString side;
    if (ui->chGuestPart->isChecked()) {
        side += "0,";
    }
    if (ui->chCompanyPart->isChecked()) {
        side += "1,";
    }
    if (ui->chNoVat->isChecked()) {
        query.replace(":novat", "-m.f_amountVat");
    } else {
        query.replace(":novat", "");
    }
    if (ui->chCheckin->isChecked()) {
        query.replace(":checkin", " or r.f_state=1");
    } else {
        query.replace(":checkin", "");
    }
    query.replace(":item1", fItems[1].fItems);
    query.replace(":item2", fItems[2].fItems);
    query.replace(":item3", fItems[3].fItems);
    query.replace(":item4", fItems[4].fItems);
    query.replace(":item5", fItems[5].fItems);
    query.replace(":item6", fItems[6].fItems);
    side.remove(side.length() - 1, 1);
    side = "m.f_side in (" + side + ") and";
    query.replace(":side", side);
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 5 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FCardexSales::cardexReport(WReportGrid *rg)
{
    fReportGrid->fModel->clearColumns();
    fReportGrid->fModel->setColumn(250, "", tr("Cardex"))
            .setColumn(50, "", tr("B/O"))
            .setColumn(50, "", tr("B/B"))
            .setColumn(50, "", tr("H/B"))
            .setColumn(50, "", tr("F/B"))
            .setColumn(80, "", tr("Pax"))
            .setColumn(80, "", tr("Nights"))
            .setColumn(fItems[1].fWidth, "", fItems[1].fTitle)
            .setColumn(fItems[2].fWidth, "", fItems[2].fTitle)
            .setColumn(fItems[3].fWidth, "", fItems[3].fTitle)
            .setColumn(fItems[4].fWidth, "", fItems[4].fTitle)
            .setColumn(fItems[5].fWidth, "", fItems[5].fTitle)
            .setColumn(fItems[6].fWidth, "", fItems[6].fTitle)
            .setColumn(80, "", tr("Total"));
    fQuery = "select distinct(car.f_name), \
            bo.total, bb.total, hb.total, fb.total, pax.total, n.nights, \
            coalesce(s1.amount, 0), coalesce(s2.amount, 0), coalesce(s3.amount, 0), coalesce(s4.amount,0), coalesce(s5.amount, 0), coalesce(s6.amount, 0),\
            coalesce(s1.amount, 0) + coalesce(s2.amount, 0) + coalesce(s3.amount, 0) + coalesce(s4.amount,0) + coalesce(s5.amount, 0) + coalesce(s6.amount, 0) \
            from f_cardex car \
            left join f_reservation r on car.f_cardex=r.f_cardex \
            left join m_register mm on mm.f_inv=r.f_invoice \
            left join (select rr.f_cardex, sum(rr.f_man+rr.f_woman+rr.f_child) as total  from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where :side (rr.f_state=3 :rrcheckin) and m.f_wdate between :f_date1 and :f_date2 and m.f_canceled=0 and f_finance=1 and m.f_itemCode in (1,66,69,14,15) group by 1) pax on pax.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, count(m.f_id) as nights from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where :side (rr.f_state=3 :rrcheckin) and f_wdate between :f_date1 and :f_date2 and f_itemCode in (1,66,69,14,15) and f_canceled=0 group by 1) n on n.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, count(rr.f_id) as total from f_reservation rr left join m_register m on m.f_inv=rr.f_invoice where (rr.f_state=3 :rrcheckin) and rr.f_arrangement=1 and m.f_wdate between :f_date1 and :f_date2 group by 1) bo on bo.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, count(rr.f_id) as total from f_reservation rr left join m_register m on m.f_inv=rr.f_invoice where (rr.f_state=3 :rrcheckin) and rr.f_arrangement=2 and m.f_wdate between :f_date1 and :f_date2 group by 1) bb on bb.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, count(rr.f_id) as total from f_reservation rr left join m_register m on m.f_inv=rr.f_invoice where (rr.f_state=3 :rrcheckin) and rr.f_arrangement=3 and m.f_wdate between :f_date1 and :f_date2 group by 1) hb on hb.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, count(rr.f_id) as total from f_reservation rr left join m_register m on m.f_inv=rr.f_invoice where (rr.f_state=3 :rrcheckin) and rr.f_arrangement=4 and m.f_wdate between :f_date1 and :f_date2 group by 1) fb on fb.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, sum((m.f_amountAmd :novat) :curr) as amount from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where (rr.f_state=3 :rrcheckin) and :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item1) and m.f_wdate between :f_date1 and :f_date2 group by 1) s1 on s1.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, sum((m.f_amountAmd :novat) :curr) as amount from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where (rr.f_state=3 :rrcheckin) and :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item2) and m.f_wdate between :f_date1 and :f_date2 group by 1) s2 on s2.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, sum((m.f_amountAmd :novat) :curr) as amount from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where (rr.f_state=3 :rrcheckin) and :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item3) and m.f_wdate between :f_date1 and :f_date2 group by 1) s3 on s3.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, sum((m.f_amountAmd :novat) :curr) as amount from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where (rr.f_state=3 :rrcheckin) and :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item4) and m.f_wdate between :f_date1 and :f_date2 group by 1) s4 on s4.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, sum((m.f_amountAmd :novat) :curr) as amount from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where (rr.f_state=3 :rrcheckin) and :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item5) and m.f_wdate between :f_date1 and :f_date2 group by 1) s5 on s5.f_cardex=r.f_cardex \
            left join (select rr.f_cardex, sum((m.f_amountAmd :novat) :curr) as amount from m_register m left join f_reservation rr on rr.f_invoice=m.f_inv where (rr.f_state=3 :rrcheckin) and :side m.f_sign=1 and f_canceled=0 and m.f_itemCode in (:item6) and m.f_wdate between :f_date1 and :f_date2 group by 1) s6 on s6.f_cardex=r.f_cardex \
            where :f_cardex (r.f_state=3 :checkin) and mm.f_wdate between :f_date1 and :f_date2"
            ;
    if (!ui->chCompanyPart->isChecked() && !ui->chGuestPart->isChecked()) {
        message_error_tr("Select guest or company side first");
        return;
    }
    QString query = fQuery;
    query.replace(":f_date1", ui->deFrom->dateMySql()).replace(":f_date2", ui->deTo->dateMySql());
    if (!ui->leCardex->fHiddenText.isEmpty()) {
        query.replace(":f_cardex", QString("r.f_cardex='%1' and").arg(ui->leCardex->fHiddenText));
    } else {
        query.replace(":f_cardex", "");
    }
    if (ui->rbAmd->isChecked()) {
        query.replace(":curr", "");
    } else {
        query.replace(":curr", " / f_amountUsd");
    }
    QString side;
    if (ui->chGuestPart->isChecked()) {
        side += "0,";
    }
    if (ui->chCompanyPart->isChecked()) {
        side += "1,";
    }
    if (ui->chNoVat->isChecked()) {
        query.replace(":novat", "-m.f_amountVat");
    } else {
        query.replace(":novat", "");
    }
    if (ui->chCheckin->isChecked()) {
        query.replace(":checkin", " or r.f_state=1");
        query.replace(":rrcheckin", " or rr.f_state=1");
    } else {
        query.replace(":checkin", "");
        query.replace(":rrcheckin", "");
    }
    query.replace(":item1", fItems[1].fItems);
    query.replace(":item2", fItems[2].fItems);
    query.replace(":item3", fItems[3].fItems);
    query.replace(":item4", fItems[4].fItems);
    query.replace(":item5", fItems[5].fItems);
    query.replace(":item6", fItems[6].fItems);
    side.remove(side.length() - 1, 1);
    side = "m.f_side in (" + side + ") and";
    query.replace(":side", side);
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FCardexSales::openInvoice()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    openInvoiceWithId(out.at(6).toString());
}

void FCardexSales::cardex(CI_Cardex *c)
{
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardex, c);
}

void FCardexSales::on_toolButton_clicked()
{
    ui->deFrom->setDate(ui->deFrom->date().addDays(-1));
    ui->deTo->setDate(ui->deTo->date().addDays(-1));
    apply(fReportGrid);
}

void FCardexSales::on_toolButton_2_clicked()
{
    ui->deFrom->setDate(ui->deFrom->date().addDays(1));
    ui->deTo->setDate(ui->deTo->date().addDays(1));
    apply(fReportGrid);
}

void FCardexSales::on_btnHelp_clicked()
{
    DlgHelp::openHelp("cardexanalysis");
}

void FCardexSales::on_cbReportType_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    apply(fReportGrid);
}

void FCardexSales::on_chNoVat_clicked()
{
    apply(fReportGrid);
}

void FCardexSales::on_chCheckin_clicked()
{
    apply(fReportGrid);
}
