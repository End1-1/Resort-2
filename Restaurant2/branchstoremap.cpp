#include "branchstoremap.h"

BranchStoreMap *BranchStoreMap::fInstance = nullptr;

BranchStoreMap::BranchStoreMap(QObject *parent) : QObject(parent)
{
    fInstance = this;
}

int BranchStoreMap::alias(int store)
{
    Q_ASSERT(fInstance->fStoreMap.contains(store));
    return fInstance->fStoreMap[store];
}

void BranchStoreMap::setAlias(int store, int alias)
{
    if (fInstance == nullptr) {
        fInstance = new BranchStoreMap();
    }
    fInstance->fStoreMap[store] = alias;
}
