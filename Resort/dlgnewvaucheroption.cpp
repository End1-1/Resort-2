#include "dlgnewvaucheroption.h"
#include "ui_dlgnewvaucheroption.h"

DlgNewVaucherOption::DlgNewVaucherOption(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgNewVaucherOption)
{
    ui->setupUi(this);
}

DlgNewVaucherOption::~DlgNewVaucherOption()
{
    delete ui;
}

int DlgNewVaucherOption::vaucherOption()
{
    DlgNewVaucherOption *d = new DlgNewVaucherOption(fPreferences.getDefaultParentForMessage());
    int result = d->exec();
    delete d;
    return result;
}

void DlgNewVaucherOption::on_btnCancel_clicked()
{
    done(0);
}

void DlgNewVaucherOption::on_btnRV_clicked()
{
    done(VO_RV);
}

void DlgNewVaucherOption::on_btnPostCharge_clicked()
{
    done(VO_PS);
}

void DlgNewVaucherOption::on_btnManual_clicked()
{
    done(VO_MANUAL);
}
