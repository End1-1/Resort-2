#ifndef BRANCHSTOREMAP_H
#define BRANCHSTOREMAP_H

#include <QObject>
#include <QMap>

#define storealias(x) BranchStoreMap::alias(x)
#define setstorealias(x, y) BranchStoreMap::setAlias(x, y)

class BranchStoreMap : public QObject
{
    Q_OBJECT
public:
    explicit BranchStoreMap(QObject *parent = nullptr);

    QMap<int, int> fStoreMap;

    static int alias(int store);

    static void setAlias(int store, int alias);

private:
    static BranchStoreMap *fInstance;

};

#endif // BRANCHSTOREMAP_H
