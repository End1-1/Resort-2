#ifndef BASEVOUCHER_H
#define BASEVOUCHER_H

#include "base.h"

class BaseVoucher : public Base
{
public:
    BaseVoucher();
    BaseVoucher &set(const QString &field, const QVariant &value);
    void setA(const QString &field, const QVariant &value);
    QVariant get(const QString &field);
    QVariant getA(const QString &field);
    QString genId(const QString &source);
    bool load(const QString &id);
    void print();
    static void print(const QString &id);
    virtual void saveVoucher() = 0;
protected:
    virtual bool loadAdditionalData() = 0;
    void save();
    bool fNeedInsert;
    QMap<QString, QVariant> fValues;
    QMap<QString, QVariant> fAddValues;
private:
    static QStringList fFields;
    QMap<QString, bool> fSet;
    void setFields();
};

#endif // BASEVOUCHER_H
