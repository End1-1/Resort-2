#include "finvoices.h"
#include "ui_finvoices.h"
#include "wreportgrid.h"
#include "waccinvoice.h"
#include <QToolButton>

FInvoices::FInvoices(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FInvoices)
{
    ui->setupUi(this);
    ui->deStart->setDate(WORKING_DATE.addDays(-1 * 180));
    if (r__(cr__super_correction)) {
        if (fPreferences.getDb("HC").toInt() == 0) {
            QToolButton *btn = fReportGrid->addToolBarButton(":/images/biohazard.png", tr("Eliminate!"), SLOT(removeInvoiceWithAllReference()), this);
            btn->setToolTip(tr("Remove invoice with all references"));
            btn->setFocusPolicy(Qt::ClickFocus);
        }
    }
    ui->chContainsEmptyRooming->setVisible(false);
}

FInvoices::~FInvoices()
{
    delete ui;
}

void FInvoices::apply(WReportGrid *rg)
{
    QString where = "where r.f_state=3 and  (r.f_endDate between '"
            + ui->deStart->date().toString(def_mysql_date_format) + "' and '"
            + ui->deEnd->date().toString(def_mysql_date_format) + "') ";
    if (ui->chContainsEmptyRooming->isChecked()) {
        where += " and f.f>0 ";
    }
    where +=  "order by r.f_endDate, r.f_room ";
    buildQuery(rg, where);
    QList<int> cols;
    cols << 4 << 5 << 6;
    QList<double> vals;
    fReportGrid->fModel->sumOfColumns(cols, vals);
    fReportGrid->setTblTotalData(cols, vals);
}

QWidget *FInvoices::firstElement()
{
    return ui->deStart;
}

QString FInvoices::reportTitle()
{
    return QString("%1 From %2 To %3")
            .arg(tr("Invoices"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QCheckBox *FInvoices::chFreeRooming()
{
    return ui->chContainsEmptyRooming;
}

void FInvoices::openReport(bool free)
{

}

void FInvoices::removeInvoiceWithAllReference()
{
    QList<QVariant> out;
    int row;
    if ((row = fReportGrid->fillRowValuesOut(out)) < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    QString inv = out.at(0).toString();
    if (inv.isEmpty()) {
        return;
    }

    if (message_confirm_tr("Confirm remove selected invoice with all reference. Log will disabled for this action.") != QDialog::Accepted) {
        return;
    }
    QList<int> ps;
    fDbBind[":f_inv"] = inv;
    fDb.select("select f_doc from m_register where f_inv=:f_inv and f_source='PS'", fDbBind, fDbRows);
    foreach_rows {
        ps << it->at(0).toInt();
    }
    foreach (int id, ps) {
        fDbBind[":f_header"] = id;
        fDb.select("delete from o_dish where f_header=:f_header", fDbBind, fDbRows);
        fDbBind[":f_id"] = id;
        fDb.select("delete from o_header where f_id=:f_id", fDbBind, fDbRows);
    }
    fDbBind[":f_inv"] = inv;
    fDb.select("delete from m_register where f_inv=:f_inv", fDbBind, fDbRows);
    fDbBind[":f_invoice"] = inv;
    fDb.select("delete from f_reservation where f_invoice=:f_invoice", fDbBind, fDbRows);
    fReportGrid->fModel->removeRow(row);
    message_info_tr("Invoice removed with all references. Refresh reports.");
}

void FInvoices::on_btnNext_clicked()
{
    Utils::dateEditNext(ui->deStart, ui->deEnd);
}

void FInvoices::on_btnPrev_clicked()
{
    Utils::dateEditPrev(ui->deStart, ui->deEnd);
}
