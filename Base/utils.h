#ifndef UTILS_H
#define UTILS_H

#include "defines.h"
#include "edateedit.h"
#include <QTableWidget>
#include <QList>
#include <QDate>
#include <QSettings>

#define float_str(value, f) QString::number(value, 'f', f).remove(QRegExp("\\.0+$")).remove(QRegExp("\\.$"))
#define float_equal(value1, value2) (abs(value1 - value2) < 0.01 ? true : false)
#define float_greaterOrEqual(value1, value2) (value1 - value2 >= 0.01 ? true : false)
#define float_printout(x) QLocale().toString(x, 'f', 2)

namespace Utils {
    void initNumbersWords();
    QString numberToWords(int num);
    QString separateForQuote(const QString &text);
    void dateEditPrev(EDateEdit *d1, EDateEdit *d2);
    void dateEditNext(EDateEdit *d1, EDateEdit *d2);
    void tableRowUp(QTableWidget *tw);
    void tableRowDown(QTableWidget *tw);
    void tableSetColumnWidths(QTableWidget *tw, int count, ...);
    void tableSetHeaderCaptions(QTableWidget *tw, int count, ...);
    void tableAppendRowData(QTableWidget *tw, const QList<QVariant> &data, int role);
    void tableAppendRowData(QTableWidget *tw, int count, ...);
    void tableSetRowDataWithOffcet(QTableWidget *tw, int row, const QList<QVariant> &data, bool createItem, int role, int colOffcet);
    inline void tableSetRowData(QTableWidget *tw, int row, const QList<QVariant> &data, bool createItem, int role) {tableSetRowDataWithOffcet(tw, row, data, createItem, role, 0);}
    int tableGetCurrentRowData(QTableWidget *tw, QList<QVariant> &data);
    void setupTableFullColumnWidth(QTableWidget *tw, int colWidth, int elements);
    QString stringListToString(const QStringList &lst);
    QString intListToString(const QList<int> &lst);
    QString getVersionString(QString fName);
    QString hostName();
    void fillTableWithData(QTableWidget *tw, QList<QList<QVariant> > &data, bool append = false);
    inline QString variantToString(const QVariant &v) {
        switch (v.type()) {
        case QVariant::Double:
            return float_str(v.toDouble(), 2);
        case QVariant::Date:
            return v.toDate().toString(def_date_format);
        case QVariant::DateTime:
            return v.toDateTime().toString(def_date_time_format);
        case QVariant::Time:
            return v.toTime().toString(def_time_format);
        default:
            return v.toString();
        }
    }

    inline QTableWidgetItem *tableItem(const QVariant &display, const QVariant &userData = QVariant()) {
        //QString displayStr = variantToString(display);
        QTableWidgetItem *i = new QTableWidgetItem();
        i->setData(Qt::EditRole, display);
        i->setData(Qt::UserRole, userData);
        return i;
    }

    template <typename T>
    void fillTableWithElements(QTableWidget *tw, QList<T> &list, int role = Qt::UserRole) {
        int row = 0, col = 0;
        for (typename QList<T>::const_iterator it = list.begin(); it != list.end(); it++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(role, qVariantFromValue(*it));
            tw->setItem(row, col++, item);
            if (col == tw->columnCount()) {
                col = 0;
                row++;
            }
        }
    }

    double countVATAmount(double amount, int mode);
}

extern QMap<int, QString> l1;
extern QMap<int, QString> l2;
extern QMap<int, QString> l3;
extern QSettings __s;

#endif // UTILS_H
