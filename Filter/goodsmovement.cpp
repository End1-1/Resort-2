#include "goodsmovement.h"
#include "ui_goodsmovement.h"
#include "dlggetidname.h"
#include "database2.h"

goodsmovement::goodsmovement(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::goodsmovement)
{
    ui->setupUi(this);
    connect(ui->leStore, &EQLineEdit::customButtonClicked, this, &goodsmovement::editStoreClicked);
}

goodsmovement::~goodsmovement()
{
    delete ui;
}

void goodsmovement::setupTab()
{
    setupTabTextAndIcon("Գույքագրում", ":/images/copy.png");
}

void goodsmovement::editStoreClicked()
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_store, this)) {
        ui->leStore->setText(name);
        ui->leStore->fHiddenText = id;
    }
}

void goodsmovement::on_btnLoad_clicked()
{
    ui->tbl->setRowCount(0);
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":date1"] = ui->date1->date();
    db2[":date2"] = ui->date2->date();
    db2[":store"] = ui->leStore->fHiddenText.toInt();
    db2.exec("call sp_goods_movement (:store, :date1, :date2);");
    db2.exec("select * from rep");
    while (db2.next()) {
        int r = ui->tbl->addRow();
        for (int i = 0; i < ui->tbl->columnCount(); i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->tbl->setItem(r, i, item);
        }
        for (int i = 0; i < db2.columnCount(); i++){
            if (i > ui->tbl->columnCount() - 1) {
                break;
            }
            ui->tbl->item(r, i)->setData(Qt::EditRole, db2.value(i));;
        }
    }
    ui->tbl->resizeColumnsToContents();
}
