#include "dlgsalary.h"
#include "ui_dlgsalary.h"
#include "dlglist.h"
#include "dlgreservation.h"
#include "rmessage.h"
#include "rnumbers.h"
#include "cacheusersgroups.h"
#include "cacheusers.h"

DlgSalary::DlgSalary(bool day, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSalary)
{
    ui->setupUi(this);
    ui->deDate->setDate(QDate::currentDate());
    showMaximized();
    fDay = day;
    ui->btnCopyFromReal->setVisible(fDay);
    if (fDay) {
        loadSalary2();
    } else {
        loadSalary();
    }
}

DlgSalary::~DlgSalary()
{
    delete ui;
}

void DlgSalary::salary()
{
    DlgSalary *d = new DlgSalary(false);
    d->exec();
    delete d;
}

void DlgSalary::salary2()
{
    DlgSalary *d = new DlgSalary(true);
    d->exec();
    delete d;
}

void DlgSalary::loadSalary2()
{
    fDbBind[":f_date"] = ui->deDate->date();
    DatabaseResult dr;
    dr.select(fDb, "select s.f_id, s.f_employee, concat(u.f_firstName, ' ', u.f_lastName) as ename, abs(s.f_amount) as f_amount, "
              "p.f_am as position "
               "from salary2 s "
               "left join users u on u.f_id=s.f_employee "
                "left join users_groups p on p.f_id=u.f_group "
                "where s.f_date=:f_date", fDbBind);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 0, 500, 100, 400);
    ui->tblData->setRowCount(0);
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        ui->tblData->setItemWithValue(row, 0, dr.value(i, "f_id"));
        ui->tblData->setItemWithValue(row, 1, dr.value(i, "f_employee"));
        ui->tblData->setItemWithValue(row, 2, dr.value(i, "ename"));
        ui->tblData->setItemWithValue(row, 3, dr.value(i, "f_amount"));
        ui->tblData->setItemWithValue(row, 4, dr.value(i, "position"));
    }
}

void DlgSalary::loadSalary()
{
    fDbBind[":f_date"] = ui->deDate->date();
    DatabaseResult dr;
    dr.select(fDb, "select s.f_id, s.f_employee, concat(u.f_firstName, ' ', u.f_lastName) as ename, abs(s.f_amount) as f_amount, "
              "p.f_am as position "
               "from salary s "
               "left join users u on u.f_id=s.f_employee "
                "left join users_groups p on p.f_id=u.f_group "
                "where s.f_date=:f_date", fDbBind);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 0, 500, 100, 400);
    ui->tblData->setRowCount(0);
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        ui->tblData->setItemWithValue(row, 0, dr.value(i, "f_id"));
        ui->tblData->setItemWithValue(row, 1, dr.value(i, "f_employee"));
        ui->tblData->setItemWithValue(row, 2, dr.value(i, "ename"));
        ui->tblData->setItemWithValue(row, 3, dr.value(i, "f_amount"));
        ui->tblData->setItemWithValue(row, 4, dr.value(i, "position"));
    }
}

void DlgSalary::on_btnClose_clicked()
{
    accept();
}

void DlgSalary::on_btnSave_clicked()
{
    if (fDay) {
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            if (ui->tblData->toInt(i, 0) == 0) {
                fDbBind[":f_employee"] = ui->tblData->toInt(i, 1);
                fDbBind[":f_date"] = ui->deDate->date();
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                ui->tblData->setItemWithValue(i, 0, fDb.insert("salary2", fDbBind));
            } else {
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                fDbBind[":f_id"] = ui->tblData->toInt(i, 0);
                fDb.select("update salary2 set f_amount=:f_amount where f_id=:f_id", fDbBind, fDbRows);
            }
        }
    } else {
        for (int i = 0; i < ui->tblData->rowCount(); i++) {
            if (ui->tblData->toInt(i, 0) == 0) {
                fDbBind[":f_employee"] = ui->tblData->toInt(i, 1);
                fDbBind[":f_date"] = ui->deDate->date();
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                ui->tblData->setItemWithValue(i, 0, fDb.insert("salary", fDbBind));
            } else {
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                fDbBind[":f_id"] = ui->tblData->toInt(i, 0);
                fDb.select("update salary set f_amount=:f_amount where f_id=:f_id", fDbBind, fDbRows);
            }
        }
        countSalary();
    }
    message_info(tr("Saved"));
}

void DlgSalary::on_btnAdd_clicked()
{
    DlgReservation *d = new DlgReservation();
    d->loadEmployes();
    d->exec();
    int id = d->fItemCode;
    QString name = d->fReservationId;
    int row = ui->tblData->rowCount();
    ui->tblData->setRowCount(row + 1);
    ui->tblData->setItemWithValue(row, 0, 0);
    ui->tblData->setItemWithValue(row, 1, id);
    ui->tblData->setItemWithValue(row, 2, name);
    ui->tblData->setItemWithValue(row, 3, 0);
    CI_User *u = CacheUsers::instance()->get(id);
    if (u) {
        CI_UsersGroups *ug = CacheUsersGroups::instance()->get(u->fGroup);
        if (ug) {
            ui->tblData->setItemWithValue(row, 4, ug->fName);
        } else {
            ui->tblData->setItemWithValue(row, 4, "");
        }
    } else {
        ui->tblData->setItemWithValue(row, 4, "");
    }
    delete d;
}

void DlgSalary::on_btnRemove_clicked()
{
    QModelIndexList rows = ui->tblData->selectionModel()->selectedRows();
    if (rows.count() == 0) {
        return;
    }
    if (!message_question(tr("Confirm to remove the selected employee"))) {
        return;
    }
    int id = ui->tblData->toInt(rows.at(0).row(), 0);
    if (fDay) {
        if (id) {
            fDbBind[":f_id"] = id;
            fDb.select("delete from salary2 where f_id=:f_id", fDbBind, fDbRows);
        }
    } else {
        if (id) {
            fDbBind[":f_id"] = id;
            fDb.select("delete from salary where f_id=:f_id", fDbBind, fDbRows);
        }
    }
    ui->tblData->removeRow(rows.at(0).row());
}

void DlgSalary::countSalary()
{
    fDbBind[":f_date"] = ui->deDate->date();
    fDbBind[":f_docType"] = 3;
    fDb.select("delete from c_cash where f_date=:f_date and f_docType=:f_docType", fDbBind, fDbRows);

    DatabaseResult drsal;
    fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
    fDbBind[":f_state2"] = DISH_STATE_READY;
    fDbBind[":f_dateCash"] =  ui->deDate->date();
    fDbBind[":f_store"] = 2;
    drsal.select(fDb, "select sum(d.f_totalUSD) as f_total from o_dish d "
                 "left join o_header h on h.f_id=d.f_header "
                 "where h.f_state=:f_state1 and d.f_state=:f_state2 "
                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store", fDbBind);

    if (drsal.value("f_total").toDouble() > 0.1) {
        fDbBind[":f_date"] =  ui->deDate->date();
        fDbBind[":f_docType"] = 3;
        fDbBind[":f_debit"] = 1;
        fDbBind[":f_credit"] = 2;
        fDbBind[":f_amount"] = drsal.value("f_total").toDouble() * -1 * 0.4;
        fDbBind[":f_comment"] = tr("Wash");
        fDb.insert("c_cash", fDbBind);
    }
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->toDouble(i, 3) > 0.01) {
            fDbBind[":f_date"] =  ui->deDate->date();
            fDbBind[":f_docType"] = 3;
            fDbBind[":f_debit"] = 1;
            fDbBind[":f_credit"] = 2;
            fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3) * -1;
            fDbBind[":f_comment"] = ui->tblData->toString(i, 4);
            fDb.insert("c_cash", fDbBind);
        }
    }
}

void DlgSalary::on_btnAmount_clicked()
{
    QModelIndexList rows = ui->tblData->selectionModel()->selectedRows();
    if (rows.count() == 0) {
        return;
    }
    float num = 0;
    if (!RNumbers::getNumber(num, 1000000, this)) {
        return;
    }
    ui->tblData->setItemWithValue(rows.at(0).row(), 3, num);
}

void DlgSalary::on_deDate_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
    if (fDay) {
        loadSalary2();
    } else {
        loadSalary();
    }
}

void DlgSalary::on_btnCopyFromReal_clicked()
{
    loadSalary();
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        ui->tblData->setItemWithValue(i, 0, 0);
    }
}
