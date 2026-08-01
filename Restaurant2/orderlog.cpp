#include "orderlog.h"
#include "base.h"
#include "database2.h"
#include "preferences.h"
#include "rmessage.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QThread>

const char *OrderLog::ACTION_ORDER_OPEN = "order_open";
const char *OrderLog::ACTION_DISH_ADD = "dish_add";
const char *OrderLog::ACTION_MENU_CHANGE = "menu_change";
const char *OrderLog::ACTION_DISCOUNT_OK = "discount_ok";
const char *OrderLog::ACTION_DISCOUNT_FAIL = "discount_fail";

static QThread *gLogThread = nullptr;
static OrderLogWorker *gLogWorker = nullptr;

static bool openDb(Database2 &db2)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    return db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
}

void OrderLogWorker::writeLog(int orderId, const QString &action, const QString &data)
{
    Database2 db2;

    if(!openDb(db2)) {
        emit writeFailed(QObject::tr("Cannot write order log: database error"));
        return;
    }

    db2[":f_order"] = orderId;
    db2[":f_datetime"] = QDateTime::currentDateTime();
    db2[":f_action"] = action;
    db2[":f_data"] = data;
    int id = 0;

    if(!db2.insert("o_log", id)) {
        emit writeFailed(QObject::tr("Cannot write order log: %1").arg(db2.lastDbError()));
    }
}

void OrderLog::init()
{
    ensureStarted();
}

void OrderLog::ensureStarted()
{
    if(gLogWorker) {
        return;
    }

    gLogThread = new QThread;
    gLogWorker = new OrderLogWorker;
    gLogWorker->moveToThread(gLogThread);

    QObject::connect(gLogThread, &QThread::finished, gLogWorker, &QObject::deleteLater);
    QObject::connect(gLogWorker, &OrderLogWorker::writeFailed, qApp,
                     [](const QString &error) {
                         RMessage::showError(error, Preferences().getDefaultParentForMessage());
                     }, Qt::QueuedConnection);

    gLogThread->start();
}

void OrderLog::write(int orderId, const QString &action, const QString &data)
{
    ensureStarted();

    QMetaObject::invokeMethod(gLogWorker, "writeLog", Qt::QueuedConnection,
                              Q_ARG(int, orderId),
                              Q_ARG(QString, action),
                              Q_ARG(QString, data));
}
