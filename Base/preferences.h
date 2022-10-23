#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "defines.h"
#include <QObject>
#include <QMap>
#include <QDate>
#include <QFont>

#define WORKING_USERID fPreferences.getLocal(def_working_user_id).toInt()
#define WORKING_USERNAME fPreferences.getLocal(def_working_username).toString()
#define WORKING_USERGROUP fPreferences.getLocal(def_working_user_group).toInt()
#define WORKING_DATE fPreferences.getLocalDate(def_working_day)
#define WORKING_USERROLE fPreferences.getLocal(def_working_user_role).toInt()

typedef struct {
    char dc_name[255];
    char dc_main_host[255];
    char dc_main_path[255];
    char dc_main_user[32];
    char dc_main_pass[32];
    char dc_log_host[255];
    char dc_log_path[255];
    char dc_log_user[32];
    char dc_log_pass[32];
} Db;

class Preferences : public QObject {
public:
    Preferences();
    void initFromConfig();
    void initFromDb(const QString &dbName, const QString &additionalSettings, int userId);
    void saveConfig();
    QString getString(const QString &name);
    QString getLocalString(const QString &name);
    QDate getLocalDate(const QString &name);
    void setLocalDate(const QString &name, const QDate &date);
    int getLocalInt(const QString &name);
    quint32 getInt(const QString &name);
    void setDb(const QString &name, const QVariant &value);
    static QVariant getDb(const QString &name);
    void set(const QString &name, const QVariant &value);
    void setLocal(const QString &name, const QVariant &value);
    QVariant getLocal(const QString &name);
    QWidget *getDefaultParentForMessage();
    void setDefaultParentForMessage(QWidget *parent);
    void clearDatabase();
    void appendDatabase(const QString &name, const QString &mainHost, const QString &mainDb, const QString &mainUser, const QString &mainPassword, const QString &logHost, const QString &logDb, const QString &logUser, const QString &logPassword);
    void setDatabasesNames(QStringList &names);
    Db getDatabase(const QString &name);
    Db getFirstDatabase();
    void removeDatabase(const QString &name);
    static void qstringToChar(char *dst, const QString &src);
    QString getUser(const QString &key);
    void setUser(const QString &connName, const QString &key, const QString &value);
    static QString hostUsername();
private:
    static QMap<QString, QVariant> fConfigPreferences;
    static QMap<QString, QVariant> fDbPreferences;
    static QMap<QString, QVariant> fLocalPreferences;
    static QMap<QString, Db> fDatabases;
    static QMap<QString, QString> fUserPreferences;
    void initMainPreferences();
};

extern QString __TAX_HOST;
extern QString __TAX_PORT;
extern QString __TAX_PASS;

extern Preferences __preferences;

#endif // PREFERENCES_H
