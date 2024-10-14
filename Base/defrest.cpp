#include "defrest.h"
#include "database2.h"

QMap<QString, QString> DefRest::fData;

DefRest::DefRest(const QString &comp) :
    Base()
{
//    Db b = Preferences().getDatabase(Base::fDbName);
//    Database2 db;
//    db.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
//    db[":f_comp"] = comp;
//    db.exec("select f_key, f_value from r_config where upper(f_comp)=upper(:f_comp)");
//    while (db.next()) {
//        fData[db.string(0)] = db.string("f_value");
//    }
    fDbBind[":f_comp"] = comp;
    fDb.select("select f_key, f_value from r_config where upper(f_comp)=upper(:f_comp)", fDbBind, fDbRows);
    foreach_rows {
        fData[it->at(0).toString()] = it->at(1).toString();
    }
    fDb.close();
    qDebug() << fData;
}

QString DefRest::v(const QString &key)
{
    return fData[key];
}
