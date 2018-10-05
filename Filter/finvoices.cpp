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
    QList<int> widths;
    widths << 80
           << 120
           << 80
           << 300
           << 100
           << 100
           << 100
           << 100
              ;
    QStringList fields;
    fields << "r.f_invoice"
           << "r.f_endDate"
           << "r.f_room"
           << "concat(g.f_title,' ',g.f_firstName,' ',g.f_lastName) as f_guest"
           << "coalesce(c.amount,0) as f_credit"
           << "coalesce(d.amount,0) as f_debet"
           << "coalesce(b.amount,0) as f_balance"
           << "coalesce(f.f, 0) as f";
    QStringList titles;
    titles << tr("Number")
           << tr("Date")
           << tr("Room")
           << tr("Guest")
           << tr("Debit")
           << tr("Credit")
           << tr("Balance")
           << tr("Free");
    QMap<QString, bool> includes;
    includes["r.f_invoice"] = true;
    includes["r.f_endDate"] = true;
    includes["r.f_room"] = true;
    includes["concat(g.f_title,' ',g.f_firstName,' ',g.f_lastName) as f_guest"] = true;
    includes["coalesce(c.amount,0) as f_credit"] = true;
    includes["coalesce(d.amount,0) as f_debet"] = true;
    includes["coalesce(b.amount,0) as f_balance"] = true;
    includes["coalesce(f.f, 0) as f"] = true;
    QStringList tables;
    tables << "f_reservation r"
           << "f_guests g"
           << "(select f_inv, sum(f_amountAmd) as amount from m_register where f_sign=1 and f_finance=1  and f_canceled=0 group by 1) c"
           << "(select f_inv, sum(f_amountAmd) as amount from m_register where f_sign=-1 and f_finance=1 and f_canceled=0 group by 1) d"
           << "(select f_inv, sum(f_amountAmd*f_sign) as amount from m_register where f_canceled=0 and f_finance=1 group by 1) b"
           << QString("(select f_inv, count(f_id) as f from m_register where f_canceled=0 and f_finance=1 and f_itemCode in (%1) and f_amountAmd<1 group by 1) f")
              .arg(fPreferences.getDb("rooming_list").toString());
    QStringList joins;
    joins << "from"
          << "left"
          << "left"
          << "left"
          << "left"
          << "left"
          << "left";
    QStringList joinConds;
    joinConds << ""
              << "g.f_id=r.f_guest"
              << "c.f_inv=r.f_invoice"
              << "d.f_inv=r.f_invoice"
              << "b.f_inv=r.f_invoice"
              << "f.f_inv=r.f_invoice"
                 ;
    QString title = tr("Invoices");
    QString icon = ":/images/invoice.png";
    WReportGrid *rg = addTab<WReportGrid>();
    rg->setQueryModel<FInvoices>(widths, fields, titles, includes, tables, joins, joinConds, title, icon);
    WAccInvoice *ai = new WAccInvoice(rg);
    FInvoices *v = static_cast<FInvoices*>(rg->fFilter);
    v->chFreeRooming()->setChecked(free);
    v->apply(rg);
    Q_UNUSED(ai)
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
