#include "cachereminderstate.h"

CacheReminderState *CacheReminderState::fInstance = 0;

CacheReminderState *CacheReminderState::instance()
{
    if (!fInstance) {
        fInstance = new CacheReminderState();
        fCacheOne[cid_reminder_state] = fInstance;
    }
    return fInstance;
}

CacheReminderState::CacheReminderState() :
    CacheBase()
{
    fCacheId = cid_reminder_state;
    fQuery = "select f_id, f_" + def_lang + " from f_reminder_state";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheReminderState::~CacheReminderState()
{
    fInstance = 0;
}
