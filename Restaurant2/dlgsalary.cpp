#include "dlgsalary.h"
#include "ui_dlgsalary.h"
#include "dlglist.h"
#include "dlgreservation.h"
#include "branchstoremap.h"
#include "rmessage.h"
#include "rnumbers.h"
#include "cacheusersgroups.h"
#include "cacheusers.h"
#include "database2.h"
#include "defrest.h"
#include "dlgsalarytotal.h"
#include "c5printing.h"
#include <QInputDialog>

DlgSalary::DlgSalary(bool day, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSalary)
{
    ui->setupUi(this);
    ui->deDate->setDate(QDate::currentDate());
    showMaximized();
    fDay = day;
    ui->btnCopyFromReal->setVisible(fDay);

    if(fDay) {
        loadSalary2();
    } else {
        loadSalary();
    }

    QString field = QString("f%1").arg(QDate::currentDate().dayOfWeek());
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2.exec(QString("select %1 from salary2_config ").arg(field));
    db2.next();
    ui->btnAdd->setEnabled(db2.integer(field) > 0);
    ui->btnRemove->setEnabled(db2.integer(field) > 0);
    ui->btnCopyFromReal->setEnabled(db2.integer(field) > 0);
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
    fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    DatabaseResult dr;
    dr.select(fDb, "select s.f_id, s.f_employee, concat(u.f_firstName, ' ', u.f_lastName) as ename, abs(s.f_amount) as f_amount, "
                   "p.f_am as position "
                   "from salary2 s "
                   "left join users u on u.f_id=s.f_employee "
                   "left join users_groups p on p.f_id=u.f_group "
                   "where s.f_date=:f_date and s.f_branch=:f_branch ", fDbBind);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 0, 500, 100, 400);
    ui->tblData->setRowCount(0);

    for(int i = 0; i < dr.rowCount(); i++) {
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
    fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    DatabaseResult dr;
    dr.select(fDb, "select s.f_id, s.f_employee, concat(u.f_firstName, ' ', u.f_lastName) as ename, abs(s.f_amount) as f_amount, "
                   "p.f_am as position "
                   "from salary s "
                   "left join users u on u.f_id=s.f_employee "
                   "left join users_groups p on p.f_id=u.f_group "
                   "where s.f_date=:f_date and s.f_branch=:f_branch ", fDbBind);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 0, 500, 100, 400);
    ui->tblData->setRowCount(0);

    for(int i = 0; i < dr.rowCount(); i++) {
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
    if(fDay) {
        for(int i = 0; i < ui->tblData->rowCount(); i++) {
            if(ui->tblData->toInt(i, 0) == 0) {
                fDbBind[":f_employee"] = ui->tblData->toInt(i, 1);
                fDbBind[":f_date"] = ui->deDate->date();
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                fDbBind[":f_branch"] = defrest(dr_branch).toInt();
                ui->tblData->setItemWithValue(i, 0, fDb.insert("salary2", fDbBind));
            } else {
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                fDbBind[":f_id"] = ui->tblData->toInt(i, 0);
                fDb.select("update salary2 set f_amount=:f_amount where f_id=:f_id", fDbBind, fDbRows);
            }
        }
    } else {
        for(int i = 0; i < ui->tblData->rowCount(); i++) {
            if(ui->tblData->toInt(i, 0) == 0) {
                fDbBind[":f_employee"] = ui->tblData->toInt(i, 1);
                fDbBind[":f_date"] = ui->deDate->date();
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 3);
                fDbBind[":f_branch"] = defrest(dr_branch).toInt();
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

    if(u) {
        CI_UsersGroups *ug = CacheUsersGroups::instance()->get(u->fGroup);

        if(ug) {
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

    if(rows.count() == 0) {
        return;
    }

    if(!message_question(tr("Confirm to remove the selected employee"))) {
        return;
    }

    int id = ui->tblData->toInt(rows.at(0).row(), 0);

    if(fDay) {
        if(id) {
            fDbBind[":f_id"] = id;
            fDb.select("delete from salary2 where f_id=:f_id", fDbBind, fDbRows);
        }
    } else {
        if(id) {
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
    fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    fDb.select("delete from c_cash where f_branch=:f_branch and f_date=:f_date and f_docType=:f_docType", fDbBind, fDbRows);
    DatabaseResult drsal;
    fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
    fDbBind[":f_state2"] = DISH_STATE_READY;
    fDbBind[":f_dateCash"] =  ui->deDate->date();
    fDbBind[":f_store"] = storealias(2);
    fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    drsal.select(fDb, "select sum(d.f_totalUSD) as f_total from o_dish d "
                      "left join o_header h on h.f_id=d.f_header "
                      "where h.f_state=:f_state1 and d.f_state=:f_state2 "
                      "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch ", fDbBind);
    double total = drsal.value("f_total").toDouble();
    fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
    fDbBind[":f_state2"] = DISH_STATE_READY;
    fDbBind[":f_dateCash"] =  ui->deDate->date();
    fDbBind[":f_store"] = storealias(2);
    fDbBind[":f_branch"] = defrest(dr_branch).toInt();
//    drsal.select(fDb,
//                 QString("select sum(d.f_qty)*1000 as f_deduction, sum(d.f_totalUSD) as f_total from o_dish d "
//                 "left join o_header h on h.f_id=d.f_header "
//                 "where h.f_state=:f_state1 and d.f_state=:f_state2 "
//                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch "
//                "and d.f_dish not in (%1)").arg("159,171,158,169,153,165,386,387,388,389,390,391"), fDbBind);
    drsal.select(fDb,
                 QString("select sum(d.f_qty)*1000 as f_deduction, sum(d.f_totalUSD) as f_total from o_dish d "
            "left join o_header h on h.f_id=d.f_header "
            "where h.f_state=:f_state1 and d.f_state=:f_state2 "
            "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch "), fDbBind);
    total -= drsal.value("f_deduction").toDouble();

    if(total > 0.1) {
        fDbBind[":f_date"] =  ui->deDate->date();
        fDbBind[":f_branch"] = defrest(dr_branch).toInt();
        fDbBind[":f_docType"] = 3;
        fDbBind[":f_debit"] = 1;
        fDbBind[":f_credit"] = 2;
        fDbBind[":f_amount"] = total * -1 * 0.4;
        fDbBind[":f_comment"] = tr("Wash");
        fDb.insert("c_cash", fDbBind);
    }

    for(int i = 0; i < ui->tblData->rowCount(); i++) {
        if(ui->tblData->toDouble(i, 3) > 0.01) {
            fDbBind[":f_date"] =  ui->deDate->date();
            fDbBind[":f_branch"] = defrest(dr_branch).toInt();
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

    if(rows.count() == 0) {
        return;
    }

    float num = 0;

    if(!RNumbers::getFloat(num, 1000000, "ԳՈՒՄԱՐ", this)) {
        return;
    }

    ui->tblData->setItemWithValue(rows.at(0).row(), 3, num);
}

void DlgSalary::on_deDate_dateChanged(const QDate &date)
{
    Q_UNUSED(date)

    if(fDay) {
        loadSalary2();
    } else {
        loadSalary();
    }
}

void DlgSalary::on_btnCopyFromReal_clicked()
{
    loadSalary();

    for(int i = 0; i < ui->tblData->rowCount(); i++) {
        ui->tblData->setItemWithValue(i, 0, 0);
    }
}

void DlgSalary::on_btnPrint_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db;
    db.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db[":f_id"] = defrest(dr_branch);
    db.exec("select f_name from r_branch where f_id=:f_id");
    db.next();
    QString branchname = db.string("f_name");
    C5Printing p;
    p.setSceneParams(2000, 2700, QPrinter::Portrait);
    QFont font(font());
    int fontbase = 26;
    font.setPointSize(fontbase);
    font.setBold(true);
    p.setFont(font);
    p.setFontSize(fontbase + 4);
    p.setFontBold(true);
    p.ctext(tr("<<ԷԼԻՏ>> ԱՎՏՈԼՎԱՑՈՒՄ"));
    p.br();
    p.setFontSize(fontbase);
    p.setFontBold(false);
    p.ctext(tr("ԱՇԽԱՏԱՎԱՐՁԻ ՎՃԱՐՄԱՆ ՓԱՍԹԱԹՈՒՂԹ"));
    p.br();
    p.ctext(branchname);
    p.setFontBold(false);
    p.br();
    p.ctext(ui->deDate->text());
    p.br();
    p.br();
    QList<qreal> points;
    points << 10 << 100 << 300 << 500 << 250 << 350;
    QStringList vals;
    vals << tr("NN") << tr("ՀԱՍՏԻԿ") << tr("ԱՆՈՒՆ") << tr("ԳՈՒՄԱՐ") <<  tr("ՍՏՈՐԱԳՐՈՒԹՅՈՒՆ");
    p.tableText(points, vals, p.fLineHeight + 30);
    p.br(p.fLineHeight + 30);
    db[":f_date"] = ui->deDate->date();
    db[":f_branch"] = defrest(dr_branch);
    db.exec("select ug.f_en as f_groupname, concat_ws(' ', u.f_lastname, u.f_firstname) as f_employeename, s.f_amount "
            "from salary2 s "
            "left join users u on u.f_id=s.f_employee "
            "left join users_groups ug on ug.f_id=u.f_group "
            "where s.f_branch=:f_branch and s.f_date=:f_date");
    double total = 0;
    int row = 1;

    while(db.next()) {
        total += db.doubleValue("f_amount");
        vals.clear();
        vals << QString::number(row++)
             << db.string("f_groupname")
             << db.string("f_employeename")
             << float_str(db.doubleValue("f_amount"), 2)
             << "";
        p.tableText(points, vals, p.fLineHeight + 30);
        p.br(p.fLineHeight + 30);
    }

    points.clear();
    points << 910 << 250 << 350;
    vals.clear();
    vals << tr("ԸՆԴԱՄԵՆԸ") << float_str(total, 2);
    p.tableText(points, vals, p.fLineHeight + 30);
#ifdef QT_DEBUG
    p.print("local", QPrinter::A4);
#else
    p.print("salary", QPrinter::A4);
#endif
}

void DlgSalary::on_btnTotal_clicked()
{
    QString pwd;

    if(RNumbers::getPassword(pwd, "ՄԵՆԵՋԵՐԻ ԳԱԽՏՆԱԲԱՌ", this) == false) {
        return;
    }

    if(pwd != "1981") {
        return;
    }

    DlgSalaryTotal(this).exec();
}
