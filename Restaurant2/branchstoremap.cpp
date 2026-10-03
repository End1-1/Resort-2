#include "branchstoremap.h"

BranchStoreMap *BranchStoreMap::fInstance = nullptr;

BranchStoreMap::BranchStoreMap(QObject *parent) : QObject(parent)
{
    fInstance = this;
}

bool BranchStoreMap::lookup(int store, int *aliasOut)
{
    if(!aliasOut || fInstance == nullptr) {
        return false;
    }
    if(!fInstance->fStoreMap.contains(store)) {
        return false;
    }
    *aliasOut = fInstance->fStoreMap.value(store);
    return true;
}

bool BranchStoreMap::hasMappings()
{
    return fInstance != nullptr && !fInstance->fStoreMap.isEmpty();
}

int BranchStoreMap::alias(int store)
{
    int mapped = 0;
    if(lookup(store, &mapped)) {
        return mapped;
    }
    Q_ASSERT_X(false, "BranchStoreMap::alias", "Unmapped store — configure r_branch_storemap");
    return 0;
}

void BranchStoreMap::setAlias(int store, int alias)
{
    if (fInstance == nullptr) {
        fInstance = new BranchStoreMap();
    }
    fInstance->fStoreMap[store] = alias;
}
