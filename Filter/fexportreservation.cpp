#include "fexportreservation.h"
#include "ui_fexportreservation.h"
#include "wreportgrid.h"
#include "vauchers.h"
#include "wreservation.h"
#include "dwselectorreservestate.h"

FExportReservation::FExportReservation(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FExportReservation)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Export reservations"), ":/images/database.png");
    fReportGrid->addToolBarButton(":/images/invoice.png", tr("Open invoice"), SLOT(openInvoice()), this)->setFocusPolicy(Qt::ClickFocus);
    fReportGrid->addToolBarButton(":/images/bed.png", tr("Open reservation"), SLOT(openReservation()), this)->setFocusPolicy(Qt::ClickFocus);
    DWSelectorReserveState *dockState = new DWSelectorReserveState(this);
    dockState->configure();
    dockState->setSelector(ui->leStateCode);
    connect(dockState, &DWSelectorReserveState::reserveState, [this](CI_ReserveState *ci) {
        dock<CI_ReserveState, CacheReserveState>(ui->leStateCode, ui->leStateName, ci);
    });
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
}

FExportReservation::~FExportReservation()
{
    delete ui;
}

QString FExportReservation::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Export reservations"))
            .arg(ui->wd->dss1())
            .arg(ui->wd->dss2());
}

QWidget *FExportReservation::firstElement()
{
    return ui->wd->fw();
}

void FExportReservation::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(30, "", tr(""))
            .setColumn(30, "", tr(""))
            .setColumn(80, "", tr("Code"))
            .setColumn(80, "", tr("Invoice"))
            .setColumn(100, "", tr("State"))
            .setColumn(120, "", tr("Entry"))
            .setColumn(120, "", tr("Departure"))
            .setColumn(250, "", tr("Guest"))
            .setColumn(200, "", tr("Cardex"));
    QString query = "select '', '', r.f_id, r.f_invoice, rs.f_" + def_lang + ", "
            "r.f_startDate, r.f_endDate, g.guest, c.f_name "
            "from f_reservation r "
            "left join f_reservation_state rs on rs.f_id=r.f_state "
            "left join guests g on g.f_id=r.f_guest "
            "left join f_cardex c on c.f_cardex=r.f_cardex "
            "where r.f_state<>0 ";
    if (ui->leStateCode->asInt() > 0) {
        query += " and r.f_state=" + ui->leStateCode->text();
    }
    if (ui->rbEntry->isChecked()) {
        query += " and r.f_startDate between " + ui->wd->ds1() + " and " + ui->wd->ds2();
    } else {
        query += " and r.f_endDate between " + ui->wd->ds1() + " and " + ui->wd->ds2() + "";
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    rg->fModel->fCheckBoxColumns.append(0);
    rg->fModel->fCheckBoxColumns.append(1);
    rg->fModel->fCheckBoxIsCheckable.append(0);

}

void FExportReservation::openInvoice()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        return;
    }
    openInvoiceWithId(out.at(3).toString());
}

void FExportReservation::openReservation()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        return;
    }
    WReservation::openReserveWindows(out.at(2).toString());
}

void FExportReservation::on_btnStartExport_clicked()
{
    if (message_confirm(tr("Confirm to start export reservations")) != QDialog::Accepted) {
        return;
    }
    for (int i = 0; i < fReportGrid->fModel->rowCount(); i++) {
        if (fReportGrid->fModel->data(i, 0, Qt::CheckStateRole).toInt() == Qt::Checked) {
            exportReservation(fReportGrid->fModel->data(i, 2).toString());
            fReportGrid->fModel->setData(i, 1, 1);
        }
    }
    message_info(tr("Done"));
}

void FExportReservation::exportReservation(const QString &id)
{
    QString errStr;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    Database sDb;
    sDb.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!sDb.open()) {
        message_error("Connection error, " + sDb.fLastError);
        return;
    }
    sDb.fDb.transaction();

    QList<QList<QVariant> > o;
    QMap<QString, QVariant> b;
    QMap<QString, QVariant> sb;

    DatabaseResult dr;
    b[":f_id"] = id;
    dr.select(fDb, "select * from f_reservation where f_id=:f_id", b);
    dr.getBindValues(0, sb);
    if (ui->chOverride->isChecked()) {
        b[":f_id"] = sb[":f_id"];
        sDb.select("delete from f_reservation where f_id=:f_id", b, o);
    }
    if (!sDb.insertWithoutId("f_reservation", sb)) {
        errStr += sDb.fLastError + "<br>";
    }

    if (dr.value("f_group").toInt() > 0) {
        DatabaseResult dt;
        b[":f_id"] = dr.value(0, "f_group").toInt();
        dt.select(fDb, "select * from f_reservation_group where f_id=:f_id", b);

        if (dt.rowCount() == 0) {
            errStr += "Missing group for this reservation <br>";
        }
        if (ui->chOverride->isChecked()) {
            b[":f_id"] = dr.value(0, "f_group").toInt();
            sDb.select("delete from f_reservation_group where f_id=:f_id", b, o);
        }
        dt.getBindValues(0, sb);
        if (sb[":f_extra1"].toString().trimmed().length() > 0) {
            sb[":f_name"] = sb[":f_extra1"];
        }
        if (ui->chOverride->isChecked()) {
            b[":f_id"] = sb[":f_id"];
            sDb.select("delete form f_reservation_group where f_id=:f_id", b, o);
        }
        if (!sDb.insertWithoutId("f_reservation_group", sb)) {
            errStr += sDb.fLastError;
        }
    }

    DatabaseResult dr2;
    b[":f_inv"] = dr.value("f_invoice");
    dr2.select(fDb, "select * from m_register where f_inv=:f_inv", b);
    for (int i = 0; i < dr2.rowCount(); i++) {
        dr2.getBindValues(i, sb);
        b[":f_id"] = sb[":f_id"];
        if (ui->chOverride->isChecked()) {
            sDb.select("delete from m_register where f_id=:f_id", b, o);
        }
        if (!sDb.insertWithoutId("m_register", sb)) {
            errStr += sDb.fLastError + "<br>";
        }
    }

        //RESTAURANT VOUCHERS OF ONLINE RESERVATIONS

    for (int i = 0; i < dr2.rowCount(); i++) {
        if (dr2.value(i, "f_source").toString() == VAUCHER_POINT_SALE_N) {
            b[":f_id"] = dr2.value(i, "f_id");
            DatabaseResult dt1;
            dt1.select(fDb, "select * from o_header where f_id=:f_id", b);
            if (dt1.rowCount() == 0) {
                errStr += "Missing header for " + dr2.value(i, "f_id").toString() + "<br>";
                continue;
            }
            dt1.getBindValues(0, sb);
            if (ui->chOverride->isChecked()) {
                b[":f_id"] = dr2.value(i, "f_id");
                sDb.select("delete from o_header where f_id=:f_id", b, o);
            }
            if(!sDb.insertWithoutId("o_header", sb)) {
               errStr += sDb.fLastError + "<br>";
            }
            b[":f_header"] = dr2.value(i, "f_id");
            DatabaseResult dt2;
            dt2.select(fDb, "select * from o_dish where f_header=:f_header", b);
            for (int j = 0; j < dt2.rowCount(); j++) {
                dt2.getBindValues(j, sb);
                if (ui->chOverride->isChecked()) {
                    b[":f_id"] = sb[":f_id"];
                    sDb.select("delete from o_dish where f_id=:f_id", b, o);
                }
                if (!sDb.insertWithoutId("o_dish", sb)) {
                    errStr += sDb.fLastError + "<br>";
                }

            }
        }
    }

    DatabaseResult dr3;
    b[":f_id"] = id;
    dr3.select(fDb, "select * from f_guests where f_id in (select f_guest from f_reservation where f_id=:f_id) "
               "or f_id in (select f_guest from f_reservation_guests where f_reservation in (select f_id from f_reservation where f_state=:f_state1 or f_state=:f_state2))", b);

    for (int i = 0; i < dr3.rowCount(); i++) {
        dr3.getBindValues(i, sb);
        b[":f_id"] = sb[":f_id"];
        sDb.select("delete from f_guests where f_id=:f_id", b, o);
        if (!sDb.insertWithoutId("f_guests", sb)){
            errStr += sDb.fLastError + "<br>";
        }
    }
    //END GUESTS

    //F_RESERVATION_GUESTS

    DatabaseResult dr4;
    b[":f_id"] = id;
    dr4.select(fDb, "select * from f_reservation_guests where f_reservation=:f_id", b);
    b[":f_reservation"] = id;
    sDb.select("delete from f_reservation_guests where f_reservation=:f_reservation", b, o);


    for (int i = 0; i < dr4.rowCount(); i++) {
        dr4.getBindValues(i, sb);
        sb.remove(":f_id");
        if(!sDb.insertWithoutId("f_reservation_guests", sb)) {
            errStr += sDb.fLastError + "<br>";
        }
    }

    if (errStr.length() > 0) {
        message_error(errStr);
        sDb.fDb.rollback();
    } else {
        sDb.fDb.commit();
    }
}
