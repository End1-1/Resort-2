#include "database2.h"
#include "logwriter.h"
#include <QDateTime>
#include <QSqlRecord>
#include <QSettings>
#include <QUuid>

QMutex Database2::fMutex;
int Database2::fDatabaseCounter = 0;

Database2::Database2() :
    Database2(_DBDRIVER_)
{
}

Database2::Database2(Database2 &other) :
    Database2(_DBDRIVER_)
{
    open(other.fSqlDatabase.hostName(), other.fSqlDatabase.databaseName(), other.fSqlDatabase.userName(), other.fSqlDatabase.password());
}

Database2::Database2(const QString &driverName)
{
    fQuery = nullptr;
    fDatabaseDriver = driverName;
    QMutexLocker ml(&fMutex);
    fDatabaseNumber = QString::number(++fDatabaseCounter);
    //LogWriter::write(LogWriterLevel::special, "", QString("DB constructor %1").arg(fDatabaseNumber));
}

Database2::~Database2()
{
    if(fQuery) {
        fQuery->finish();
        delete fQuery;
    }

    fSqlDatabase.close();
    fSqlDatabase = QSqlDatabase::addDatabase(fDatabaseDriver);
    QSqlDatabase::removeDatabase("DB2" + fDatabaseNumber);
    //LogWriter::write(LogWriterLevel::special, "", QString("DB destructor %1").arg(fDatabaseNumber));
}

bool Database2::open(const QString &configFile)
{
    QSettings s(configFile, QSettings::IniFormat);
    return open(s.value("db/host").toString(), s.value("db/schema").toString(), s.value("db/username").toString(), s.value("db/password").toString());
}

bool Database2::open(const QString &host, const QString &schema, const QString &username, const QString &password)
{
    fSqlDatabase = QSqlDatabase::addDatabase(fDatabaseDriver, "DB2" + fDatabaseNumber);
    fSqlDatabase.setHostName(host);
    fSqlDatabase.setDatabaseName(schema);
    fSqlDatabase.setUserName(username);
    fSqlDatabase.setPassword(password);

    if(!fSqlDatabase.open()) {
        LogWriter::write(LogWriterLevel::errors, "open database", fSqlDatabase.lastError().databaseText());
        return false;
    }

    fQuery = new QSqlQuery(fSqlDatabase);
    return true;
}

bool Database2::startTransaction()
{
    return fSqlDatabase.transaction();
}

bool Database2::commit()
{
    return fSqlDatabase.commit();
}

bool Database2::rollback()
{
    return fSqlDatabase.rollback();
}

bool Database2::exec(const QString &query)
{
    LogWriter::write(LogWriterLevel::verbose, "", query);

    if(!fQuery->prepare(query)) {
        LogWriter::write(LogWriterLevel::errors, fSqlDatabase.databaseName(), fQuery->lastError().databaseText());
        LogWriter::write(LogWriterLevel::errors, fSqlDatabase.databaseName(), lastQuery());
        //Q_ASSERT(false);
        return false;
    }

    LogWriter::write(LogWriterLevel::verbose, "", "prepear complete");
    QStringList keys = fBindValues.keys();

    for(const QString &key : qAsConst(keys)) {
        fQuery->bindValue(key, fBindValues[key]);
    }

    fBindValues.clear();

    if(!fQuery->exec()) {
        LogWriter::write(LogWriterLevel::errors, fSqlDatabase.databaseName(), fQuery->lastError().databaseText());
        LogWriter::write(LogWriterLevel::errors, fSqlDatabase.databaseName(), lastQuery());
        //Q_ASSERT(false);
        return false;
    }

    LogWriter::write(LogWriterLevel::verbose, fSqlDatabase.databaseName(), lastQuery());
    bool isSelect = fQuery->isSelect() || query.contains("call ", Qt::CaseInsensitive);

    if(isSelect) {
        //fQuery->first();
        fColumnsNames.clear();
        QSqlRecord rec = fQuery->record();

        for(int i = 0; i < rec.count(); i++) {
            fColumnsNames[rec.fieldName(i).toLower()] = i;
            fColumnsIndexes[i] = rec.fieldName(i).toLower();
        }
    }

    LogWriter::write(LogWriterLevel::verbose, "", fSqlDatabase.databaseName() + QString(" Affected rows %1").arg(fQuery->numRowsAffected()));
    return true;
}

bool Database2::execDirect(const QString &query)
{
    if(!fQuery->prepare(query)) {
        LogWriter::write(LogWriterLevel::errors, "", fQuery->lastError().databaseText());
        LogWriter::write(LogWriterLevel::errors, "", query);
        Q_ASSERT(false);
        return false;
    }

    QStringList keys = fBindValues.keys();

    for(const QString &key : qAsConst(keys)) {
        fQuery->bindValue(key, fBindValues[key]);
    }

    fBindValues.clear();

    if(!fQuery->exec()) {
        LogWriter::write(LogWriterLevel::errors, "", fQuery->lastError().databaseText());
        LogWriter::write(LogWriterLevel::errors, "", query);
        Q_ASSERT(false);
        return false;
    }

    return true;
}

bool Database2::insert(const QString &table)
{
    QString sql = "insert into " + table;
    QString k, v;
    bool first = true;

    for(QMap<QString, QVariant>::const_iterator it = fBindValues.constBegin(); it != fBindValues.constEnd(); it++) {
        if(first) {
            first = false;
        } else {
            k += ",";
            v += ",";
        }

        k += QString(it.key()).remove(0, 1);
        v += it.key();
    }

    sql += QString("(%1) values (%2)").arg(k, v);
    return exec(sql);
}

bool Database2::insert(const QString &table, int& id)
{
    id = 0;

    if(insert(table)) {
        fQuery->exec("select last_insert_id()");

        if(fQuery->next()) {
            id = fQuery->value(0).toInt();
        }
    }

    return id > 0;
}

bool Database2::update(const QString &table)
{
    return update(table, "fid", fBindValues[":fid"]);
}

bool Database2::update(const QString &table, const QString &field, const QVariant &value)
{
    fBindValues[":" + field] = value;
    QString sql = "update " + table + " set ";
    bool first = true;

    for(QMap<QString, QVariant>::const_iterator it = fBindValues.constBegin(); it != fBindValues.constEnd(); it++) {
        if(first) {
            first = false;
        } else {
            sql += ",";
        }

        QString f = it.key();
        sql += f.remove(0, 1) + "=" + it.key();
    }

    sql += QString(" where %1=:%1").arg(field);
    return exec(sql);
}

bool Database2::deleteFromTable(const QString &table, const QString &field, const QVariant &value)
{
    fBindValues[":" + field] = value;
    return exec(QString("delete from %1 where %2=:%2").arg(table, field));
}

QString Database2::uuid()
{
    return QUuid::createUuid().toString().replace("{", "").replace("}", "");
}

void Database2::setBindValues(const QMap<QString, QVariant>& v)
{
    fBindValues = v;
}

QMap<QString, QVariant> Database2::getBindValues()
{
    QMap<QString, QVariant> b;

    for(QHash<QString, int>::const_iterator it = fColumnsNames.constBegin(); it != fColumnsNames.constEnd(); it++) {
        b[":" + it.key()] = fQuery->value(fColumnsNames[it.key()]);
    }

    return b;
}

void Database2::close()
{
    if(fQuery) {
        fQuery->clear();
    }

    fSqlDatabase.close();
}

QVariant& Database2::operator[](const QString &name)
{
    return fBindValues[name];
}

const QString Database2::lastQuery()
{
    QString sql = fQuery->lastQuery();
    QMapIterator<QString, QVariant> it(fQuery->boundValues());

    while(it.hasNext()) {
        it.next();
        QVariant value = it.value();

        switch(it.value().type()) {
        case QVariant::String:
            value = QString("'%1'").arg(value.toString().replace("'", "''"));
            break;

        case QVariant::Date:
            value = QString("'%1'").arg(value.toDate().toString("yyyy-MM-dd"));
            break;

        case QVariant::DateTime:
            value = QString("'%1'").arg(value.toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            break;

        case QVariant::Double:
            value = QString("%1").arg(value.toDouble());
            break;

        case QVariant::Int:
            value = QString("%1").arg(value.toInt());
            break;

        case QVariant::Time:
            value = QString("'%1'").arg(value.toTime().toString("HH:mm:ss"));
            break;

        case QVariant::ByteArray:
            value = QString("'%1'").arg(QString(value.toByteArray().toHex()));
            break;

        default:
            break;
        }

        sql.replace(QRegExp(it.key() + "\\b"), value.toString());
    }

    return sql;
}

QString Database2::lastDbError() const
{
    if(fQuery) {
        return fQuery->lastError().databaseText();
    } else {
        return fSqlDatabase.lastError().databaseText();
    }
}

int Database2::genFBID(const QString &name)
{
    if(exec("select gen_id(" + name + ",1) as fid from rdb$database")) {
        if(next()) {
            return integer("fid");
        }
    }

    return 0;
}

void Database2::rowToMap(QMap<QString, QVariant>& map)
{
    for(int i = 0; i < columnCount(); i++) {
        map[columnName(i)] = value(i);
    }
}
