#ifndef REPORTQUERY_H
#define REPORTQUERY_H

#include <QString>
#include <QList>
#include <QMap>

class QComboBox;

class ReportQuery
{
public:
    ReportQuery(const QString &name);
    QString query;
    QString reportTitle;
    QList<int> sumColumns;
    QMap<int, int> columnsWidths;
    void loadData(const QString &name);
    void costumizeCombo(QComboBox *c, const QString &dataname, bool all = true, int index = -1);
};

#endif // REPORTQUERY_H
