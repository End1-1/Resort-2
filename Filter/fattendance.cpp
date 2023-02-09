#include "fattendance.h"
#include "ui_fattendance.h"
#include "wreportgrid.h"
#include "reportquery.h"
#include "dlgaddattendance.h"
#include "database2.h"
#include "dlgconfigattendance.h"
#include "dlggetidname.h"
#include "dlgattendanceselectworkers.h"

FAttendance::FAttendance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAttendance)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Attendance"), ":/images/staff.png");
    fReportQuery = new ReportQuery("attendance_1");
    if (check_permission(pr_edit_attendance_list)) {
        fReportGrid->addToolBarButton(":/images/new.png", tr("Add"), SLOT(newWorker()), this)->setFocusPolicy(Qt::NoFocus);
        fReportGrid->addToolBarButton(":/images/new.png", "Ավելցանել ցանկով", SLOT(newWorkers()), this)->setFocusPolicy(Qt::NoFocus);
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

void FAttendance::newWorkers()
{
    DlgAttendanceSelectWorkers d(this);
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
        int docnum = fReportGrid->fModel->data(rl.at(i), 0).toInt();
        db[":f_id"] = docnum;
        db.exec("select * from salary2 where f_id=:f_id");
        if (db.next() == false) {
            message_error("Սխալ տող");
        }
        QDate date = db.date("f_date");
        int branch = db.integer("f_branch");
        int worker = db.integer("f_employee");
        db[":f_date"] = date;
        db[":f_branch"] = branch;
        db[":f_employee"] = worker;
        db.exec("delete from salary2 where f_date=:f_date and f_branch=:f_branch and f_employee=:f_employee");
        db[":f_date"] = date;
        db[":f_branch"] = branch;
        db[":f_employee"] = worker;
        db.exec("delete from salary where f_date=:f_date and f_branch=:f_branch and f_employee=:f_employee");
    }
    apply(fReportGrid);
}

void FAttendance::config()
{
    DlgConfigAttendance(this).exec();
}
