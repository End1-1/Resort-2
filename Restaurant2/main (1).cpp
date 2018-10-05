#include "rface.h"
#include "preferences.h"
#include "logging.h"
#include "cacheone.h"
#include "dlgconnecttoserver.h"
#include "rdesk.h"
#include "defrest.h"
#include "rlogin.h"
#include "rmessage.h"
#include <QApplication>
#include <QLockFile>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char *argv[])
{
    logEnabled = true;
    QApplication a(argc, argv);

    QTranslator t;
    t.load(":/Restaurant.am.qm");
    a.installTranslator(&t);

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

    RFace *w = new RFace();
    if (!w->fIsConfigured) {
        return -1;
    }
    if (!w->setup()) {
        return -1;
    }

    User *u = 0;
    DefRest der(QHostInfo::localHostName().toUpper());
    if (der.v(dr_open_table_after_run).toInt() != 0)
    {
        QString login;
        if (RLogin::getLogin(login, QObject::tr("Login"), 0)) {
            u = new User(login, 0);
            if (!u->isValid()) {
                delete u;
                u = 0;
                RMessage::showError(QObject::tr("Access denied"), 0);
                return 0;
            }
        } else {
            return 0;
        }

        TableStruct *t = 0;
        RDesk *rd = new RDesk(0);
        rd->prepareToShow();
        rd->setStaff(u);
        t = rd->loadHall(1);
        if (rd->setup(t)) {
            rd->exec();
            return 0;
        }

    } else {
        w->showFullScreen();
        qApp->processEvents();

    }

    return a.exec();
}
