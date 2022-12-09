#include "fattendance.h"
#include "ui_fattendance.h"
#include "wreportgrid.h"
#include "reportquery.h"
#include "dlgaddattendance.h"
#include "database2.h"
#include "dlgconfigattendance.h"
#include "dlggetidname.h"

FAttendance::FAttendance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAttendance)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Attendance"), ":/images/staff.png");
    fReportQuery = new ReportQuery("attendance_1");
    if (check_permission(pr_edit_attendance_list)) {
        fReportGrid->addToolBarButton(":/images/new.png", tr("Add"), SLOT(newWorker()), this)->setFocusPolicy(Qt::NoFocus);
        fReportGrid->addToolBarButton(":/images/cancel.png", tr("Remove"), SLOT(removeWorker()), this)->setFocusPolicy(Qt::NoFocus);
        fReportGrid->addToolBarButton(":/images/update.png", tr("Config"), SLOT(config()), this)->setFocusPolicy(Qt::NoFocus);
    }
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FAttendance::branchDoubleClicked);
}

FAttendance::~FAttendance()
{
    delete ui;
}

void FAttendance::apply(WReportGrid *rg)
{
    QString query = fReportQuery->query;
    query.replace(":date1", ui->led1->dateMySql()).replace(":date2", ui->led2->dateMySql());
    if (ui->leBranch->fHiddenText.isEmpty()) {
        query.replace(":branch", "");
    } else {
        query.replace(":branch", " and s.f_branch in (" + ui->leBranch->fHiddenText + ")");
    }
    rg->fModel->clearColumns();
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
//    QList<double> vals;
//    rg->fModel->sumOfColumns(fReportQuery->sumColumns, vals);
//    rg->setTblTotalData(fReportQuery->sumColumns, vals);
    for (QMap<int, int>::const_iterator it = fReportQuery->columnsWidths.constBegin(); it != fReportQuery->columnsWidths.constEnd(); it++) {
        rg->fTableView->setColumnWidth(it.key(), it.value());
    }
}

QWidget *FAttendance::firstElement()
{
    return ui->led1;
}

QString FAttendance::reportTitle()
{
    return QString("%1 %2 - %3").arg(tr("Attendance"), ui->led1->text(), ui->led2->text());
}

void FAttendance::branchDoubleClicked(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}

void FAttendance::newWorker()
{
    DlgAddAttendance d(this);
    if (d.exec() == QDialog::Accepted) {
        apply(fReportGrid);
    }
}

void FAttendance::removeWorker()
{
    QModelIndexList sr = fReportGrid->fTableView->selectionModel()->selectedIndexes();
    QSet<int> rows;
    for (QModelIndex &mi: sr) {
        rows.insert(mi.row());
    }
    QList<int> rl = rows.toList();
    if (rl.count() > 0) {
        if (message_confirm_tr("Confirm to delete worker") != QDialog::Accepted) {
            return;
        }
    } else {
        return;
    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    for (int i = rl.count() - 1; i > -1; i--) {
        int docnum = fReportGrid->fModel->data(i, 0).toInt();
        db[":f_id"] = docnum;
        db.exec("delete from salary2 where f_id=:f_id");
    }
    apply(fReportGrid);
}

void FAttendance::config()
{
    DlgConfigAttendance(this).exec();
}
