#ifndef ECOMBOBOXCOMPLETER_H
#define ECOMBOBOXCOMPLETER_H

#include <QComboBox>

class EComboBoxCompleter : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY (QString SQL READ getSQL WRITE setSQL)
public:
    EComboBoxCompleter(QWidget *parent = 0);
    QVariant currentData() const;
    QString getSQL();
    void setSQL(const QString &sql);
    void refreshData();
    static void setDefaultDatabaseName(const QString &dbName);
    static void clearCache();
protected:
    QString fDatabaseName;
    QString fSql;
    static QMap<QString, QMap<QString, QString> > fCache;
private:
    static QString fDefaultDatabaseName;
    virtual void setData();
private slots:
    void textChanged(const QString &text);
};

#endif // ECOMBOBOXCOMPLETER_H
