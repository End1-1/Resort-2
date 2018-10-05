#include "dlgmain.h"
#include <QApplication>

//CREATEPROCESS_MANIFEST_RESOURCE_ID RT_MANIFEST "updater.exe.manifest"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DlgMain w;
    w.show();
    qApp->processEvents();
    if (autoStart) {
        w.on_btnUpdate_clicked();
        return 0;
    }
    return a.exec();
}
