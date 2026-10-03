#ifndef RESORTLOG_H
#define RESORTLOG_H

#include "database.h"
#include <QJsonObject>
#include <QString>

class TrackControl;

class ResortLog
{
public:
    static void logFieldChange(const QString &entityType, const QString &entityId,
                               const QString &field, const QString &oldValue, const QString &newValue);
    static void logSnapshot(const QString &entityType, const QString &entityId, const QString &action,
                            const QString &snapshotBefore, const QString &snapshotAfter);
    static void logTrackField(int trackType, const QString &recordId,
                              const QString &field, const QString &oldValue, const QString &newValue);

    static QString entityTypeFromTrack(int trackType);
    static QString dishSnapshotJson(Database &db, int dishId);
    static QString storeDocSnapshotJson(Database &db, int docId);
    static QString giftCardSnapshotJson(Database &db, int cardId);

    static void logJobEvent(const QString &runId, const QString &action, const QJsonObject &data);

private:
    static void insertRow(const QString &entityType, const QString &entityId, const QString &action,
                          const QString &field, const QString &valueOld, const QString &valueNew,
                          const QString &snapshot);
};

#endif // RESORTLOG_H
