#include "user.h"

User::User()
{
    fValid = false;
}

User::User(const QString &password, QObject *parent) :
    QObject(parent),
    Base()
{
    QString query = "select f_id, f_group, concat(f_firstName, ' ', f_lastName) from users where f_altPassword=md5(:pass)";
    fDbBind[":pass"] = password;
    fDb.select(query, fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        fValid = false;
    } else {
        fId = fDbRows.at(0).at(0).toInt();
        fGroup = fDbRows.at(0).at(1).toInt();
        fName = fDbRows.at(0).at(2).toString();
        fValid = true;
    }
}

bool User::isValid()
{
    return fValid;
}

void User::loadUser()
{

}
