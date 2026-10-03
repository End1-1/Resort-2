#include "fcoupondocuments.h"
#include "ui_fcoupondocuments.h"
#include "reportquery.h"
#include "wreportgrid.h"
#include "dlgcouponservicedocument.h"
#include "dlgcouponserviceback.h"
#include "dlgcouponservicepayment.h"
#include "database2.h"

namespace {

constexpr int docTypePartnerBalance = 5;

const char *partnerBalanceSql = R"(
select p.f_name as `Գնորդ`,
       g.f_name as `Տեսակ`,
       t.f_code as `Կոդ`,
       h.f_date as `Վաճառքի ամսաթիվ`,
       t.f_validto as `Վավեր է մինչև`,
       case when t.f_validto is not null and t.f_validto < curdate() then 'Ժամկետանց' else '' end as `Ժամկետանց`,
       case when coalesce(t.f_special, 0) = 1 then 'Էլեկրոնային' else '' end as `Էլեկրոնային`
from talon_service t
inner join r_partners p on p.f_id = t.f_partner
inner join talon_service_items_group g on g.f_id = t.f_group
inner join talon_documents_header h on h.f_id = t.f_trsale
where t.f_trsale > 0
  and coalesce(t.f_used, 0) = 0
  and coalesce(t.f_trback, 0) = 0
  %partner%
order by p.f_name, g.f_name, t.f_code
)";

} // namespace

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
    const int docType = ui->cbDocType->currentData().toInt();

    if(docType == docTypePartnerBalance) {
        QString query = QString(partnerBalanceSql);

        if(ui->cbPartner->currentData().toInt() == 0) {
            query.replace("%partner%", " and t.f_partner > 0 ");
        } else {
            query.replace("%partner%", QString(" and t.f_partner = %1 ").arg(ui->cbPartner->currentData().toInt()));
        }

        rg->fModel->clearColumns();
        rg->fModel->setSqlQuery(query);
        rg->fModel->apply(rg);
        rg->setTblTotalData(QList<int>(), QList<double>());
        rg->fTableView->resizeColumnsToContents();
        return;
    }

    QString query = fReportQuery->query;
    query.replace("%d1%", ui->leD1->dateMySql()).replace("%d2%", ui->leD2->dateMySql());
    if(docType == 0) {
        query.replace("%type%", "");
    } else {
        query.replace("%type%", QString(" and d.f_type=%1").arg(docType));
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
    if(ui->cbDocType->currentData().toInt() == docTypePartnerBalance) {
        return tr("Coupon balance with partners");
    }

    return fReportQuery->reportTitle;
}

void FCouponDocuments::doubleClickOnRow(const QList<QVariant> &values)
{
    if(ui->cbDocType->currentData().toInt() == docTypePartnerBalance) {
        return;
    }

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
