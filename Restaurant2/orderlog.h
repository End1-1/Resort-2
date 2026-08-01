#ifndef ORDERLOG_H
#define ORDERLOG_H

#include <QObject>
#include <QString>

class OrderLogWorker : public QObject
{
    Q_OBJECT

public slots:
    void writeLog(int orderId, const QString &action, const QString &data);

signals:
    void writeFailed(const QString &error);
};

class OrderLog
{
public:
    static void init();
    static void write(int orderId, const QString &action, const QString &data);

    static const char *ACTION_ORDER_OPEN;
    static const char *ACTION_DISH_ADD;
    static const char *ACTION_MENU_CHANGE;
    static const char *ACTION_DISCOUNT_OK;
    static const char *ACTION_DISCOUNT_FAIL;

private:
    static void ensureStarted();
};

#endif // ORDERLOG_H
