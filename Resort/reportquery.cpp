#include "reportquery.h"
#include "doubledatabase.h"
#include "database2.h"
#include "message.h"
#include <QComboBox>
#include <QDebug>

ReportQuery::ReportQuery(const QString &name)
{
    loadData(name);
}

void ReportQuery::loadData(const QString &name)
{
    sumColumns.clear();
    columnsWidths.clear();
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_name"] = name;
    db.exec("select f_query from queries where f_name=:f_name");
    db.next(true);
    query = db.string("f_query");

    db[":f_name"] = name;
    db.exec("select f_columns from queries_sum where f_name=:f_name");
    db.next(true);
    QStringList cols = db.string("f_columns").split(",", Qt::SkipEmptyParts);
    for (const QString &s: qAsConst(cols)) {
        sumColumns.append(s.toInt());
    }

    db[":f_name"] = name;
    db.exec("select f_widths from queries_columns_widths where f_name=:f_name");
    db.next(true);
    QStringList widths = db.string("f_widths").split(",", Qt::SkipEmptyParts);
    for (const QString &s: qAsConst(widths)) {
        QStringList v = s.split(":", Qt::SkipEmptyParts);
        if (v.count() == 2) {
            columnsWidths[v.at(0).toInt()] = v.at(1).toInt();
        }
    }

    db[":f_query"] = name;
    db.exec("select * from queries_names where f_query=:f_query");
    if (db.next()) {
        reportTitle = db.string("f_name");
    }
}

void ReportQuery::costumizeCombo(QComboBox *c, const QString &dataname, bool all, int index)
{
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_name"] = dataname;
    db.exec("select f_query from queries_data where f_name=:f_name");
    db.next(true);
    if (all) {
        c->addItem("*", 0);
    }
    db.exec(db.string(db.string("f_query")));
    while (db.next()) {
        c->addItem(db.string("f_name"));
        for (int i = 1; i < db.columnCount(); i++) {
            c->setItemData(c->count() - 1, db.value(i), Qt::UserRole + i - 1);
        }
    }
    if (index > -1) {
        c->setCurrentIndex(index);
    }
}
