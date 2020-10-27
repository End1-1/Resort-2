#ifndef CACHEBASE_H
#define CACHEBASE_H

#include "defines.h"
#include "cacheone.h"
#include <QMap>
#include <QVariant>
#include <QDate>

#ifdef QT_DEBUG
#include <QDebug>
#endif

typedef struct CI_Base {
    QString fCode;
    QString fName;
    bool fValid;
    CI_Base() {fValid = true;}
} CI_Base;
Q_DECLARE_METATYPE(CI_Base*)

template<class T>
class CacheBase : public CacheOne
{
public:
    CacheBase() :
        CacheOne()
    {

    }

    virtual void clear() {
        if (fStaticCache.contains(fCacheId)) {
            qDeleteAll(fStaticCache[fCacheId]);
            fStaticCache.remove(fCacheId);
        }
    }

    static bool isEmpty(int cacheId) {
        return !fStaticCache.contains(cacheId);
    }

    virtual ~CacheBase() {
    }

    virtual void load() {
        QSqlQuery *q = prepareDb();
        while (q->next()) {
            T *t = new T();
            t->fCode = q->value(0).toString();
            t->fName = q->value(1).toString();
            fStaticCache[fCacheId][t->fCode] = t;
        }
        closeDb(q);
    }

    T *get(const QString &code) {
        if (fStaticCache[fCacheId].isEmpty()) {
            load();
        }
        if (fStaticCache[fCacheId].contains(code)) {
            return reinterpret_cast<T*>(fStaticCache[fCacheId][code]);
        }
        return 0;
    }

    T *get(int code)
    {
        return get(QString::number(code));
    }

    QMapIterator<QString, T*> it()
    {
        if (fStaticCache[fCacheId].isEmpty()) {
            load();
        }
        return QMapIterator<QString, T*>(fStaticCache[fCacheId]);
    }

    int elementsCount() {
        return fStaticCache[fCacheId].count();
    }

protected:
    static QMap<int, QMap<QString, T*> > fStaticCache;
};

template<typename T>
QMap<int, QMap<QString, T*> > CacheBase<T>::fStaticCache;



#endif // CACHEBASE_H
