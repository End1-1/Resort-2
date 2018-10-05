#include "dlgtaxprintsetup.h"
#include "ui_dlgtaxprintsetup.h"

DlgTaxPrintSetup::DlgTaxPrintSetup(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgTaxPrintSetup)
{
    ui->setupUi(this);
    ui->lePort->setValidator(new QIntValidator());
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    fDb.select("select f_address, f_port, f_password, f_adg, f_hall, f_menu from s_tax_print where f_comp=:f_comp", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leAddress->setText(fDbRows.at(0).at(0).toString());
        ui->lePort->setText(fDbRows.at(0).at(1).toString());
        ui->lePassword->setText(fDbRows.at(0).at(2).toString());
        ui->leAdg->setText(fDbRows.at(0).at(3).toString());
        ui->leHallCode->setText(fDbRows.at(0).at(4).toString());
        ui->leMenuCode->setText(fDbRows.at(0).at(5).toString());
    }
    CI_RestHall *ch = CacheRestHall::instance()->get(ui->leHallCode->text());
    hall(ch);
    CI_RestMenu *cm = CacheRestMenu::instance()->get(ui->leMenuCode->text());
    menu(cm);
    fDockHall = new DWSelectorHall(this);
    fDockHall->configure();
    fDockHall->setSelector(ui->leHallCode);
    connect(fDockHall, SIGNAL(hall(CI_RestHall*)), this, SLOT(hall(CI_RestHall*)));
    fDockMenu = new DWSelectorRestMenu(this);
    fDockMenu->configure();
    fDockMenu->setSelector(ui->leMenuCode);
    connect(fDockMenu, SIGNAL(menu(CI_RestMenu*)), this, SLOT(menu(CI_RestMenu*)));
}

DlgTaxPrintSetup::~DlgTaxPrintSetup()
{
    delete ui;
}

void DlgTaxPrintSetup::hall(CI_RestHall *c)
{
    dockResponse<CI_RestHall, CacheRestHall>(ui->leHallCode, ui->leHallName, c);
}

void DlgTaxPrintSetup::menu(CI_RestMenu *c)
{
    dockResponse<CI_RestMenu, CacheRestMenu>(ui->leMenuCode, ui->leMenuName, c);
}

void DlgTaxPrintSetup::on_btnCancel_clicked()
{
    reject();
}

void DlgTaxPrintSetup::on_btnOk_clicked()
{
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    fDb.select("delete from s_tax_print where f_comp=:f_comp", fDbBind, fDbRows);
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    fDbBind[":f_address"] = ui->leAddress->text();
    fDbBind[":f_port"] = ui->lePort->asInt();
    fDbBind[":f_password"] = ui->lePassword->text();
    fDbBind[":f_adg"] = ui->leAdg->text();
    fDbBind[":f_hall"] = ui->leHallCode->asInt();
    fDbBind[":f_menu"] = ui->leMenuCode->asInt();
    fDb.insert("s_tax_print", fDbBind);
    fPreferences.setDb(def_tax_address, ui->leAddress->text());
    fPreferences.setDb(def_tax_port, ui->lePort->text());
    fPreferences.setDb(def_tax_password, ui->lePassword->text());
    accept();
}
