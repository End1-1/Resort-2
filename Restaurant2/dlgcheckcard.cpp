#include "dlgcheckcard.h"
#include "ui_dlgcheckcard.h"

DlgCheckCard::DlgCheckCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCheckCard)
{
    ui->setupUi(this);
}

DlgCheckCard::~DlgCheckCard()
{
    delete ui;
}
