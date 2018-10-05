#include "dlghelp.h"
#include "ui_dlghelp.h"
#include <QFile>

DlgHelp::DlgHelp(const QString &helpFile, QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgHelp)
{
    ui->setupUi(this);
    QFile f("help/" + helpFile + ".html");
    if (f.open(QIODevice::ReadOnly)) {
        ui->teHelp->setHtml(f.readAll());
    }
}

DlgHelp::~DlgHelp()
{
    delete ui;
}

void DlgHelp::openHelp(const QString &helpFile)
{
    DlgHelp *d = new DlgHelp(helpFile, DlgHelp::fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}
