#include "reresttable.h"
#include "ui_reresttable.h"

RERestTable::RERestTable(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_REST_TABLE, parent),
    ui(new Ui::RERestTable)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    addWidget(ui->leHallCode, "");
    addWidget(ui->leHallName, "Hall");
    addWidget(ui->leQueue, "Queue");
    fTable = "r_table";
    fDockHall = new DWSelectorHall(this);
    fDockHall->configure();
    fDockHall->setSelector(ui->leHallCode);
    connect(fDockHall, SIGNAL(hall(CI_RestHall*)), this, SLOT(hall(CI_RestHall*)));
}

RERestTable::~RERestTable()
{
    delete ui;
}

bool RERestTable::isDataCorrect()
{
    if (ui->leHallCode->asInt() == 0) {
        message_error_tr("Hall is not defined for this table");
        return false;
    }
    return true;
}

void RERestTable::hall(CI_RestHall *h)
{
    if (!h) {
        if (ui->leHallCode->asInt() == 0) {
            ui->leHallCode->clear();
            ui->leHallName->clear();
            return;
        } else {
            h = CacheRestHall::instance()->get(ui->leHallCode->text());
            if (!h) {
                ui->leHallCode->clear();
                ui->leHallName->clear();
                return;
            }
        }
    }
    ui->leHallCode->setText(h->fCode);
    ui->leHallName->setText(h->fName);
}
void RERestTable::on_btnCancel_clicked()
{
    reject();
}

void RERestTable::on_btnOk_clicked()
{
    if (ui->leQueue->asInt() == 0) {
        DatabaseResult dr;
        dr.select(fDb, "select max(f_queue) + 1 as q from r_table", fDbBind);
        ui->leQueue->setInt(dr.value("q").toInt());
    }
    save();
}
