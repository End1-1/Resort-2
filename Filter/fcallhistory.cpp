#include "fcallhistory.h"
#include "ui_fcallhistory.h"
#include "wreportgrid.h"

FCallHistory::FCallHistory(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCallHistory)
{
    ui->setupUi(this);
    ui->chIn->setVisible(r__(cr__call_in));
    ui->chOut->setVisible(r__(cr__call_out));
    ui->chInt->setVisible(r__(cr__call_int));
    ui->chTIn->setVisible(r__(cr__call_tin));
    ui->chTOut->setVisible(r__(cr__call_tout));
    ui->chIn->setChecked(r__(cr__call_in));
    ui->chOut->setChecked(r__(cr__call_out));
    ui->chInt->setChecked(r__(cr__call_int));
    ui->chTIn->setChecked(r__(cr__call_tin));
    ui->chTOut->setChecked(r__(cr__call_tout));
    fQuery = "select l.f_local, f_u1, f_date, l.f_time, t.f_name, l.f_remote, l.f_duration, l.f_cost, l.f_acc, l.f_doc \
            from f_call_log l \
            left join f_call_type t on t.f_id=l.f_ident \
            where f_date between :f_date1 and :f_date2 and l.f_ident in (:where) \
            order by f_date, f_time";
    fReportGrid->fModel->setColumn(80, "", tr("Local"))
            .setColumn(50, "", tr("Line"))
            .setColumn(100, "", tr("Date"))
            .setColumn(80, "", tr("Time"))
            .setColumn(100, "", tr("Type"))
            .setColumn(100, "", tr("Remote"))
            .setColumn(100, "", tr("Duration"))
            .setColumn(80, "", tr("Price"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(0, "", tr("Doc"));
    fReportGrid->setupTabTextAndIcon(tr("History of calls"), ":/images/phone-call.png");
}

FCallHistory::~FCallHistory()
{
    delete ui;
}

QWidget *FCallHistory::firstElement()
{
    return ui->deStart;
}

QString FCallHistory::reportTitle()
{
    return QString("%1 %2 %3 %4 %5")
            .arg(tr("Call history"))
            .arg(tr("Date from"))
            .arg(ui->deStart->text())
            .arg(tr("Date to"))
            .arg(ui->deEnd->text());
}

void FCallHistory::apply(WReportGrid *rg)
{
    if (!ui->chIn->isChecked() && !ui->chOut->isChecked() && !ui->chInt->isChecked()
            && !ui->chTIn->isChecked() && !ui->chTOut->isChecked()) {
        message_error_tr("Select call type");
        return;
    }
    QString query = fQuery;
    query.replace(":f_date1", ui->deStart->dateMySql()).replace(":f_date2", ui->deEnd->dateMySql());
    QString ch;
    if (ui->chIn->isChecked()) {
        ch += "2,";
    }
    if (ui->chOut->isChecked()) {
        ch += "3,";
    }
    if (ui->chInt->isChecked()) {
        ch += "1,";
    }
    if (ui->chTIn->isChecked()) {
        ch += "5,";
    }
    if (ui->chTOut->isChecked()) {
        ch += "4,";
    }
    ch.remove(ch.length() - 1, 1);
    query.replace(":where", ch);
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 7;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FCallHistory::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCallHistory *ch = new FCallHistory(rg);
    rg->addFilterWidget(ch);
    ch->apply(rg);
}

void FCallHistory::on_toolButton_2_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
    apply(fReportGrid);
}

void FCallHistory::on_toolButton_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
    apply(fReportGrid);
}
