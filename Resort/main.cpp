#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QStyleFactory>
#include <QTranslator>
#include "mainwindow.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    def_station = "SmartHotel: ";
    Utils::initNumbersWords();
    QApplication a(argc, argv);
    Preferences p;
    p.initFromConfig();


    QTranslator t;
    if (t.load(":/Resort.qm")) {
        a.installTranslator(&t);
    }

    a.setStyle(QStyleFactory::create("fusion"));

    QString fsn = qApp->applicationDirPath() +  "/styles.qss";
    QFile styleFile(fsn);
    if (styleFile.exists()) {
        if (styleFile.open(QIODevice::ReadOnly)) {
            a.setStyleSheet(styleFile.readAll());
            styleFile.close();
        }
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
