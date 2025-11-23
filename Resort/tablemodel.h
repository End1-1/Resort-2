#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QTableView>
#include "doubledatabase.h"

#define COLOR_DARK_ROW QColor::fromRgb(230, 230, 230)
#define COLOR_VERY_DARK_ROW QColor::fromRgb(100, 100, 100)

class Column
{
public:
    Column(int width, const QString &fieldName, const QString &title);
    ~Column();
    int fIndex;
    int fWidth;
    QString fFieldName;
    QString fTitle;
};

class Row
{
    int fIndex;
};

class WReportGrid;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QTableView *tableView);
    ~TableModel();
    DoubleDatabase fDD;
    QTableView* fTableView;
    void apply(WReportGrid *rg);
    void apply(const QStringList &queries);
    void applyFinal(WReportGrid *rg, bool clearBefore);
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual int rowCount(const QModelIndex &parent) const;
    int rowCount() const;
    virtual int columnCount(const QModelIndex &parent) const;
    int columnCount() const;
    int columnWidth(int index) const;
    QString columnTitle(int index) const;
    int columnIndex(const QString &title);
    virtual QVariant data(const QModelIndex &index, int role) const;
    QVariant data(int row, int column, int role = Qt::DisplayRole) const;
    QString stringData(int row, int col) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setData(int row, int col, const QVariant &value);
    void setBackgroundColor(int row, const QColor &color);
    void setBackgroundColor(int row, int column, const QColor &color);
    void clearColumns();
    void clearData();
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setDataFromSource(const QList<QList<QVariant> >& dataSource);
    void clearProxyRows();
    TableModel& setColumn(int width, const QString &fieldName, const QString &title);
    TableModel& setColumn(Column *column);
    TableModel& setColumns(const QList<Column>& cols);
    void setSqlQuery(const QString &query);
    void searchInTable(const QString &text);
    void appendRow(const QList<QVariant>& values);
    void insertRow(int row, const QList<QVariant>& values);
    void removeRow(int row);
    void updateRowValues(int row, const QList<QVariant>& values);
    void uniqueValuesForColumn(int column, QSet<QString>& values);
    void sumOfColumns(const QList<int> columns, QList<double>& out);
    void insertSubTotals(int colName, const QList<int>& totalCols);
    QList<int> fCheckBoxColumns;
    QList<int> fCheckBoxIsCheckable;
private:
    Qt::SortOrder fLastSortOrder;
    int fLastSortIndex;
    QList<Column*> fColumns;
    QList<int> fRows;
    QStringList fQuery;
    QLocale fLocale;
    QList<int> fSumColumns;
    QList<double> fSumValues;
    QMap<int, QMap<int, QColor> > fBackgroundColors;
signals:
    void rowCount(int count);
    void endApply();
    void newSum(const QList<int>& columns, const QList<double>& values);
};

#endif // TABLEMODEL_H
