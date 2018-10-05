#include "databaseresult.h"
#include <QSqlRecord>
#include <QSqlError>

QString DatabaseResult::fMainDbConnection;

DatabaseResult::DatabaseResult()
{
    Db db = Preferences().getDatabase(fMainDbConnection);
    fDb.setConnectionParams(db.dc_main_host, db.dc_main_path, db.dc_main_user, db.dc_main_pass);
}

bool DatabaseResult::select(Database &database, const QString &sql, QMap<QString, QVariant> &bind)
{
    QSqlQuery *q = 0;
    q = database.select(q, sql, bind);
    if (!q) {
        fLastError = database.fLastError;
        return false;
    }
    fFieldsMap.clear();
    fData.clear();
    QSqlRecord r = q->record();
    int fieldsCount = r.count();
    for (int i = 0; i < fieldsCount; i++) {
        fFieldsMap[r.fieldName(i)] = i;
    }
    while (q->next()) {
        fData.append(QList<QVariant>());
        QList<QVariant> &data = fData[fData.count() - 1];
        for (int i = 0; i < fieldsCount; i++) {
            data << q->value(i);
        }
    }
    return true;
}

bool DatabaseResult::select(const QString &sql, QMap<QString, QVariant> &bind)
{
    if (!fDb.open()) {
        fLastError = fDb.fLastError;
        return false;
    }
    select(fDb, sql, bind);
    fDb.close();
    return true;
}

QVariant DatabaseResult::singleResult(const QString &sql, const QMap<QString, QVariant> &bind)
{
    QVariant result;
    if (!fDb.open()) {
        fLastError = fDb.fLastError;
        return result;
    }
    QSqlQuery q(fDb.fDb);
    if (!q.prepare(sql)) {
        fLastError = q.lastError().databaseText();
        return result;
    }
    for (QMap<QString, QVariant>::const_iterator it = bind.begin(); it != bind.end(); it++) {
        q.bindValue(it.key(), it.value());
    }
    if (!q.exec()) {
        fLastError = q.lastError().databaseText();
        return result;
    }
    if (q.next()) {
        result = q.value(0);
    }
    fDb.close();
    return result;
}

bool DatabaseResult::makeIn(int column, QString &result)
{
    result.clear();
    for (int i = 0, count = fData.count(); i < count; i++) {
        result += fData.at(0).at(column).toString() + ",";
    }
    if (!result.isEmpty()) {
        result.remove(result.length() - 1, 1);
    }
    return !result.isEmpty();
}

bool DatabaseResult::makeIn(const QString &column, QString &result)
{
    int col = fFieldsMap[column];
    return makeIn(col, result);
}

void DatabaseResult::getBindValues(int row, QMap<QString, QVariant> &bind)
{
    const QList<QVariant> &r = fData.at(row);
    for (QMap<QString, int>::const_iterator it = fFieldsMap.begin(); it != fFieldsMap.end(); it++) {
        bind[":" + it.key()] = r.at(it.value());
    }
}

void DatabaseResult::removeRow(int row)
{
    fData.removeAt(row);
}

void DatabaseResult::getFieldsNames(QStringList &sl)
{
    sl = fFieldsMap.keys();
}
