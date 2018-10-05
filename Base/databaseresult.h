#ifndef DATABASERESULT_H
#define DATABASERESULT_H

#include "database.h"

class DatabaseResult
{
public:
    DatabaseResult();
    bool select(Database &database, const QString &sql, QMap<QString, QVariant> &bind);
    bool select(const QString &sql, QMap<QString, QVariant> &bind);
    QVariant singleResult(const QString &sql, const QMap<QString, QVariant> &bind = QMap<QString, QVariant>());
    QString fLastError;
    inline QVariant value(int row, int column) { return fData.at(row).at(column); }
    inline QVariant value(int row, const QString &column) { return fData.at(row).at(fFieldsMap[column]); }
    inline QVariant value(const QString &column) { return fData.at(0).at(fFieldsMap[column]); }
    inline void setValue(int row, const QString &column, const QVariant &value) {fData[row][fFieldsMap[column]] = value;}
    bool makeIn(int column, QString &result);
    bool makeIn(const QString &column, QString &result);
    inline int rowCount() { return fData.count(); }
    inline int columnCount() { return fFieldsMap.count(); }
    void getBindValues(int row, QMap<QString, QVariant> &bind);
    void removeRow(int row);
    void getFieldsNames(QStringList &sl);
    static QString fMainDbConnection;
private:
    QMap<QString, int> fFieldsMap;
    QList<QList<QVariant> > fData;
    Database fDb;

};

#endif // DATABASERESULT_H
