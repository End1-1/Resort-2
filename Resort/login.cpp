#include "login.h"
#include "ui_login.h"
#include "loginsettings.h"
#include "databasesconnections.h"
#include "cacherights.h"
#include "databaseresult.h"
#include "appconfig.h"
#include "ecomboboxcompleter.h"
#include <QProcess>
#include <windows.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QShortcut>
#include <QInputDialog>

Login::Login(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);    
    connect(ui->cbDatabase, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {fPreferences.set(def_last_db_combo_index, index);});
    ui->leUsername->setText(__s.value("_last_login_user_name").toString());
    if (ui->leUsername->text().length() > 0) {
        ui->lePassword->setFocus();
    }
    fLockUser = 0;
    QShortcut *shortcut = new QShortcut(QKeySequence("F1"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(shortcutSlot()));
    QFile f("./logo_login.png");
    if (f.exists()) {
        QPixmap p("./logo_login.png");
        ui->lbLogo->setPixmap(p);
    }
    if (__s.value("db_direct_connection").toBool()) {
        getDatabases();
    } else {
        connect(&fUdpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
        timeout();
        connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
        fTimer.start(2000);
    }
    ui->lbLogo->setPixmap(QPixmap(qApp->applicationDirPath() + "/bg.jpg"));
}

Login::~Login()
{
    delete ui;
}

void Login::setLockUser(int id)
{
    fLockUser = id;
    ui->cbDatabase->setEnabled(false);
}

void Login::shortcutSlot()
{
    if (ui->cbDatabase->count() > 0) {
        return;
    }
    QString password = QInputDialog::getText(this, tr("First setup"), tr("MySQL server password"), QLineEdit::Password);
    if (password.isEmpty()) {
        return;
    }
    QJsonObject jObj;
    jObj["first_setup"] = password;
    jObj["client"] = HOSTNAME + "." + fPreferences.hostUsername();
    QJsonDocument jDoc(jObj);
    QByteArray datagram = jDoc.toJson();
    fUdpSocket.writeDatagram(datagram, QHostAddress::Broadcast, DATAGRAM_PORT);
}

void Login::readDatagram()
{
    fTimer.stop();
    while (fUdpSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(fUdpSocket.pendingDatagramSize());
        fUdpSocket.readDatagram(datagram.data(), datagram.size());
        QJsonDocument jDoc = QJsonDocument::fromJson(datagram);
        QJsonObject jObj = jDoc.object();
        if (jObj.contains("server")) {
            QString rep = jObj.value("server").toString();
            if (rep == "me") {
                Database db;
                db.setConnectionParams(jObj.value("host").toString(),
                                       jObj.value("database").toString(),
                                       jObj.value("username").toString(),
                                       jObj.value("password").toString());
                if (!db.open()) {
                    message_error(tr("Cannot connect to main database")
                                  + "<br>"
                                  + db.fLastError
                                  + "<br>" + db.fDb.hostName() + ":" + db.fDb.databaseName());
                    return;
                }

                if (!__s.value("db_direct_connection").toBool()) {
                    AppConfig::fServerAddress =  jObj.value("host").toString();
                    Base::fAirHost = jObj.value("host").toString();
                    Base::fAirDbName = jObj.value("database").toString();
                    Base::fAirUser = jObj.value("username").toString();
                    Base::fAirPass = jObj.value("password").toString();

                }
                TrackControl::fDbHost = jObj["loghost"].toString();
                TrackControl::fDbDb = jObj["logdb"].toString();
                TrackControl::fDbUser = jObj["loguser"].toString();
                TrackControl::fDbPass = jObj["logpass"].toString();
                _IDGENMODE_ = jObj["idgen"].toInt();
                DatabaseResult dr;
                fDbBind[":f_user"] = HOSTNAME + "." + fPreferences.hostUsername();
                dr.select(db, "select * from f_db where f_id in "
                          "(select f_db from f_access where lower(f_user)=lower(:f_user) and f_right=1)", fDbBind);
                for (int i = 0; i < dr.rowCount(); i++) {
                    QMap<QString, QString> conn;
                    conn["f_host"] = dr.value(i, "f_host").toString();
                    conn["f_database"] = dr.value(i, "f_database").toString();
                    conn["f_username"] = dr.value(i, "f_username").toString();
                    conn["f_password"] = dr.value(i, "f_password").toString();
                    fDatabaseList[dr.value(i, "f_name").toString()] = conn;
                }
                getDatabases();
                return;
            }
        } else if (jObj.contains("first_setup")) {
            QString reply = jObj["first_setup"].toString();
            if (reply != "ok") {
                message_error_tr("First setup failed");
                return;
            }
            timeout();
        }
    }
    fTimer.start(2000);
}

void Login::on_btnCancel_clicked()
{
    reject();
}

void Login::on_btnAddDatabase_clicked()
{
    if (fPreferences.getString(def_preferences_password).length() > 0) {
        LoginSettings ls(this);
        if (ls.exec() != QDialog::Accepted) {
            return;
        }
    }
    DatabasesConnections dc(this);
    dc.exec();
}

void Login::getDatabases()
{
    ui->cbDatabase->clear();
    if (!__s.value("db_direct_connection").toBool()) {
        QStringList dbNames = fDatabaseList.keys();
        fPreferences.clearDatabase();
        foreach (QString s, dbNames) {
            fPreferences.appendDatabase(s,
                    fDatabaseList[s]["f_host"],
                    fDatabaseList[s]["f_database"],
                    fDatabaseList[s]["f_username"],
                    fDatabaseList[s]["f_password"],
                    "", "", "", "");
            }
            ui->cbDatabase->addItems(dbNames);
    } else {
        QStringList dbNames;
        fPreferences.setDatabasesNames(dbNames);
        for (QStringList::const_iterator it = dbNames.begin(); it != dbNames.end(); it++) {
            Db db = fPreferences.getDatabase(*it);
            fPreferences.appendDatabase(db.dc_name,
                    db.dc_main_host,
                    db.dc_main_path,
                    db.dc_main_user,
                    db.dc_main_pass,
                    "", "", "", "");
            ui->cbDatabase->addItem(db.dc_name);
            AppConfig::fServerAddress = db.dc_main_host;
            Base::fAirHost = db.dc_main_host;
            Base::fAirDbName = "airwick";
            Base::fAirUser = db.dc_main_user;
            Base::fAirPass = db.dc_main_pass;
        }
    }
    ui->cbDatabase->setCurrentIndex(ui->cbDatabase->findText(__s.value("_last_login_db_name").toString()));
    on_leUsername_textChanged(ui->leUsername->text());
}

void Login::on_btnLogin_clicked()
{
    QString connName = ui->cbDatabase->currentText();
    Db conn = fPreferences.getDatabase(connName);
    fDb.setConnectionParams(conn.dc_main_host, conn.dc_main_path, conn.dc_main_user, conn.dc_main_pass);
    if (!fDb.open()) {
        message_error(fDb.fLastError);
        return;
    }
    fDbBind[":username"] = ui->leUsername->text();
    fDbBind[":password"] = ui->lePassword->text();
    fDb.select("select f_id, f_firstName, f_lastName, f_group from users where length(f_username)>0 and f_username=:username and f_password=md5(:password)", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Access denied");
        return;
    }
    if (fLockUser > 0) {
        if (fDbRows.at(0).at(0).toInt() != fLockUser) {
            message_error_tr("You cannot unlock this station");
            return;
        }
    }
    __s.setValue("_last_login_user_name", ui->leUsername->text());
    __s.setValue("_last_login_db_name", ui->cbDatabase->currentText());
    fDbName = connName;
    DatabaseResult::fMainDbConnection = connName;
    EComboBoxCompleter::setDefaultDatabaseName(connName);
    fPreferences.initFromDb(connName, "", fDbRows.at(0).at(0).toInt());
    fPreferences.setLocal(def_working_conn_name, connName);
    fPreferences.setLocal(def_working_user_id, fDbRows.at(0).at(0).toInt());
    fPreferences.setLocal(def_working_username, fDbRows.at(0).at(1).toString() + " " + fDbRows.at(0).at(2).toString());
    fPreferences.setLocal(def_working_user_group, fDbRows.at(0).at(3).toInt());
    //setup new double database connection
    Db d = fPreferences.getDatabase(connName);
    __dd1Host = d.dc_main_host;
    __dd1Database = d.dc_main_path;
    __dd1Username = d.dc_main_user;
    __dd1Password = d.dc_main_pass;

    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() == 4) {
        __dd2Host = dbParams[0];
        __dd2Database = dbParams[1];
        __dd2Username = dbParams[2];
        __dd2Password = dbParams[3];
    }

    // setup tax parameters
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    fDb.select("select f_address, f_port, f_password, f_adg from s_tax_print where f_comp=:f_comp", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        if (fDbRows.count() > 0) {
            fPreferences.setDb(def_tax_address, fDbRows.at(0).at(0).toString());
            fPreferences.setDb(def_tax_port, fDbRows.at(0).at(1).toString());
            fPreferences.setDb(def_tax_password, fDbRows.at(0).at(2).toString());
            fPreferences.setDb(def_tax_adg, fDbRows.at(0).at(3).toString());
        }
    }
    fDbBind[":f_app"] = "smarthotel";
    fDb.select("select f_version from s_app where lower(f_app)=lower(:f_app)", fDbBind, fDbRows);
    if (fDbRows.count() > 0) { /*
        if (Utils::getVersionString(qApp->applicationFilePath()) != fDbRows.at(0).at(0).toString()) {
            #ifdef Q_OS_WIN
            int result = (int)::ShellExecuteA(0, "open","updater.exe", 0, 0, SW_SHOWNORMAL);
            if (SE_ERR_ACCESSDENIED == result)
            {
            // Requesting elevation
            result = (int)::ShellExecuteA(0, "runas", "updater.exe", 0, 0, SW_SHOWNORMAL);
            }
            if (result <= 32)
            {
            // error handling
            }
            #else
            if (!QProcess::startDetached("updater.exe"))
            {
            // error handling
            }
            #endif

        }*/
    }
    fDbBind[":f_id"] = 2;
    fDb.select("select f_rate from f_acc_currencies where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        def_usd = fDbRows.at(0).at(0).toDouble();
    }
    DatabaseResult dtime;
    dtime.select(fDb, "select current_timestamp() as 'time'", fDbBind);
    qDebug() << QDateTime::currentDateTime().toTime_t() - dtime.value("time").toDateTime().toTime_t();

    DatabaseResult dr;
    fDbBind[":f_comp"] = def_station + QHostInfo::localHostName();
    dr.select(fDb, "select f_active, f_host, f_port, f_password from serv_tax where upper(f_comp)=upper(:f_comp)", fDbBind);
    if (dr.rowCount() > 0) {
        if (dr.value("f_active").toInt() > 0) {
            __TAX_HOST = dr.value("f_host").toString();
            __TAX_PORT = dr.value("f_port").toString();
            __TAX_PASS = dr.value("f_pass").toString();
        }
    } else {
        fDbBind[":f_active"] = 0;
        fDbBind[":f_comp"] = def_station + QHostInfo::localHostName();
        fDb.insert("serv_tax", fDbBind);
    }
    BaseUID::fUserId = WORKING_USERID;
    EDateEditFirstDate = WORKING_DATE;

    accept();
}

void Login::on_leUsername_textChanged(const QString &arg1)
{
    QString connName = ui->cbDatabase->currentText();
    Db conn = fPreferences.getDatabase(connName);
    fDb.setConnectionParams(conn.dc_main_host, conn.dc_main_path, conn.dc_main_user, conn.dc_main_pass);
    if (!fDb.open()) {
        return;
    }
    fDbBind[":username"] = arg1;
    fDb.select("select f_id, f_firstName, f_lastName, f_group from users where length(f_username)>0 and f_username=:username ", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->lbFullName->setText(QString("%1 %2")
                                .arg(fDbRows.at(0).at(1).toString())
                                .arg(fDbRows.at(0).at(2).toString()));
    } else {
        ui->lbFullName->clear();
    }
    fDb.close();
}

void Login::timeout()
{
    if (__s.value("db_direct_connection").toBool()) {
        getDatabases();
    } else {
        QJsonObject jObj;
        jObj["server"] = "who";
        jObj["client"] = HOSTNAME + "." + fPreferences.hostUsername();
        QJsonDocument jDoc(jObj);
        QByteArray datagram = jDoc.toJson();
        fUdpSocket.writeDatagram(datagram, QHostAddress::Broadcast, DATAGRAM_PORT);
    }
}
