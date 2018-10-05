#include "fnoshowcancelationfee.h"
#include "ui_fnoshowcancelationfee.h"
#include "wreportgrid.h"
#include "dlgnoshow.h"

FNoShowCancelationFee::FNoShowCancelationFee(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FNoShowCancelationFee)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Cancelation/No show fee"), ":/images/garbage.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
}

FNoShowCancelationFee::~FNoShowCancelationFee()
{
    delete ui;
}

QString FNoShowCancelationFee::reportTitle()
{
    return QString("%1")
            .arg("Cancelation and no show fees");
}

QWidget *FNoShowCancelationFee::firstElement()
{
    return ui->deStart;
}

void FNoShowCancelationFee::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Voucher"))
            .setColumn(120, "", tr("Date"))
            .setColumn(100, "", tr("Reservation"))
            .setColumn(100, "", tr("Invoice"))
            .setColumn(50, "", tr("Room"))
            .setColumn(120, "", tr("Entry"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(300, "", tr("Guest"))
            .setColumn(200, "", tr("Name"))
            .setColumn(120, "", tr("Amount"));
    QString sql = "select m.f_id, m.f_wdate, m.f_res, m.f_inv, r.f_room, r.f_startDate, r.f_endDate, g.guest, m.f_finalName, m.f_amountAmd "
            "from m_register m "
            "left join f_reservation r on r.f_id=m.f_res "
            "left join guests g on g.f_id=r.f_guest "
            "where m.f_wdate between :date1 and :date2 "
            "and (m.f_itemCode in (" + fPreferences.getDb(def_noshowfee_code).toString() + ", " + fPreferences.getDb(def_cancelfee_code).toString() + ")) ";
    sql.replace(":date1", ui->deStart->dateMySql());
    sql.replace(":date2", ui->deEnd->dateMySql());
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 9;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FNoShowCancelationFee::doubleClickOnRow(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        return;
    }
    DlgNoShow *d = new DlgNoShow(this);
    d->load(row.at(0).toString());
    d->exec();
    delete d;
}

void FNoShowCancelationFee::on_btnBack_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}

void FNoShowCancelationFee::on_btnForward_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}
