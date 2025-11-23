#include "tablemodel.h"
#include "wreportgrid.h"
#include <QHeaderView>

TableModel::TableModel(QTableView *tableView) :
    QAbstractTableModel(tableView)
{
    fTableView = tableView;
    fLastSortIndex = -1;
    fLastSortOrder = Qt::AscendingOrder;
}

TableModel::~TableModel()
{
    qDeleteAll(fColumns);
    fColumns.clear();
}

void TableModel::apply(WReportGrid *rg)
{
    clearProxyRows();
    beginResetModel();

    if(rg == 0 || fQuery.count() > 0) {
        if(fQuery.count() > 0) {
            if(!fDD.open(true, false)) {
                message_error("Sql error<br>" + fDD.fLastError + "<br>Additional information in the log file");
            } else {
                fBackgroundColors.clear();

                foreach(QString s, fQuery) {
                    if(!fDD.exec(s)) {
                        message_error("Sql error<br>" + fDD.fLastError + "<br>Additional information in the log file");
                    }
                }

                fDD.close();
            }
        }
    } else {
        //rg->getModelData(fDD.fDbRows);
    }

    applyFinal(rg, false);
}

void TableModel::apply(const QStringList &queries)
{
    fQuery = queries;
    clearProxyRows();
    beginResetModel();
    fDD.open(true, false);
    fBackgroundColors.clear();

    foreach(QString s, queries) {
        if(!fDD.exec(s)) {
            message_error("Sql error<br>" + fDD.fLastError + "<br>Additional information in the log file");
        }
    }

    fDD.close();
    applyFinal(0, false);
}

void TableModel::applyFinal(WReportGrid *rg, bool clearBefore)
{
    if(clearBefore) {
        clearProxyRows();
        beginResetModel();
    }

    for(int i = 0, count = fDD.rowCount(); i < count; i++)
        fRows.append(i);

    fTableView->setModel(this);

    if(fDD.rowCount() > 0) {
        if(fColumns.count() == 0) {
            for(int i = 0; i < fDD.fDbRows.at(0).count(); i++) {
                setColumn(80, "", fDD.fDefColNames[i]);
            }
        }

        if(fColumns.count() != fDD.fDbRows.at(0).count()) {
            qDebug() << "Stop! Columns and fields is not equal!" << fColumns.count() << fDD.fDbRows.at(0).count();
        }
    }

    if(rg) {
        rg->dontResizeSave(true);
    }

    for(int i = 0; i < fColumns.count(); i++)
        fTableView->setColumnWidth(i, fColumns.at(i)->fWidth);

    if(rg) {
        rg->dontResizeSave(false);
    }

    endResetModel();
    emit rowCount(fRows.count());
    emit endApply();
}

void TableModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(order)
    QMap<QVariant, int> m;

    for(int i = 0, count = fRows.count(); i < count; i++) {
        m.insertMulti(fDD.fDbRows.at(fRows.at(i)).at(column), fRows.at(i));
    }

    beginResetModel();
    fRows = m.values();

    if(fLastSortIndex == column) {
        fLastSortOrder = fLastSortOrder == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        fLastSortOrder = Qt::AscendingOrder;
    }

    fLastSortIndex = column;

    if(fLastSortOrder == Qt::DescendingOrder) {
        std::reverse(fRows.begin(), fRows.end());
    }

    endResetModel();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role) {
    case Qt::DisplayRole: {
        switch(orientation) {
        case Qt::Horizontal:
            return fColumns[section]->fTitle;

        case Qt::Vertical:
            return QString::number(section + 1);
        }
    }

    default:
        return QVariant();
    }
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return fRows.count();
}

int TableModel::rowCount() const
{
    return rowCount(QModelIndex());
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return fColumns.count();
}

int TableModel::columnCount() const
{
    return columnCount(QModelIndex());
}

int TableModel::columnWidth(int index) const
{
    if(index > fColumns.count() - 1)
        return 0;

    return fColumns[index]->fWidth;
}

QString TableModel::columnTitle(int index) const
{
    if(index > fColumns.count() - 1)
        return "";

    return fColumns[index]->fTitle;
}

int TableModel::columnIndex(const QString &title)
{
    for(int i = 0; i < fColumns.count(); i++) {
        if(fColumns.at(i)->fTitle == title) {
            return i;
        }
    }

    return -1;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const QVariant &v = fDD.fDbRows.at(fRows.at(index.row())).at(index.column());

    switch(role) {
    case Qt::DisplayRole: {
        switch(v.type()) {
        case QVariant::Date:
            return v.toDate().toString(def_date_format);

        case QVariant::DateTime:
            return v.toDateTime().toString(def_date_time_format);

        case QVariant::Time:
            return v.toTime().toString("HH:mm:ss");

        case QVariant::Double:
            return fLocale.toString(v.toDouble(), 'f', 2).remove(QRegExp("\\.0+$")).remove(QRegExp("\\.$"));

        default:
            return v;
        }
    }

    case Qt::EditRole:
        return fDD.fDbRows.at(fRows.at(index.row())).at(index.column());

    case Qt::BackgroundColorRole:
        if(fBackgroundColors.contains(index.row())) {
            return fBackgroundColors[index.row()][index.column()];
        }

        return QVariant(QColor(Qt::white));
        break;

    case Qt::CheckStateRole: {
        if(fCheckBoxColumns.contains(index.column())) {
            return v.toInt() == 0 ? Qt::Unchecked : Qt::Checked;
        }
    }

    default:
        return QVariant();
    }
}

QVariant TableModel::data(int row, int column, int role) const
{
    return data(createIndex(row, column), role);
}

QString TableModel::stringData(int row, int col) const
{
    QModelIndex index = createIndex(row, col);
    return data(index, Qt::EditRole).toString();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()) {
        return false;
    }

    if(role == Qt::CheckStateRole) {
        if(fCheckBoxIsCheckable.contains(index.column())) {
            if((Qt::CheckState)value.toInt() == Qt::Checked) {
                setData(index.row(), index.column(), 1);
                return true;
            } else {
                setData(index.row(), index.column(), 0);
                return true;
            }
        }
    }

    return false;
}

void TableModel::setData(int row, int col, const QVariant &value)
{
    int r = fRows.at(row);
    fDD.fDbRows[r][col] = value;
    resetInternalData();
}

void TableModel::setBackgroundColor(int row, const QColor &color)
{
    for(int i = 0; i < columnCount(); i++) {
        fBackgroundColors[row][i] = color;
    }
}

void TableModel::setBackgroundColor(int row, int column, const QColor &color)
{
    if(!fBackgroundColors.contains(row)) {
        for(int i = 0; i < columnCount(); i++) {
            fBackgroundColors[row][i] = Qt::white;
        }
    }

    fBackgroundColors[row][column] = color;
}

void TableModel::clearColumns()
{
    qDeleteAll(fColumns);
    fColumns.clear();
    fSumColumns.clear();
    fSumValues.clear();
    fCheckBoxColumns.clear();
}

void TableModel::clearData()
{
    if(fDD.fDbRows.count() == 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
    fRows.clear();
    fDD.fDbRows.clear();
    fBackgroundColors.clear();
    endRemoveRows();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(fCheckBoxColumns.contains(index.column())) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    }

    return QAbstractTableModel::flags(index);
}

void TableModel::setDataFromSource(const QList<QList<QVariant> >& dataSource)
{
    clearData();
    fDD.fDbRows = dataSource;
    apply(0);
}

void TableModel::clearProxyRows()
{
    if(fRows.count() == 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
    fRows.clear();
    endRemoveRows();
}

TableModel& TableModel::setColumn(int width, const QString &fieldName, const QString &title)
{
    Column *c = new Column(width, fieldName, title);
    fColumns.append(c);
    return *this;
}

TableModel& TableModel::setColumn(Column *column)
{
    fColumns.append(column);
    return *this;
}

TableModel& TableModel::setColumns(const QList<Column>& cols)
{
    for(auto  &c : cols) {
        setColumn(c.fWidth, c.fFieldName, c.fTitle);
    }

    return *this;
}

void TableModel::setSqlQuery(const QString &query)
{
    fQuery.clear();

    if(!query.isEmpty()) {
        fQuery << query;
    }
}

void TableModel::searchInTable(const QString &text)
{
    auto *sm = fTableView->selectionModel();

    if(!sm) {
        return;
    }

    QModelIndexList sel = sm->selectedColumns();
    QList<int> columns;

    if(sel.count() == 0) {
        for(int i = 0; i < fColumns.count(); i++) {
            columns.append(i);
        }
    } else {
        for(QModelIndexList::const_iterator it = sel.begin(); it != sel.end(); it++) {
            columns.append(it->column());
        }
    }

    QList<int> tempRows;
    QStringList searchList = text.split("|");

    for(int i = 0; i < fDD.fDbRows.count(); i++) {
        QList<QVariant>& row = fDD.fDbRows[i];
        bool found = false;

        for(int j = 0; j < columns.count(); j++) {
            if(fTableView->columnSpan(i, j) > 1) {
                continue;
            }

            for(QStringList::const_iterator si = searchList.begin(); si != searchList.end(); si++) {
                const QVariant &v = row.at(columns.at(j));
                QString cmp;

                switch(v.type()) {
                case QVariant::Double:
                    cmp = float_str(v.toDouble(), 2);
                    break;

                case QVariant::Date:
                    cmp = v.toDate().toString(def_date_format);
                    break;

                case QVariant::DateTime:
                    cmp = v.toDateTime().toString(def_date_time_format);
                    break;

                default:
                    cmp = v.toString();
                    break;
                }

                if(cmp.contains(*si, Qt::CaseInsensitive)) {
                    tempRows.append(i);
                    found = true;
                    break;
                }
            }

            if(found) {
                break;
            }
        }
    }

    beginResetModel();
    fRows = tempRows;
    endResetModel();
    QItemSelection selection;
    bool first = true;

    if(sel.count() > 0) {
        for(QModelIndexList::const_iterator it = sel.begin(); it != sel.end(); it++) {
            fTableView->selectColumn(it->column());

            if(first) {
                first = false;
                selection = fTableView->selectionModel()->selection();
            } else {
                selection.merge(fTableView->selectionModel()->selection(), QItemSelectionModel::Select);
            }
        }
    }

    fTableView->clearSelection();
    fTableView->selectionModel()->select(selection, QItemSelectionModel::Select);

    if(fSumColumns.count() > 0) {
        sumOfColumns(fSumColumns, fSumValues);
        emit newSum(fSumColumns, fSumValues);
    }
}

void TableModel::appendRow(const QList<QVariant>& values)
{
    int row = fRows.count() - 1;

    if(row < 0) {
        row = 0;
    }

    beginInsertRows(QModelIndex(), row, row);
    fDD.fDbRows.append(values);
    fRows.append(fDD.fDbRows.count() - 1);
    endInsertRows();
}

void TableModel::insertRow(int row, const QList<QVariant>& values)
{
    beginInsertRows(QModelIndex(), row, row);
    fDD.fDbRows.insert(row, values);
    fRows.append(fDD.fDbRows.count() - 1);
    endInsertRows();
}

void TableModel::removeRow(int row)
{
    if(row < 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    int r = fRows.at(row);

    for(int i = row + 1; i < fRows.count(); i++) {
        fRows[i] --;
    }

    fRows.removeAt(row);
    fDD.fDbRows.removeAt(r);
}

void TableModel::updateRowValues(int row, const QList<QVariant>& values)
{
    int realRow = fRows.at(row);

    for(int i = 0; i < fColumns.count(); i++) {
        fDD.fDbRows[realRow][i] = values.at(i);
    }

    emit dataChanged(createIndex(row, 0), createIndex(row, fColumns.count() - 1));
}

void TableModel::uniqueValuesForColumn(int column, QSet<QString>& values)
{
    if(fDD.fDbRows.count() == 0) {
        return;
    }

    if(column > fDD.fDbRows.at(0).count()) {
        return;
    }

    for(int i = 0; i < fDD.fDbRows.count(); i++) {
        values.insert(fDD.fDbRows.at(i).at(column).toString());
    }
}

void TableModel::sumOfColumns(const QList<int> columns, QList<double>& out)
{
    out.clear();

    if(columns.length() == 0) {
        return;
    }

    int count = columns.count();
    double* values = new double[count];

    for(int i = 0; i < count; i++) {
        values[i] = 0.000;
    }

    foreach(int r, fRows) {
        for(int i = 0; i < count; i++) {
            values[i] += fDD.fDbRows.at(r).at(columns.at(i)).toDouble();
        }
    }

    for(int i = 0; i < count; i++) {
        out << values[i];
    }

    fSumColumns  = columns;
    fSumValues = out;
    delete [] values;
}

void TableModel::insertSubTotals(int colName, const QList<int>& totalCols)
{
    if(totalCols.count() == 0) {
        return;
    }

    QList<QVariant> emptyRow;

    for(int i = 0; i < columnCount(); i++) {
        emptyRow << QVariant();
    }

    double* totals = new double[totalCols.count()];

    for(int i = 0; i < totalCols.count(); i++) {
        totals[i] = 0.000;
    }

    QString currName;
    int r = 0;

    while(r < fDD.fDbRows.count()) {
        if(r == 0) {
            currName = data(r, colName).toString();
        } else {
            if(data(r, colName).toString().compare(currName) != 0) {
                QList<QVariant> newrow = emptyRow;
                newrow[0] = tr("SUBTOTAL") + " " + currName;

                for(int c = 0; c < totalCols.count(); c++) {
                    newrow[totalCols.at(c)] = totals[c];
                }

                insertRow(r, newrow);
                fTableView->setSpan(r, 0, 1, totalCols[0]);
                r++;
                insertRow(r, emptyRow);
                fTableView->setSpan(r, 0, 1, columnCount());
                r++;
                currName = data(r, colName).toString();

                for(int i = 0; i < totalCols.count(); i++) {
                    totals[i] = 0.000;
                }
            }
        }

        for(int c = 0; c < totalCols.count(); c++) {
            totals[c] += data(r, totalCols.at(c), Qt::EditRole).toDouble();
        }

        r++;
    }

    if(r > 0) {
        QList<QVariant> newrow = emptyRow;
        newrow[0] = tr("Subtotal") + " " + currName;

        for(int c = 0; c < totalCols.count(); c++) {
            newrow[totalCols.at(c)] = totals[c];
        }

        insertRow(r, newrow);
        fTableView->setSpan(r, 0, 1, totalCols[0]);
    }

    delete [] totals;
}

Column::Column(int width, const QString &fieldName, const QString &title)
{
    fIndex = 0;
    fWidth = width;
    fFieldName = fieldName;
    fTitle = title;
}

Column::~Column()
{
}
