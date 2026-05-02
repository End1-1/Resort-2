#ifndef CHECKTIME_H
#define CHECKTIME_H

#include <QObject>

class CheckTime : public QObject
{
    Q_OBJECT
public:
    explicit CheckTime(QObject *parent = nullptr);

    bool check();
signals:
};

#endif // CHECKTIME_H
