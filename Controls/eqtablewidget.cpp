#include "eqtablewidget.h"
#include "utils.h"
#include <QDebug>
#include <QItemSelectionModel>
#include <QStyledItemDelegate>

class TableDelegate : public QStyledItemDelegate {
    QLocale loc;
public:
    QString displayText(const QVariant &value, const QLocale &locale) const {
        Q_UNUSED(locale)
        switch (value.type()) {
        case QVariant::Double: {
            QString v = loc.toString(value.toDouble(), 'f', 2);
            v.remove(QRegExp("\\.0+$")).remove(QRegExp("\\.$"));
            return v;
        }
        case QVariant::Date:
            return value.toDate().toString(def_date_format);
        case QVariant::DateTime:
            return value.toDateTime().toString(def_date_time_format);
        default:
            return value.toString();
        }
    }
};

EQTableWidget::EQTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setItemDelegate(new TableDelegate());
}

void EQTableWidget::setValue(int row, int column, const QVariant &data, int role)
{
    item(row, column)->setData(role, data);
}

double EQTableWidget::sumOfColumn(int column)
{
    double result = 0;
    for (int i = 0; i < rowCount(); i++) {
        result += toDouble(i, column);
    }
    return result;
}

double EQTableWidget::sumOfColumn(int column, int columnCond, int cond)
{
    double result = 0;
    for (int i = 0; i < rowCount(); i++) {
        if (toInt(i, columnCond) == cond) {
            result += toDouble(i, column);
        }
    }
    return result;
}

QString EQTableWidget::toString(int row, int column)
{
    QLocale loc;
    QTableWidgetItem *i = item(row, column);
    QVariant v = i->data(Qt::EditRole);
    switch (v.type()) {
    case QVariant::Double:
        return loc.toString(v.toDouble(), 'f', 0);
    case QVariant::Date:
        return v.toDate().toString(def_date_format);
    case QVariant::DateTime:
        return v.toDateTime().toString(def_date_time_format);
    default:
        return v.toString();
    }
}

void EQTableWidget::setItem(int row, int column, QTableWidgetItem *i)
{
    i->setData(Qt::EditRole, i->text());
    QTableWidget::setItem(row, column, i);
}

void EQTableWidget::setItemWithValue(int row, int column, const QVariant &display, const QVariant &user)
{
    setItem(row, column, Utils::tableItem(display, user));
}

void EQTableWidget::addButton(int row, int column, const char *slot, QObject *receiver, const QIcon &icon)
{
    EPushButton *b = new EPushButton();
    b->setTag(row);
    b->setIcon(icon);
    //b->setMaximumSize(25, 25);
    //b->setMinimumSize(25, 25);
    connect(b, SIGNAL(clickedWithTag(int)), receiver, slot);
    setCellWidget(row, column, b);
}

EPushButton *EQTableWidget::button(int row, int column)
{
    return static_cast<EPushButton*>(cellWidget(row, column));
}

EQCheckBox *EQTableWidget::addCheckBox(int row, int column)
{
    EQCheckBox *check = new EQCheckBox();
    setCellWidget(row, column, check);
    return check;
}

EQCheckBox *EQTableWidget::checkBox(int row, int column)
{
    return static_cast<EQCheckBox*>(cellWidget(row, column));
}

EQLineEdit *EQTableWidget::addLineEdit(int row, int column, bool valueToItem)
{
    QTableWidgetItem *i = item(row, column);
    if (!i) {
        i = new QTableWidgetItem();
        setItem(row, column, i);
    }
    EQLineEdit *line = new EQLineEdit();
    line->fRow = row;
    line->fColumn = column;
    line->setText(i->text());
    line->setFrame(false);
    setCellWidget(row, column, line);
    if (valueToItem)  {
        connect(line, SIGNAL(textChanged(QString)), this, SLOT(lineEditTextChanged(QString)));
    }
    return line;
}

EQLineEdit *EQTableWidget::lineEdit(int row, int column)
{
    return dynamic_cast<EQLineEdit*>(cellWidget(row, column));
}

EDateEdit *EQTableWidget::addDateEdit(int row, int column, bool valueToItem)
{
    QTableWidgetItem *i = item(row, column);
    if (!i) {
        i = new QTableWidgetItem();
        setItem(row, column, i);
    }
    EDateEdit *d = new EDateEdit();
    d->fRow = row;
    d->fColumn = column;
    d->setFrame(false);
    setCellWidget(row, column, d);
    if (valueToItem) {
        connect(d, SIGNAL(dateChanged(QDate)), this, SLOT(dateEditDateChanged(QDate)));
    }
    return d;
}

EDateEdit *EQTableWidget::dateEdit(int row, int column)
{
    return dynamic_cast<EDateEdit*>(cellWidget(row, column));
}

EQComboBox *EQTableWidget::addComboBox(int row, int column)
{
    QTableWidgetItem *i = item(row, column);
    if (!i) {
        i = new QTableWidgetItem();
        setItem(row, column, i);
    }
    EQComboBox *c = new EQComboBox();
    c->fRow = row;
    c->setFrame(false);
    setCellWidget(row, column, c);
    return c;
}

EQComboBox *EQTableWidget::comboBox(int row, int column)
{
    return dynamic_cast<EQComboBox*>(cellWidget(row, column));
}

int EQTableWidget::addRow()
{
    int row = rowCount();
    setRowCount(row + 1);
    return row;
}

void EQTableWidget::setItemChecked(int row, int column, bool v)
{
    QTableWidgetItem *i = item (row, column);
    i->setCheckState(v ? Qt::Checked : Qt::Unchecked);
}

bool EQTableWidget::itemChecked(int row, int column)
{
    QTableWidgetItem *i = item(row, column);
    return i->checkState() == Qt::Checked;
}

bool EQTableWidget::findWidgetCell(QWidget *find, int &row, int &col)
{
    for (int i = 0; i < columnCount(); i++) {
        for (int j = 0; j < rowCount(); j++) {
            QWidget *w = cellWidget(j, i);
            if (w == find) {
                row = j;
                col = i;
                return true;
            }
        }
    }
    return false;
}

void EQTableWidget::focusOutEvent(QFocusEvent *event)
{
    QTableWidget::focusOutEvent(event);
}

void EQTableWidget::lineEditTextChanged(const QString &text)
{
    EQLineEdit *line = static_cast<EQLineEdit*>(sender());
    item(line->fRow, line->fColumn)->setText(text);
}

void EQTableWidget::dateEditDateChanged(const QDate &date)
{
    EDateEdit *d = static_cast<EDateEdit*>(sender());
    item(d->fRow, d->fColumn)->setText(date.toString(def_date_format));
    item(d->fRow, d->fColumn)->setData(Qt::UserRole, date);
}
