#include "reresthall.h"
#include "ui_reresthall.h"
#include "cacheresthall.h"
#include "defrest.h"
#include "databaseresult.h"
#include <QHostInfo>

RERestHall::RERestHall(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_REST_HALL, parent),
    ui(new Ui::RERestHall)
{
    ui->setupUi(this);
    ui->leItemId->setValidator(new QIntValidator());
    ui->leService->setValidator(new QDoubleValidator(0, 100, 2));
    addWidget(ui->leCode, "Code");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leMenuCode, "");
    addWidget(ui->leMenuName, "Menu");
    addWidget(ui->leService, "Service");
    addWidget(ui->leItemId, "Item for invoice");
    addWidget(ui->leReceiptPrinter, "Receipt printer");
    addWidget(ui->leVATDept, "VAT dept");
    addWidget(ui->leNoVatDept, "No VAT dept");
    addWidget(ui->chShowBanket, "Show in banket");
    addWidget(ui->chShowHall, "Show in hall");
    addWidget(ui->leServiceItem, "Service item");
    addWidget(ui->leServiceValue, "Service value");
    addWidget(ui->lePrefix, "Order prefix");
    fTable = "r_hall";
    fCacheId = cid_rest_hall;
    fDockMenu = new DWSelectorRestMenu(this);
    fDockMenu->configure();
    fDockMenu->setSelector(ui->leMenuCode);
    connect(fDockMenu, SIGNAL(menu(CI_RestMenu*)), this, SLOT(menu(CI_RestMenu*)));
}

RERestHall::~RERestHall()
{
    delete ui;
}

void RERestHall::setValues()
{
    RowEditorDialog::setValues();
    if (isNew) {
        ui->chShowHall->setChecked(true);
    }
}

void RERestHall::prepareDbBind(EQLineEdit *id)
{
    if (id->asInt() == 0 && !fDbBind.contains(":f_branch")) {
        QMap<QString, QVariant> bind;
        bind[":f_comp"] = QHostInfo::localHostName();
        bind[":f_key"] = dr_branch;
        DatabaseResult dr;
        dr.select(fDb,
                  "select f_value from r_config where upper(f_comp)=upper(:f_comp) and f_key=:f_key",
                  bind);
        if (dr.rowCount() > 0) {
            const int branch = dr.value(0, "f_value").toInt();
            if (branch > 0) {
                fDbBind[":f_branch"] = branch;
            }
        }
    }
}

bool RERestHall::isDataCorrect()
{
    if (ui->leMenuCode->asInt() == 0) {
        message_error(tr("Menu for hall is not defined"));
        return false;
    }
    return true;
}

void RERestHall::menu(CI_RestMenu *m)
{
    dockResponse<CI_RestMenu, CacheRestMenu>(ui->leMenuCode, ui->leMenuName, m);
}
void RERestHall::on_btnCancel_clicked()
{
    reject();
}

void RERestHall::on_btnOk_clicked()
{
    save();
}
