#include "dlgconnecttoserver.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include "rmessage.h"
#include "ui_dlgconnecttoserver.h"

DlgConnectToServer::DlgConnectToServer(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgConnectToServer)
{
    ui->setupUi(this);
    loadFromConfig();
}

DlgConnectToServer::~DlgConnectToServer()
{
    delete ui;
}

void DlgConnectToServer::loadFromConfig()
{
    QString path = QCoreApplication::applicationDirPath() + "/cnf.inf";

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        message_error("cnf.inf not found");
        reject();
        return;
    }

    QMap<QString, QString> cfg;

    QTextStream s(&f);
    while (!s.atEnd()) {
        QString line = s.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#"))
            continue;

        QStringList parts = line.split("=");
        if (parts.size() == 2)
            cfg[parts[0].trimmed()] = parts[1].trimmed();
    }

    Database db;
    db.setConnectionParams(cfg["host"], cfg["database"], cfg["username"], cfg["password"]);

    if (!db.open()) {
        message_error("Cannot connect database<br>" + db.fLastError);
        reject();
        return;
    }

    fPreferences.appendDatabase("Main",
                                cfg["host"],
                                cfg["database"],
                                cfg["username"],
                                cfg["password"],
                                "",
                                "",
                                "",
                                "");

    accept();
}
