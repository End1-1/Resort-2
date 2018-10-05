#include "dlgsmile.h"
#include "ui_dlgsmile.h"
#include <QBitmap>

DlgSmile::DlgSmile(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSmile)
{
    ui->setupUi(this);
    connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    fUp = false;
    fCount = 10;
    fTimer.start(50);
}

DlgSmile::~DlgSmile()
{
    delete ui;
}

void DlgSmile::timeout()
{
    fCount--;
    fUp = !fUp;
    QPoint p = pos();
    if (fUp) {
        p.setY(p.ry() - 20);
    } else {
        p.setY(p.ry() + 40);
    }
    setGeometry(p.rx(), p.ry(), width(), height());
    if (fCount < 0) {
        accept();
    }
}
