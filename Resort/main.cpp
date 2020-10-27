#include "mainwindow.h"
#include "login.h"
#include "utils.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFile>
#include <QStyleFactory>
#include <QTranslator>

int main(int argc, char *argv[])
{
    def_station = "SmartHotel: ";
    Utils::initNumbersWords();
    QApplication a(argc, argv);
    Preferences p;
    p.initFromConfig();


    QTranslator t;
    t.load(":/Resort.qm");
    a.installTranslator(&t);


    a.setStyle(QStyleFactory::create("fusion"));

    QString fsn = qApp->applicationDirPath() +  "/styles.qss";
    QFile styleFile(fsn);
    if (styleFile.exists()) {
        styleFile.open(QIODevice::ReadOnly);
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    a.setWindowIcon(QIcon(":/images/app.ico"));
    MainWindow w;
    p.setDefaultParentForMessage(&w);
    w.show();
    //w.show(); 778842034
    a.processEvents();
    w.login();
    a.exec();
    p.saveConfig();
    return 0;
}
