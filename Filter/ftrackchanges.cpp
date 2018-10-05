#include "ftrackchanges.h"
#include "ui_ftrackchanges.h"
#include "wreportgrid.h"

FTrackChanges::FTrackChanges(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FTrackChanges)
{
    ui->setupUi(this);
    fReportGrid->fModel->fDD.setDatabase(TrackControl::fDbHost, TrackControl::fDbDb, TrackControl::fDbUser, TrackControl::fDbPass, 1);
    ui->deStart->setDate(QDate::currentDate().addDays(-30));
    ui->deEnd->setDate(QDate::currentDate());
    fDockTable = new DWSelectorTrackingTables(this);
    fDockTable->configure();
    fDockTable->setSelector(ui->leType);
    connect(fDockTable, SIGNAL(tracking(CI_TrackingTables*)), this, SLOT(tracking(CI_TrackingTables*)));
    fDockUser = new DWSelectorUsers();
    fDockUser->configure();
    fDockUser->setSelector(ui->leUser);
    connect(fDockUser, SIGNAL(user(CI_User*)), this, SLOT(user(CI_User*)));
}

FTrackChanges::~FTrackChanges()
{
    delete ui;
}

void FTrackChanges::apply(WReportGrid *rg)
{
    if (!fPreferences.getDb(def_show_logs).toBool()) {
        message_error(tr("Logs disabled"));
        return;
    }
    QString where = "where (c.f_date between '"
            + ui->deStart->date().toString(def_mysql_date_format) + "' and '"
            + ui->deEnd->date().toString(def_mysql_date_format) + "') "
            + "and (c.f_time between '"
            + ui->teStart->time().toString("HH:mm") + "' and '"
            + ui->teEnd->time().toString("HH:mm") + "') ";
    if (!ui->leUser->fHiddenText.isEmpty()) {
        where += " and c.f_user in (" + ui->leUser->fHiddenText + ") ";
    }
    if (!ui->leType->fHiddenText.isEmpty()) {
        where += " and c.f_table in (" + ui->leType->fHiddenText + ") ";
    }
    where += " order by f_date desc, f_time desc ";
    buildQuery(rg, where);
}

QWidget *FTrackChanges::firstElement()
{
    return ui->deStart;
}

void FTrackChanges::tracking(CI_TrackingTables *c)
{
    dockResponse<CI_TrackingTables, CacheTrackingTables>(ui->leType, c);
}

void FTrackChanges::user(CI_User *c)
{
    dockResponse<CI_User,  CacheUsers>(ui->leUser, c);
}
