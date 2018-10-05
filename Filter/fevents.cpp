#include "fevents.h"
#include "ui_fevents.h"
#include "wreportgrid.h"
#include "vauchers.h"
#include <QInputDialog>

FEvents::FEvents(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FEvents)
{
    ui->setupUi(this);
    fQuery = "select e.f_id, f_wdate, f_time, h.f_name, i.f_" + def_lang + ","
            "e.f_comment, "
            "e.f_pax, e.f_price, e.f_total, e.f_tax, ii.f_" + def_lang + ", "
            "e.f_paymentDetails "
            "from o_event e "
            "left join r_hall h on h.f_id=e.f_hall "
            "left join f_invoice_item i on i.f_id=e.f_itemCode "
            "left join f_payment_type ii on ii.f_id=e.f_paymentMode "
            "left join f_credit_card cr on cr.f_id=e.f_creditCard "
            "left join f_city_ledger cl on cl.f_id=e.f_cityLedger "
            "where e.f_wdate between :f_wdate1 and :f_wdate2 and f_canceled=0 ";
    fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removeEvent()), this)->setFocusPolicy(Qt::NoFocus);
    QToolButton *b = fReportGrid->addToolBarButton(":/images/biohazard.png", tr("Eliminate"), SLOT(eliminate()), this);
    b->setFocusPolicy(Qt::NoFocus);
    b->setVisible(r__(cr__super_correction));
    fReportGrid->setupTabTextAndIcon(tr("Events"), ":/images/banket.png");
}

FEvents::~FEvents()
{
    delete ui;
}

QWidget *FEvents::firstElement()
{
    return ui->deStart;
}

QString FEvents::reportTitle()
{
    return "Events";
}

void FEvents::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Code"))
            .setColumn(100, "", tr("Date"))
            .setColumn(80, "", tr("Time"))
            .setColumn(100, "", tr("Hall"))
            .setColumn(300, "", tr("Name"))
            .setColumn(200, "", tr("Comment"))
            .setColumn(50, "", tr("Pax"))
            .setColumn(80, "", tr("Price"))
            .setColumn(80, "", tr("Total"))
            .setColumn(30, "", tr("Tax"))
            .setColumn(100, "", tr("Mode of payment"))
            .setColumn(200, "", tr("Payment details"))
            ;
    QString query = fQuery;
    query.replace(":f_wdate1", ui->deStart->dateMySql()).replace(":f_wdate2", ui->deEnd->dateMySql());
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 6 << 8;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FEvents::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FEvents *e = new FEvents(rg);
    rg->addFilterWidget(e);
    e->apply(rg);
}

void FEvents::removeEvent()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    if (message_confirm(tr("Confirm removal")) != QDialog::Accepted) {
        return;
    }
    QString reason = QInputDialog::getText(this, tr("Reason"), tr("Input reason"));
    fDb.fDb.transaction();
    fDbBind[":f_canceled"] = 1;
    fDbBind[":f_cancelUser"] = WORKING_USERID;
    fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
    fDbBind[":f_cancelReason"] = reason;
    fDb.update("o_event", fDbBind, where_id(out.at(0).toInt()));

    fDbBind[":f_source"] = VAUCHER_EVENT_N;
    fDbBind[":f_doc"] = out.at(0).toInt();
    fDb.select("select f_id from m_register where f_source=:f_source and f_doc=:f_doc", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error("Application error. Contact to developer. Message: row count == 0, remove event");
        return;
    }

    int recid = fDbRows.at(0).at(0).toInt();

    fDbBind[":f_canceled"] = 1;
    fDbBind[":f_cancelUser"] = WORKING_USERID;
    fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
    fDbBind[":f_cancelReason"] = reason;
    fDb.update("m_register", fDbBind, where_id(recid));
    fDb.fDb.commit();

}

void FEvents::eliminate()
{
    QList<QVariant> out;
    int row = fReportGrid->fillRowValuesOut(out);
    if (row < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    if (message_confirm(tr("THIS WILL REMOVE PERMANENTLY EVENT DATE!")) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] =out.at(0);
    fDb.select("delete from o_event where f_id=:f_id", fDbBind, fDbRows);
    fDbBind[":f_id"] =out.at(0);
    fDb.select("delete from m_register where f_id=:f_id", fDbBind, fDbRows);
    fReportGrid->fModel->removeRow(row);
}
