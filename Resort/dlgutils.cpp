#include "dlgutils.h"
#include "ui_dlgutils.h"
#include "base.h"
#include <QFile>

DlgUtils::DlgUtils(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgUtils)
{
    ui->setupUi(this);
}

DlgUtils::~DlgUtils()
{
    delete ui;
}

void DlgUtils::on_btnGenNumbers_clicked()
{
    QFile f("./uuid.txt");
    f.open(QIODevice::WriteOnly);
    f.reset();
    for (int i = 0; i < ui->leCount->text().toInt(); i++) {
        QString uid = uuid(ui->lePrefix->text(), fAirDb);
        f.write(uid.toLatin1());
        f.write("\r\n");
    }
    f.close();
    message_info_tr("Done.");
}
