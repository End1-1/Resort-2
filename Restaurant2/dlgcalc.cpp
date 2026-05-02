#include "dlgcalc.h"
#include "base.h"
#include "database2.h"
#include "preferences.h"
#include "ui_dlgcalc.h"
#include "utils.h"

DlgCalc::DlgCalc(int dish, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgCalc)
{
    ui->setupUi(this);
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    if (db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        db2[":f_dish"] = dish;
        db2.exec(R"(
    SELECT rp.f_en, r.f_qty
    FROM r_recipe r
    LEFT JOIN r_dish rp ON rp.f_id=r.f_part
    WHERE r.f_dish=:f_dish
)");
        ui->tbl->setRowCount(db2.rowCount());
        int row = 0;
        while (db2.next()) {
            ui->tbl->setItem(row, 0, new QTableWidgetItem(db2.string("f_en")));
            ui->tbl->setItem(row, 1, new QTableWidgetItem(float_str(db2.doubleValue("f_qty"), 2)));
            row++;
        }
    }
    ui->tbl->setColumnWidth(0, 400);
    ui->tbl->setColumnWidth(1, 100);
}

DlgCalc::~DlgCalc()
{
    delete ui;
}

void DlgCalc::on_btnClose_clicked()
{
    accept();
}
