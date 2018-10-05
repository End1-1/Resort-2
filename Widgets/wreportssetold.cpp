#include "wreportssetold.h"
#include "ui_wreportssetold.h"
#include "wreportgrid.h"
#include "databaseresult.h"
#include "edateedit.h"
#include "ecomboboxcompleter.h"

WReportsSetOld::WReportsSetOld(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WReportsSetOld)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);

    fBtnMonth.addButton(ui->rbJanuary);
    fBtnMonth.addButton(ui->rbFebrary);
    fBtnMonth.addButton(ui->rbMarch);
    fBtnMonth.addButton(ui->rbApril);
    fBtnMonth.addButton(ui->rbMay);
    fBtnMonth.addButton(ui->rbJune);
    fBtnMonth.addButton(ui->rbJuly);
    fBtnMonth.addButton(ui->rbAugust);
    fBtnMonth.addButton(ui->rbSeptember);
    fBtnMonth.addButton(ui->rbOctober);
    fBtnMonth.addButton(ui->rbNovember);
    fBtnMonth.addButton(ui->rbDecember);
    fBtnMonth.addButton(ui->rbYear);

    ui->rbJanuary->fData["month"] = 1;
    ui->rbFebrary->fData["month"] = 2;
    ui->rbMarch->fData["month"] = 3;
    ui->rbApril->fData["month"] = 4;
    ui->rbMay->fData["month"] = 5;
    ui->rbJune->fData["month"] = 6;
    ui->rbJuly->fData["month"] = 7;
    ui->rbAugust->fData["month"] = 8;
    ui->rbSeptember->fData["month"] = 9;
    ui->rbOctober->fData["month"] = 10;
    ui->rbNovember->fData["month"] = 11;
    ui->rbDecember->fData["month"] = 12;
    ui->rbYear->fData["month"] = 0;

    int month = QDate::currentDate().month();
    QList<QAbstractButton*> e = fBtnMonth.buttons();
    for (int i = 0; i < e.count(); i++) {
        ERadioButton *er = static_cast<ERadioButton*>(e.at(i));
        if (er->fData["month"].toInt() == month) {
            er->setChecked(true);
            break;
        }
    }


    fBtnGroup.addButton(ui->rbrCategory);
    fBtnGroup.addButton(ui->rbrCategoryYearly);
    fBtnGroup.addButton(ui->rbrOccupancyCategory);
    fBtnGroup.addButton(ui->rbrNationality);
    fBtnGroup.addButton(ui->rbrCardex);
    fBtnGroup.addButton(ui->rbrCardexYearly);
    fBtnGroup.addButton(ui->rbrNatYearly);
    fBtnGroup.addButton(ui->rbrNationalityPax);
    fBtnGroup.addButton(ui->rbrSalesMan);
    fBtnGroup.addButton(ui->rbrSalesManYearly);
    fBtnGroup.addButton(ui->rbrArrangement);
    fBtnGroup.addButton(ui->rbrPax);
    fBtnGroup.addButton(ui->rbrMarketSigmentYearly);
    fBtnGroup.addButton(ui->rbrMarketSigment);

    ui->rbrCategory->fData["rep"] = 5;
    ui->rbrCategoryYearly->fData["rep"] = 6;
    ui->rbrOccupancyCategory->fData["rep"] = 7;
    ui->rbrNationality->fData["rep"] = 8;
    ui->rbrCardex->fData["rep"] = 9;
    ui->rbrCardexYearly->fData["rep"] = 10;
    ui->rbrNatYearly->fData["rep"] = 11;
    ui->rbrNationalityPax->fData["rep"] = 120;

    ui->rbrSalesMan->fData["rep"] = 12;
    ui->rbrSalesManYearly->fData["rep"] = 13;

    ui->rbrArrangement->fData["rep"] = 14;

    ui->rbrMarketSigmentYearly->fData["rep"] = 15;
    ui->rbrMarketSigment->fData["rep"] = 16;

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [this] (int index) {
        QWidget *w = ui->tabWidget->widget(index);
        w->deleteLater();
        ui->tabWidget->removeTab(index);
    });

    ui->cbYear->setCurrentIndex(ui->cbYear->findText(QDate::currentDate().toString("yyyy")));
}

WReportsSetOld::~WReportsSetOld()
{
    delete ui;
}

void WReportsSetOld::setup()
{
    setupTabTextAndIcon(tr("Statistics"), ":/images/report.png");
}

QString WReportsSetOld::title()
{
    return "";
}

void WReportsSetOld::rbClicked()
{
    ERadioButton *rb = static_cast<ERadioButton*>(sender());
    DatabaseResult dr;
    fDbBind[":f_id"] = rb->fData["rep"];
    dr.select("select f_sql, f_widths, f_titles_en, f_filter, f_sum from serv_reports where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Cannot load report data.");
        return;
    }
    foreach (QWidget* w, fFilterWidgets) {
        ui->hlFilter->removeWidget(w);
        delete w;
    }
    fFilterWidgets.clear();
    ui->hlFilter->removeItem(ui->hlFilter->takeAt(0));
    fFilterFields.clear();
    fFilterDefExpr.clear();
    QStringList filter = dr.value("f_filter").toString().split(";", QString::SkipEmptyParts);
    int i = 0;
    foreach (QString s, filter) {
        QStringList f = s.split("^", QString::KeepEmptyParts);
        if (f.at(0) == ":year" || f.at(0) == ":month") {
            continue;
        }
        QLabel *l = new QLabel(f.at(1));
        fFilterWidgets.append(l);
        ui->hlFilter->addWidget(l);
        QWidget *wdt = 0;
        if (f.at(2).toLower() == "date") {
            wdt = new EDateEdit();
        } else if (f.at(2).toLower() == "integer") {

        } else if (f.at(2).toLower() == "combobox") {
            wdt = new EComboBoxCompleter();
            EComboBoxCompleter *cc = static_cast<EComboBoxCompleter*>(wdt);
            QStringList comboParams = f.at(4).split(":", QString::SkipEmptyParts);
            if (comboParams.at(0).toLower() == "sql") {
                cc->setSQL(comboParams.at(1));
            } else if (comboParams.at(0).toLower() == "list") {
                QStringList list = comboParams.at(1).split(",", QString::SkipEmptyParts);
                foreach (QString l, list) {
                    cc->addItem(l, l);
                }
            }
            if (comboParams.count() > 2) {
                cc->setCurrentIndex(comboParams.at(2).toInt());
            }
        } else if (f.at(2).toLower() == "combo month") {

        } else {
            message_error_tr("Unknown filter widget. Contact to administrator.");
        }
        if (wdt) {
            fFilterWidgets.append(wdt);
            fFilterFields[wdt] = f.at(0);
            fFilterBuilds[wdt] = f.at(3);
            fFilterDefExpr[wdt] = f.at(5);
            ui->hlFilter->addWidget(wdt);
            if (i == 1) {
                wdt->setFocus();
            }
        }
        i++;
    }
    ui->hlFilter->addStretch();
}

void WReportsSetOld::on_btnGo_clicked()
{
    ERadioButton *eb = static_cast<ERadioButton*>(fBtnGroup.checkedButton());
    if (!eb) {
        return;
    }
    fDbBind[":f_id"] = eb->fData["rep"];
    DatabaseResult dr;
    dr.select("select f_sql, f_widths, f_titles_en, f_filter, f_sum, f_name from serv_reports where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Cannot load report data.");
        return;
    }

    QString fFilterSQL = dr.value("f_sql").toString();
    QString reportTitle = QString("%1 %2%3")
            .arg(dr.value("f_name").toString())
            .arg(ui->cbYear->currentText())
            .arg(static_cast<ERadioButton*>(fBtnMonth.checkedButton())->fData["month"].toInt() > 0 ? "-" + static_cast<ERadioButton*>(fBtnMonth.checkedButton())->fData["month"].toString() : "");

    WReportGrid *rg = new WReportGrid(this);
    ui->tabWidget->addTab(rg, QIcon(":/images/report.png"), reportTitle);
    rg->setTab(ui->tabWidget, ui->tabWidget->count() - 1);
    rg->setupTabTextAndIcon(reportTitle, ":/images/report.png");

    rg->fModel->clearColumns();
    QStringList widths = dr.value("f_widths").toString().split(";", QString::SkipEmptyParts);
    QStringList titles = dr.value("f_titles_en").toString().split(";", QString::SkipEmptyParts);
    for (int i = 0; i < widths.count(); i++) {
        rg->fModel->setColumn(widths.at(i).toInt(), "", titles.at(i));
    }

    QString query = fFilterSQL;
    query.replace(":year", ui->cbYear->currentText())
            .replace(":month", static_cast<ERadioButton*>(fBtnMonth.checkedButton())->fData["month"].toString());

    for (QMap<QWidget*, QString>::const_iterator it = fFilterFields.begin(); it != fFilterFields.end(); it++) {
       if (isEDateEdit(it.key())) {
           EDateEdit *e = static_cast<EDateEdit*>(it.key());
           query.replace(it.value(), e->dateMySql());
       } else if (isComboCompleter(it.key()) || isComboMonth(it.key())) {
           EComboBoxCompleter *c = static_cast<EComboBoxCompleter*>(it.key());
           if (c->currentData().toString().isEmpty()) {
               query.replace(it.value(), fFilterDefExpr[c]);
           } else {
               QString repStr = fFilterBuilds[c];
               query.replace(it.value(), repStr.replace(it.value(), c->currentData().toString()));
           }
       }
    }


    rg->fModel->apply(query.split(";", QString::SkipEmptyParts));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    rg->setTblNoTotalData();
    QStringList sums = dr.value("f_sum").toString().split(";", QString::SkipEmptyParts);
    QList<int> cols;
    QList<double> vals;
    if (sums.count() > 0) {
        foreach (QString s, sums) {
            cols << s.toInt();
        }
        rg->fModel->sumOfColumns(cols, vals);
        rg->setTblTotalData(cols, vals);
    }
}
