#include "fcoupondocuments.h"
#include "ui_fcoupondocuments.h"
#include "reportquery.h"
#include "wreportgrid.h"
#include "dlgcouponservicedocument.h"
#include "dlgcouponserviceback.h"
#include "dlgcouponservicepayment.h"
#include "database2.h"

FCouponDocuments::FCouponDocuments(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCouponDocuments)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Coupons of services"), ":/images/talon.png");
    fReportQuery = new ReportQuery("coupon_service_documents");
    fReportQuery->costumizeCombo(ui->cbDocType, "talon_document_type");
    fReportQuery->costumizeCombo(ui->cbPartner, "partners");
    fReportQuery->costumizeCombo(ui->cbTemplate, "coupon_doc_templates", false, 0);
    if (check_permission(pr_coupon_sale)) {
        fReportGrid->addToolBarButton(":/images/new.png", tr("Sale"), SLOT(newDoc()), this)->setFocusPolicy(Qt::NoFocus);
        fReportGrid->addToolBarButton(":/images/returnbox.png", tr("Return"), SLOT(returnDoc()),
                                      this)->setFocusPolicy(Qt::NoFocus);
        fReportGrid->addToolBarButton(":/images/payment.png", tr("Payment"), SLOT(paymentDoc()),
                                      this)->setFocusPolicy(Qt::NoFocus);
    }
    connect(fReportGrid, &WReportGrid::doubleClickOnRow, this, &FCouponDocuments::doubleClickOnRow);
}

FCouponDocuments::~FCouponDocuments()
{
    delete ui;
}

void FCouponDocuments::apply(WReportGrid *rg)
{
    QString query = fReportQuery->query;
    query.replace("%d1%", ui->leD1->dateMySql()).replace("%d2%", ui->leD2->dateMySql());
    if (ui->cbDocType->currentData().toInt() == 0) {
        query.replace("%type%", "");
    } else {
        query.replace("%type%", QString(" and d.f_type=%1").arg(ui->cbDocType->currentData().toInt()));
    }
    if (ui->cbPartner->currentData().toInt() == 0) {
        query.replace("%partner%", "");
        query.replace("%partner1%", " t.f_partner>0");
    } else {
        query.replace("%partner%", QString(" and d.f_partner=%1").arg(ui->cbPartner->currentData().toInt()));
        query.replace("%partner1%", QString(" t.f_partner=%1").arg(ui->cbPartner->currentData().toInt()));
    }
    rg->fModel->clearColumns();
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<double> vals;
    rg->fModel->sumOfColumns(fReportQuery->sumColumns, vals);
    rg->setTblTotalData(fReportQuery->sumColumns, vals);
    for (QMap<int, int>::const_iterator it = fReportQuery->columnsWidths.constBegin();
            it != fReportQuery->columnsWidths.constEnd(); it++) {
        rg->fTableView->setColumnWidth(it.key(), it.value());
    }
    if (fReportQuery->columnsWidths.isEmpty()) {
        rg->fTableView->resizeColumnsToContents();
    }
}

QWidget *FCouponDocuments::firstElement()
{
    return ui->leD1;
}

QString FCouponDocuments::reportTitle()
{
    return fReportQuery->reportTitle;
}

void FCouponDocuments::doubleClickOnRow(const QList<QVariant> &values)
{
    if (values.count() > 0) {
        Database2 db;
        if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
            message_error(db.lastDbError());
            return;
        }
        db[":f_id"] = values.at(0).toInt();
        db.exec("select * from talon_documents_header where f_id=:f_id");
        int t = 0;
        if (db.next()) {
            t = db.integer("f_type");
        }
        if (t == 0) {
            message_error("No document with this id");
            return;
        }
        switch (t) {
            case 3: {
                DlgCouponServiceBack d(fReportQuery, this);
                d.openDoc(values.at(0).toInt());
                d.exec();
            }
            return;
        }
        DlgCouponServiceDocument d(fReportQuery, this);
        d.openDocument(values.at(0).toInt());
        d.exec();
    }
}

void FCouponDocuments::newDoc()
{
    DlgCouponServiceDocument(fReportQuery, this).exec();
}

void FCouponDocuments::returnDoc()
{
    DlgCouponServiceBack(fReportQuery, this).exec();
}

void FCouponDocuments::paymentDoc()
{
    DlgCouponServicePayment(fReportQuery, this).exec();
}

void FCouponDocuments::on_cbTemplate_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    if (ui->cbTemplate->itemData(index).toString().isEmpty()) {
        return;
    }
    fReportQuery->loadData(ui->cbTemplate->itemData(index).toString());
    apply(fReportGrid);
}
