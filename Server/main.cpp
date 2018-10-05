#include "dlgmain.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/server.png"));
    DlgMain w;
    w.exec();

    return a.exec();
}
