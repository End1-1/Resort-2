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

    QStringList params;
    for (int i = 0; i < argc; i++) {
        params << argv[i];
    }

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
        QMessageBox::warning(nullptr, "Stylesheet", "Missing stylesheet\r\n" + styleSheet.fileName());
    }
    styleSheet.open(QIODevice::ReadOnly);
    a.setStyleSheet(styleSheet.readAll());
    QFont f("Tahoma", 12);
    a.setFont(f);
    a.setWindowIcon(QIcon(":/images/app.ico"));

//    QFont font("Arial LatArm Unicode", 12);
//    a.setFont(font);

    RFace *w = new RFace();
    if (!w->fIsConfigured) {
        return -1;
    }
    if (!w->setup()) {
        return -1;
    }

    User *u = nullptr;
    DefRest der(QHostInfo::localHostName().toUpper());

        QString login;
        if (RLogin::getLogin(login, QObject::tr("Login"), nullptr)) {
            u = new User(login, 0);
            if (!u->isValid()) {
                delete u;
                u = 0;
                RMessage::showError(QObject::tr("Access denied"), nullptr);
                return 0;
            }
        } else {
            return 0;
        }

        TableStruct *ts = nullptr;
        RDesk *rd = new RDesk(nullptr);
        rd->prepareToShow();
        rd->setStaff(u);
        ts = rd->loadHall(1);
        if (rd->setup(ts)) {
            rd->exec();
            return 0;
        }



    return a.exec();
}
