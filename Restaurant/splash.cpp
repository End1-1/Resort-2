#include "splash.h"
#include "ui_splash.h"
#include <QMovie>

Splash::Splash(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::Splash)
{
    ui->setupUi(this);
}

Splash::~Splash()
{
    delete ui;
}

void Splash::setText(const QString &text)
{
    ui->lbMovie->setText(text);
    qApp->processEvents();
}
