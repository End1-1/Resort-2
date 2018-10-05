#include "fnatbyperiod.h"
#include "ui_fnatbyperiod.h"
#include "wreportgrid.h"

FNatByPeriod::FNatByPeriod(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FNatByPeriod)
{
    ui->setupUi(this);
    ui->cbYear1->setCurrentIndex(ui->cbYear1->findText(QString::number(QDate::currentDate().year())));
    ui->cbYear2->setCurrentIndex(ui->cbYear2->findText(QString::number(QDate::currentDate().year())));
    ui->cbMonth1->setCurrentIndex(QDate::currentDate().month() - 1);
    ui->cbMonth2->setCurrentIndex(QDate::currentDate().month() - 1);
    fReportGrid->setupTabTextAndIcon(tr("Nationality report"), ":/images/reports.png");
}

FNatByPeriod::~FNatByPeriod()
{
    delete ui;
}

QWidget *FNatByPeriod::firstElement()
{
    return ui->cbYear1;
}

QString FNatByPeriod::reportTitle()
{
    return QString("%1 from %2 %3 to %4 %5")
            .arg(tr("Nationaly report for period"))
            .arg(ui->cbMonth1->currentText())
            .arg(ui->cbYear1->currentText())
            .arg(ui->cbMonth2->currentText())
            .arg(ui->cbYear2->currentText());
}

void FNatByPeriod::apply(WReportGrid *rg)
{
    QString sdate1 = ui->cbYear1->currentText() + "-" + QString("%1").arg(ui->cbMonth1->currentIndex() + 1, 2, 10, QChar('0')) + "-01";
    QString sdate2 = ui->cbYear2->currentText() + "-" + QString("%1").arg(ui->cbMonth2->currentIndex() + 1, 2, 10, QChar('0')) + "-01";
    QDate date1 = QDate::fromString(sdate1, def_mysql_date_format);
    QDate date2 = QDate::fromString(sdate2, def_mysql_date_format);
    date2 = date2.addDays(date2.daysInMonth()).addDays(-1);
    rg->fModel->clearColumns();
    rg->fModel->setColumn(70, "", tr("Index"))
            .setColumn(120, "", tr("Nationality"))
            .setColumn(80, "", tr("Guests"))
            .setColumn(80, "", tr("Nights"))
            .setColumn(80, "", tr("Free pax"))
            .setColumn(80, "", tr("Free nights"))
            .setColumn(80, "", tr("Rooming"))
            .setColumn(80, "", tr("Length of stay"))
            .setColumn(80, "", tr("Avg room"));
    QStringList queries;
    queries << "call nat_by_period('" + date1.toString(def_mysql_date_format) + "', '" + date2.toString(def_mysql_date_format) + "');";
    queries << "select * from rep";
    rg->fModel->apply(queries);
    QList<int> col;
    col << 2 << 3 << 4 << 5 << 6 << 7;
    QList<double> val;
    rg->fModel->sumOfColumns(col, val);
    rg->setTblTotalData(col, val);
}
