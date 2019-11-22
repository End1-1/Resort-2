#include "basevoucher.h"
#include "pprintvaucher.h"
#include <QMessageBox>

QStringList BaseVoucher::fFields;

BaseVoucher::BaseVoucher() :
    Base()
{
    setFields();
    foreach (QString s, fFields) {
        fSet[s] = false;
    }
    fNeedInsert = false;
    /* TO BE REMOVED AFTER AUDIT */
    set("f_doc", "");
    set("f_rec", "");
    /* END TO BE REMOVED */
}

BaseVoucher &BaseVoucher::set(const QString &field, const QVariant &value)
{
    if (!fFields.contains(field)) {
        QMessageBox::critical(fPreferences.getDefaultParentForMessage(), "Error", QString("Field not exists") + QString("\r\n") + field);
        return *this;
    }
    fValues[field] = value;
    fSet[field] = true;
    return *this;
}

void BaseVoucher::setA(const QString &field, const QVariant &value)
{
    fAddValues[field] = value;
}

QVariant BaseVoucher::get(const QString &field)
{
    return fValues[field];
}

QVariant BaseVoucher::getA(const QString &field)
{
    return fAddValues[field];
}

QString BaseVoucher::genId(const QString &source)
{
    QString id = uuuid(source, fAirDb);
    set("f_id", id);
    set("f_source", source);
    fNeedInsert = true;
    return id;
}

bool BaseVoucher::load(const QString &id)
{
    fDbBind[":f_id"] = id;
    DatabaseResult dr;
    dr.select(fDb, "select * from m_register where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        return false;
    }
    foreach (QString s, fFields) {
        fValues[s] = dr.value(s);
    }
    return loadAdditionalData();
}

void BaseVoucher::print()
{
    PPrintVaucher::printVaucher(fValues["f_id"].toString());
}

void BaseVoucher::print(const QString &id)
{
    PPrintVaucher::printVaucher(id);
}

void BaseVoucher::setFields()
{
    if (fFields.count() > 0) {
        return;
    }
    DatabaseResult dr;
    dr.select(fDb, "describe m_register", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fFields.append(dr.value(i, "Field").toString());
    }
}

void BaseVoucher::save()
{
    if (fNeedInsert) {
        fDbBind[":f_id"] = fValues["f_id"];
        fDb.insertWithoutId("m_register", fDbBind);
    }
    for (QMap<QString, QVariant>::const_iterator it = fValues.begin(); it != fValues.end(); it++) {
        if (it.key() == "f_id") {
            continue;
        }
        fDbBind[":" + it.key()] = it.value();
    }
    fDb.update("m_register", fDbBind, where_id(ap(fValues["f_id"].toString())));
}
