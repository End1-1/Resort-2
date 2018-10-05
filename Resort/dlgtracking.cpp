#include "dlgtracking.h"
#include "ui_dlgtracking.h"
#include "cacherights.h"

DlgTracking::DlgTracking(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgTracking)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                100, 90, 70, 200, 150, 200, 200);
}

DlgTracking::~DlgTracking()
{
    delete ui;
}

void DlgTracking::showTracking(int trackId, const QString &windowId)
{
    if (!r__(cr__tracking_chnages_individual)) {
        message_error(tr("Access denied"));
        return;
    }
    if (!fPreferences.getDb(def_show_logs).toBool()) {
        message_error(tr("Logs disabled"));
        return;
    }
    DlgTracking *d = new DlgTracking(fPreferences.getDefaultParentForMessage());
    d->fTrack = trackId;
    d->fWindow = windowId;
    QString query = "select f_comp, f_date, f_time, f_user, "
            "f_action, f_value1, f_value2 "
            "from airlog.log "
            "where f_type=:f_table and (f_rec=:f_rec or f_invoice=:f_invoice or f_reservation=:f_reservation) "
            "order by f_date, f_time";
    d->loadTrack(query, trackId, windowId);
    d->exec();
    delete d;
}

void DlgTracking::loadTrack(const QString &query, int trackId, const QString &windowId)
{
    fDbBind[":f_table"] = trackId;
    fDbBind[":f_rec"] = windowId;
    fDbBind[":f_reservation"] = windowId;
    fDbBind[":f_invoice"] = windowId;
    Database db;
    db.setConnectionParams(TrackControl::fDbHost, TrackControl::fDbDb, TrackControl::fDbUser, TrackControl::fDbPass);
    db.open();
    db.select(query, fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
    db.close();
}

void DlgTracking::on_chOld_clicked(bool checked)
{
    if (checked) {
        fDbBind[":f_windowId"] = fWindow;
        QString query = "select f_comp, f_date, f_time, f_user, "
                "f_key, f_old, f_new "
                "from f_changes_tracking "
                "where f_windowId=:f_windowId "
                "order by f_date, f_time";
        fDb.select(query, fDbBind, fDbRows);
        Utils::fillTableWithData(ui->tblData, fDbRows);
        Database db;
        db.setConnectionParams("10.1.0.44", "hna", "root", "rootjan2018");
        db.open();
        fDbBind[":f_windowId"] = fWindow;
        db.select(query, fDbBind, fDbRows);
        db.close();
        for (int i = 0; i < fDbRows.count(); i++) {
            Utils::tableAppendRowData(ui->tblData, fDbRows.at(i), Qt::DisplayRole);
        }
    } else {
        QString query = "select f_comp, f_date, f_time, f_user, "
                "f_action, f_value1, f_value2 "
                "from airlog.log "
                "where f_type=:f_table and (f_rec=:f_rec or f_invoice=:f_invoice or f_reservation=:f_reservation) "
                "order by f_date, f_time";
        fDbBind[":f_table"] = fTrack;
        fDbBind[":f_rec"] = fWindow;
        fDbBind[":f_reservation"] = fWindow;
        fDbBind[":f_invoice"] = fWindow;
        Database db;
        db.setConnectionParams(TrackControl::fDbHost, TrackControl::fDbDb, TrackControl::fDbUser, TrackControl::fDbPass);
        db.open();
        db.select(query, fDbBind, fDbRows);
        db.close();
        Utils::fillTableWithData(ui->tblData, fDbRows);
    }


}
