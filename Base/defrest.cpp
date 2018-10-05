#include "defrest.h"

QMap<QString, QString> DefRest::fData;

DefRest::DefRest(const QString &comp) :
    Base()
{
    fDbBind[":f_comp"] = comp;
    fDb.select("select f_key, f_value from r_config where upper(f_comp)=upper(:f_comp)", fDbBind, fDbRows);
    foreach_rows {
        fData[it->at(0).toString()] = it->at(1).toString();
    }
    fDb.close();
}

QString DefRest::v(const QString &key)
{
    return fData[key];
}
