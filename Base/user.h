#ifndef USER_H
#define USER_H

#include "base.h"
#include <QObject>

class User : public QObject, Base
{
    Q_OBJECT
public:
    explicit User();
    explicit User(const QString &password, QObject *parent = 0);
    explicit User(const QString &userName, const QString &password);
    bool isValid();
    int fId;
    int fGroup;
    QString fName;
signals:

public slots:
private:
    bool fValid;
    void loadUser();
};

#endif // USER_H
