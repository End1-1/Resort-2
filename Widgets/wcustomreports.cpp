#include "wcustomreports.h"
#include "ui_wcustomreports.h"
#include "databaseresult.h"
#include "wreportgrid.h"
#include "ecomboboxcompleter.h"
#include "ecombomonth.h"
#include "edateedit.h"
#include <QSpacerItem>

WCustomReports::WCustomReports(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WCustomReports)
{
    ui->setupUi(this);
    DatabaseResult dr;
    dr.select("select r.f_id, g.f_name as f_groupName, r.f_name, r.f_groupAccess "
              "from serv_reports r "
              "left join serv_reports_group g on g.f_id=r.f_group "
              "order by g.f_queue ", fDbBind);
    QSet<QString> groups;
    for (int i = 0; i < dr.rowCount(); i++) {
        if (dr.value(i, "f_groupAccess").toString() == "*") {
            groups.insert(dr.value(i, "f_groupName").toString());
            continue;
        }
        QStringList access = dr.value(i, "f_groupAccess").toString().split(";", QString::SkipEmptyParts);
        if (access.contains(QString::number(WORKING_USERGROUP))) {
            groups.insert(dr.value(i, "f_groupName").toString());
        }
    }
    foreach (QString s, groups) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->trMenu);
        item->setText(0, s);
        ui->trMenu->addTopLevelItem(item);
    }
    for (int i = 0; i < dr.rowCount(); i++) {
        QString groupName = dr.value(i, "f_groupName").toString();
        for (int j = 0; j < ui->trMenu->topLevelItemCount(); j++) {
            if (ui->trMenu->topLevelItem(j)->text(0) == groupName) {
                QTreeWidgetItem *item = new QTreeWidgetItem(ui->trMenu->topLevelItem(j));
                item->setText(0, dr.value(i, "f_name").toString());
                item->setData(0, Qt::UserRole, dr.value(i, "f_id"));
                ui->trMenu->addTopLevelItem(item);
            }
        }
    }
    fReportGrid = new WReportGrid(ui->wdtReport);
    fReportGrid->fTitleWidget = this;
    ui->vlReport->addWidget(fReportGrid);
}

WCustomReports::~WCustomReports()
{
    delete ui;
}

void WCustomReports::setup()
{
    setupTabTextAndIcon(tr("Reports set"), ":/images/report.png");
}

void WCustomReports::on_btnShowMenu_clicked()
{
    ui->trMenu->setVisible(!ui->trMenu->isVisible());
}

void WCustomReports::on_trMenu_clicked(const QModelIndex &index)
{
    if (index.data(Qt::UserRole).toInt() == 0) {
        return;
    }
    foreach (QWidget* w, fFilterWidgets) {
        ui->lFilter->removeWidget(w);
        delete w;
    }
    fFilterWidgets.clear();
    ui->lFilter->removeItem(ui->lFilter->takeAt(0));
    fFilterFields.clear();
    fFilterDefExpr.clear();
    DatabaseResult dr;
    fDbBind[":f_id"] = index.data(Qt::UserRole);
    dr.select("select f_sql, f_widths, f_titles_en, f_filter, f_sum, f_subtotal from serv_reports where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Cannot load report data.");
        return;
    }
    QStringList filter = dr.value("f_filter").toString().split(";", QString::SkipEmptyParts);
    fFilterSQL = dr.value("f_sql").toString();
    fSumCols = dr.value("f_sum").toString();
    fSubtotal = dr.value("f_subtotal").toString();
    int i = 0;
    foreach (QString s, filter) {
        QStringList f = s.split("^", QString::KeepEmptyParts);
        QLabel *l = new QLabel(f.at(1));
        fFilterWidgets.append(l);
        ui->lFilter->addWidget(l);
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
            wdt = new EComboMonth();
            EComboMonth *cm = static_cast<EComboMonth*>(wdt);
            QStringList comboParams = f.at(4).split(":", QString::SkipEmptyParts);
            if (comboParams.at(0).toLower() == "sql") {
                cm->setSQL(comboParams.at(1));
            } else if (comboParams.at(0).toLower() == "list") {
                QStringList list = comboParams.at(1).split(",", QString::SkipEmptyParts);
                foreach (QString l, list) {
                    cm->addItem(l, l);
                }
            }
            if (comboParams.count() > 2) {
                cm->setCurrentIndex(comboParams.at(2).toInt());
            }
        } else {
            message_error_tr("Unknown filter widget. Contact to administrator.");
        }
        if (wdt) {
            fFilterWidgets.append(wdt);
            fFilterFields[wdt] = f.at(0);
            fFilterBuilds[wdt] = f.at(3);
            fFilterDefExpr[wdt] = f.at(5);
            ui->lFilter->addWidget(wdt);
            if (i == 1) {
                wdt->setFocus();
            }
        }
        i++;
    }
    ui->lFilter->addStretch();
    QPushButton *btnGo = new QPushButton(ui->wdtFilter);
    connect(btnGo, SIGNAL(clicked()), this, SLOT(executeSQL()));
    btnGo->setMaximumSize(25, 25);
    btnGo->setMinimumSize(25, 25);
    btnGo->setText("");
    btnGo->setIcon(QIcon(":/images/goto.png"));
    ui->lFilter->addWidget(btnGo);
    fFilterWidgets.append(btnGo);

    fReportGrid->fModel->clearColumns();
    QStringList widths = dr.value("f_widths").toString().split(";", QString::SkipEmptyParts);
    QStringList titles = dr.value("f_titles_en").toString().split(";", QString::SkipEmptyParts);
    for (int i = 0; i < widths.count(); i++) {
        fReportGrid->fModel->setColumn(widths.at(i).toInt(), "", titles.at(i));
    }

    executeSQL();
}

void WCustomReports::executeSQL()
{
    QString query = fFilterSQL;
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
    fReportGrid->fModel->apply(query.split(";", QString::SkipEmptyParts));

    fReportGrid->setTblNoTotalData();
    QStringList sums = fSumCols.split(";", QString::SkipEmptyParts);
    QList<int> cols;
    QList<double> vals;
    if (sums.count() > 0) {
        foreach (QString s, sums) {
            cols << s.toInt();
        }
        fReportGrid->fModel->sumOfColumns(cols, vals);
        fReportGrid->setTblTotalData(cols, vals);
    }

    QStringList subtotals = fSubtotal.split(";", QString::SkipEmptyParts);
    cols.clear();
    if (subtotals.count() > 1) {
        foreach (QString s, subtotals) {
            cols << s.toInt();
        }
        int colCompare = cols.at(0);
        cols.removeAt(0);
        fReportGrid->fModel->insertSubTotals(colCompare, cols);
    }
}
