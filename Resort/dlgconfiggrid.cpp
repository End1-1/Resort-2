#include "dlgconfiggrid.h"
#include "ui_dlgconfiggrid.h"
#include "wreportgrid.h"
#include "gowidget.h"

DlgConfigGrid::DlgConfigGrid(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgConfigGrid)
{
    ui->setupUi(this);
    fAddWidget = 0;
    if (parent) {
        WReportGrid *rg = dynamic_cast<WReportGrid*>(parent);
        if (rg) {
            QWidget *gow = rg->gridOptionWidget();
            if (gow) {
                fAddWidget = static_cast<GOWidget*>(gow);
                QVBoxLayout *l = new QVBoxLayout();
                l->addWidget(fAddWidget);
                ui->wdtAdd->setLayout(l);
            }
        }
    }
    adjustSize();
}

DlgConfigGrid::~DlgConfigGrid()
{
    delete ui;
}

bool DlgConfigGrid::config(QString &fontName, int &fontSize, bool &fontBold, QWidget *parent)
{
    DlgConfigGrid *d = new DlgConfigGrid(parent);
    d->ui->fcbFontName->setCurrentText(fontName);
    d->ui->spFontSize->setValue(fontSize);
    d->ui->chFontBold->setChecked(fontBold);
    bool result = d->exec() == QDialog::Accepted;
    if (result) {
        fontName = d->ui->fcbFontName->currentText();
        fontSize = d->ui->spFontSize->value();
        fontBold = d->ui->chFontBold->isChecked();
    }
    delete d;
    return result;
}

void DlgConfigGrid::on_btnOK_clicked()
{
    if (fAddWidget) {
        fAddWidget->save();
    }
    accept();
}

void DlgConfigGrid::on_btnCancel_clicked()
{
    reject();
}
