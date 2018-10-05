#include "cachebase.h"

QSqlDatabase fCacheDb;
QDate fCacheDate;
QMap<int, QMap<QString, CI_Base*> > fStaticCache;
