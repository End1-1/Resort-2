#include "rface.h"
#include "preferences.h"
#include "logging.h"
#include "cacheone.h"
#include "dlgconnecttoserver.h"
#include <QApplication>
#include <QLockFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    def_station = "Restaurant: ";
    logEnabled = true;
    QApplication a(argc, argv);
    Preferences p;
    p.initFromConfig();
    DlgConnectToServer *d = new DlgConnectToServer();
    if (d->exec() != QDialog::Accepted) {
        return -1;
    }
    delete d;

    QFile file(p.getString(def_home_path) + "/lock.pid");
    file.remove();
    QLockFile lockFile(p.getString(def_home_path) + "/lock.pid");
    if (!lockFile.tryLock())
        return -1;

    Base::fDbName = "Main";
    p.initFromDb(Base::fDbName, HOSTNAME, 0);
    CacheOne::setDatabase(p.getDatabase(Base::fDbName));


    QFile styleSheet(":/files/stylesheet.qss");
    if (!styleSheet.exists()) {
        QMessageBox::warning(0, "Stylesheet", "Missing stylesheet\r\n" + styleSheet.fileName());
    }
    styleSheet.open(QIODevice::ReadOnly);
    a.setStyleSheet(styleSheet.readAll());
    QFont f("Times", 12);
    a.setFont(f);
    a.setWindowIcon(QIcon(":/images/app.ico"));

    RFace w;
    if (!w.fIsConfigured) {
        return -1;
    }
    w.showFullScreen();
    qApp->processEvents();
    if (!w.setup()) {
        return -1;
    }

    return a.exec();
}
