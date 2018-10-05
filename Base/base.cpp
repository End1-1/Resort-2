#include "base.h"
#include "trackcontrol.h"

Preferences Base::fPreferences;
QString Base::fDbName;
QString Base::fAirDbName;
QString Base::fAirHost;
QString Base::fAirUser;
QString Base::fAirPass;

Base::Base()
{
    if (!fDbName.isEmpty()) {
        Db db = fPreferences.getDatabase(fDbName);
        fDb.setConnectionParams(db.dc_main_host, db.dc_main_path, db.dc_main_user, db.dc_main_pass);
        fDb.open();
    }
    fAirDb.setConnectionParams(fAirHost, fAirDbName, fAirUser, fAirPass);
}

Base::~Base()
{
    fDb.close();
}

void Base::selector(int selectorNumber, const QVariant &value)
{
    Q_UNUSED(selectorNumber)
    Q_UNUSED(value)
}
