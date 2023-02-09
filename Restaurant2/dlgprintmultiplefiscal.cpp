#include "dlgprintmultiplefiscal.h"
#include "ui_dlgprintmultiplefiscal.h"
#include "database2.h"
#include "defrest.h"
#include "message.h"
#include "printtaxn.h"

DlgPrintMultipleFiscal::DlgPrintMultipleFiscal(QWidget *parent) :
    BaseDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::DlgPrintMultipleFiscal)
{
    ui->setupUi(this);
    showFullScreen();
    hide();
    ui->btnCard->setStyleSheet("QPushButton:checked {background-color:green};");
    ui->btnCash->setStyleSheet("QPushButton:checked {background-color:green};");
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db;
    db.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db[":f_branch"] = defrest(dr_branch).toInt();
    db[":f_state"] = ORDER_STATE_OPENED;
    db.exec("select o.f_id, h.f_name as f_hallname, t.f_name as f_tablename, c.f_govnumber, "
            "o.f_total, o.f_tax "
            "from o_header o "
            "left join r_table t on t.f_id=o.f_table "
            "left join r_hall h on h.f_id=t.f_hall "
            "left join o_car c on c.f_order=o.f_id "
            "where o.f_branch=:f_branch and o.f_state=:f_state and o.f_tax=0 "
            "order by 1, 2");
    while (db.next()) {
        int r = ui->tbl->rowCount();
        ui->tbl->setRowCount(r + 1);
        for (int i = 0; i < db.columnCount(); i++) {
            ui->tbl->setItem(r, i, new QTableWidgetItem(db.string(db.columnName(i))));
        }
    }
    ui->tbl->resizeColumnsToContents();

    QSettings s(QString("%1\\fiscal.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);
    QStringList groups = s.childGroups();
    for (const QString &g: qAsConst(groups)) {
        ui->cbFiscalMachine->addItem(g);
        s.beginGroup(g);
        QStringList keys = s.childKeys();
        for (const QString &k: keys) {
            fFiscalMachines[g][k] = s.value(k);
        }
        s.endGroup();
    }
    ui->cbFiscalMachine->setCurrentIndex(-1);
    QString defFiscal = __s.value("multitaxfiscal").toString();
    if (ui->cbFiscalMachine->count() == 1) {
        ui->cbFiscalMachine->setCurrentIndex(0);
    }
    if (ui->cbFiscalMachine->currentIndex() == -1 && defFiscal.isEmpty()) {
        ui->cbFiscalMachine->setCurrentIndex(ui->cbFiscalMachine->findText(defFiscal));
    }
}

DlgPrintMultipleFiscal::~DlgPrintMultipleFiscal()
{
    delete ui;
}

void DlgPrintMultipleFiscal::on_btnCancel_clicked()
{
    reject();
}

void DlgPrintMultipleFiscal::on_btnPrint_clicked()
{
    QModelIndexList ml = ui->tbl->selectionModel()->selectedRows();
    for (const QModelIndex &mi: qAsConst(ml)) {
        if (ui->tbl->item(mi.row(), 5)->text().toInt() > 0) {
            continue;
        }
        int order = ui->tbl->item(mi.row(), 0)->text().toInt();
        Db b = Preferences().getDatabase(Base::fDbName);
        Database2 db;
        db.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
        db[":f_header"] = order;
        QMap<QString, QVariant> s = fFiscalMachines[ui->cbFiscalMachine->currentText()];
        PrintTaxN pn(s.value("ip").toString(),
                     s.value("port").toInt(),
                     s.value("password").toString(),
                     s.value("extpos").toString(),
                     s.value("opcode").toString(),
                     s.value("oppin").toString());

        db.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
        db[":f_header"] = order;
        db[":f_state"] = DISH_STATE_READY;
        db.exec("select d.f_en, d.f_adgt, od.f_qty, od.f_price, od.f_dctvalue, d.f_taxdebt, d.f_id "
                "from o_dish od "
                "left join r_dish d on d.f_id=od.f_dish "
                "where od.f_header=:f_header and od.f_state=:f_state and od.f_total>0 ");
        while (db.next()) {
            pn.addGoods(db.string("f_taxdebt"),
                        db.string("f_adgt"),
                        db.string("f_id"),
                        db.string("f_name"),
                        db.doubleValue("f_qty"),
                        db.doubleValue("f_price"),
                        db.doubleValue("f_dctvalue"));
        }

        QString in, out, err;
        int fiscalrecid;
        int result = pn.makeJsonAndPrint(ui->btnCash->isChecked() ? 0 : ui->tbl->item(mi.row(), 4)->text().toDouble(),
                                         0, in, out, err);
        db[":f_order"] = order;
        db[":f_in"] = in;
        db[":f_out"] = out;
        db[":f_err"] = err;
        db.insert("o_tax_log", fiscalrecid);
        if (result != pt_err_ok) {
            message_error(tr("Fiscal error.") + "\r\n" + err);
            accept();
            return;
        }
        QString sn, firm, address, fiscal, hvhh, rseq, devnum, time;
        pn.parseResponse(out, firm, hvhh, fiscal, rseq, sn, address, devnum, time);
        db[":f_fiscal"] = rseq.toInt();
        db.update("o_tax_log", "f_id", fiscalrecid);
        db[":f_tax"] = rseq.toInt();
        db.update("o_header", "f_id", order);
    }
    accept();
}

void DlgPrintMultipleFiscal::on_btnCard_clicked(bool checked)
{
    ui->btnCash->setChecked(!checked);
}

void DlgPrintMultipleFiscal::on_btnCash_clicked(bool checked)
{
    ui->btnCard->setChecked(!checked);
}

void DlgPrintMultipleFiscal::on_cbFiscalMachine_currentTextChanged(const QString &arg1)
{
    __s.setValue("multitaxfiscal", arg1);
}
