#ifndef CACHEREMINDERSTATE_H
#define CACHEREMINDERSTATE_H

#include "cachebase.h"

#define cid_reminder_state 56

typedef struct : CI_Base {

} CI_ReminderState;
Q_DECLARE_METATYPE(CI_ReminderState*)

class CacheReminderState : public CacheBase<CI_ReminderState>
{
public:
    static CacheReminderState *instance();
private:
    static CacheReminderState *fInstance;
    CacheReminderState();
    ~CacheReminderState();
};

#endif // CACHEREMINDERSTATE_H
