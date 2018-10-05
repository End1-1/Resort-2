#include "rchangelanguage.h"
#include "ui_rchangelanguage.h"

RChangeLanguage::RChangeLanguage(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RChangeLanguage)
{
    ui->setupUi(this);
    QListWidgetItem *am = new QListWidgetItem(ui->lstCountry);
    am->setSizeHint(QSize(0, 60));
    am->setIcon(QIcon(":/images/armenia.png"));
    am->setText(QString::fromUtf8("Հայերեն"));
    QListWidgetItem *en = new QListWidgetItem(ui->lstCountry);
    en->setSizeHint(QSize(0, 60));
    en->setIcon(QIcon(":/images/usa.png"));
    en->setText("English");
    QListWidgetItem *ru = new QListWidgetItem(ui->lstCountry);
    ru->setSizeHint(QSize(0, 60));
    ru->setIcon(QIcon(":/images/russia.png"));
    ru->setText(QString::fromUtf8("Русский"));
    ui->lstCountry->setIconSize(QSize(32, 32));
    ui->lstCountry->addItem(am);
    ui->lstCountry->addItem(en);
    ui->lstCountry->addItem(ru);
}

RChangeLanguage::~RChangeLanguage()
{
    delete ui;
}

bool RChangeLanguage::changeLanguage(QWidget *parent)
{
    bool result = false;
    RChangeLanguage *l = new RChangeLanguage(parent);
    if (l->exec() == QDialog::Accepted) {
        result = true;
    }
    delete l;
    return result;
}

void RChangeLanguage::on_btnCancel_clicked()
{
    reject();
}

void RChangeLanguage::on_btnOk_clicked()
{
    switch (ui->lstCountry->currentRow()) {
    case 0:
        def_lang = "am";
        break;
    case 1:
        def_lang = "en";
        break;
    case 2:
        def_lang = "ru";
        break;
    default:
        reject();
        return;
    }
    accept();
}
