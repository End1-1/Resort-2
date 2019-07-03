#include "utils.h"
#include <windows.h>
#include <QHeaderView>
#include <cstdarg>

QMap<int, QString> l1;
QMap<int, QString> l2;
QMap<int, QString> l3;
QSettings __s("SmartHotel", "SmartHotel");

namespace Utils {

void tableAppendRowData(QTableWidget *tw, const QList<QVariant> &data, int role)
{
    int rowCount = tw->rowCount();
    tw->setRowCount(rowCount + 1);
    for (int i = 0; i < tw->columnCount(); i++)
        tw->setItem(rowCount, i, new QTableWidgetItem());
    Utils::tableSetRowData(tw, rowCount, data, false, role);
}

int tableGetCurrentRowData(QTableWidget *tw, QList<QVariant> &data)
{
    QModelIndexList rowList = tw->selectionModel()->selectedRows();
    if (rowList.count() == 0)
        return -1;
    int row = rowList.at(0).row();
    for (int i = 0; i < tw->columnCount(); i++)
        data.append(tw->item(row, i)->data(Qt::EditRole));
    return row;
}

QString getVersionString(QString fName)
{
// first of all, GetFileVersionInfoSize
    DWORD dwHandle;
    DWORD dwLen = GetFileVersionInfoSize(fName.toStdWString().c_str(), &dwHandle);

    // GetFileVersionInfo
    BYTE *lpData = new BYTE[dwLen];
    if(!GetFileVersionInfo(fName.toStdWString().c_str(), dwHandle, dwLen, lpData)) {
        delete[] lpData;
        return "";
    }

    // VerQueryValue
    VS_FIXEDFILEINFO *lpBuffer = NULL;
    UINT uLen;
    if(VerQueryValue(lpData, QString("\\").toStdWString().c_str(), (LPVOID*)&lpBuffer, &uLen)) {
        return
            QString::number((lpBuffer->dwFileVersionMS >> 16) & 0xffff) + "." +
            QString::number((lpBuffer->dwFileVersionMS) & 0xffff ) + "." +
            QString::number((lpBuffer->dwFileVersionLS >> 16 ) & 0xffff ) + "." +
            QString::number((lpBuffer->dwFileVersionLS) & 0xffff );
    }
    return "";
}

void setupTableFullColumnWidth(QTableWidget *tw, int colWidth, int elements)
{
    tw->setColumnCount(0);
    tw->setRowCount(0);
    int colCount = (tw->width() - 20) / colWidth;
    if (colCount == 0) {
        if (elements == 0) {
            return;
        } else {
            colCount = 1;
        }
    }
    int delta = tw->width() - (colCount * colWidth) - 15;
    colWidth += delta / colCount;
    tw->horizontalHeader()->setDefaultSectionSize(colWidth);
    tw->setColumnCount(colCount);

    int rowCount = elements / colCount;
    if (elements % colCount > 0) {
        rowCount++;
    }
    tw->setRowCount(rowCount);
}

QString hostName()
{
    return "unknown";
}

void fillTableWithData(QTableWidget *tw, QList<QList<QVariant> > &data, bool append)
{
    int start = 0;
    if (append) {
        start = tw->rowCount();
        tw->setRowCount(tw->rowCount() + data.count());
    } else {
        tw->setRowCount(data.count());
    }
    for (int i = start, rowCount = tw->rowCount(); i < rowCount; i++) {
        for (int j = 0, colCount = tw->columnCount(); j < colCount; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::EditRole, data.at(i - start).at(j));
            tw->setItem(i, j, item);
        }
    }
}

void tableRowDown(QTableWidget *tw)
{
    int row = tw->currentRow();
    bool stop = false;
    do {
        row ++;
        if (row >= tw->rowCount() - 1) {
            row = tw->rowCount() - 1;
            stop = true;
        }
        if (tw->isRowHidden(row)) {
            continue;
        }
        tw->selectRow(row);
        stop = true;
    } while (!stop);
}

void tableRowUp(QTableWidget *tw)
{
    int row = tw->currentRow();
    bool stop = false;
    do {
        row --;
        if (row <= 0) {
            row = 0;
            stop = true;
        }
        if (tw->isRowHidden(row)) {
            continue;
        }
        tw->selectRow(row);
        stop = true;
    } while (!stop);
}

void tableSetColumnWidths(QTableWidget *tw, int count, ...)
{
    tw->setColumnCount(count);
    va_list vl;
    va_start(vl, count);
    for (int i = 0; i < count; i++) {
        tw->setColumnWidth(i, va_arg(vl, int));
    }
    va_end(vl);
}

void tableSetHeaderCaptions(QTableWidget *tw, int count, ...)
{
    QStringList header;
    va_list vl;
    va_start(vl, count);
    for (int i = 0; i < count; i++) {
        header << QString(va_arg(vl, QString));
    }
    va_end(vl);
    tw->setHorizontalHeaderLabels(header);
}

double countVATAmount(double amount, int mode)
{
    double vat = 0;
    switch (mode) {
    case VAT_INCLUDED: {
        vat = amount - (amount / ((def_vat / 100) + 1));
        break;
    }
    case VAT_WITHOUT: {
        vat = amount * (def_vat / 100);
        break;
    }
    case VAT_NOVAT:
        break;
    }
    return vat;
}

void tableAppendRowData(QTableWidget *tw, int count, ...)
{
    va_list vl;
    va_start(vl, count);
    int row = tw->rowCount();
    tw->setRowCount(row + 1);
    for (int i = 0; i < count; i++) {
        tw->setItem(row, i, new QTableWidgetItem(QString(va_arg(vl, QString))));
    }
}

void tableSetRowDataWithOffcet(QTableWidget *tw, int row, const QList<QVariant> &data, bool createItem, int role, int colOffcet)
{
    for (int i = 0; i < data.count(); i++) {
        if (createItem) {
            tw->setItem(row, i + colOffcet, new QTableWidgetItem());
        }
        tw->item(row, i + colOffcet)->setData(role, data[i]);
    }
}

void initNumbersWords() {
    l1[0] = "ZERO";
    l1[1] = "ONE";
    l1[2] = "TWO";
    l1[3] = "THREE";
    l1[4] = "FOUR";
    l1[5] = "FIVE";
    l1[6] = "SIX";
    l1[7] = "SEVEN";
    l1[8] = "EIGHT";
    l1[9] = "NINE";
    l1[10] = "TEN";
    l1[11] = "ELEVEN";
    l1[12] = "TWELVE";
    l1[13] = "THIRTEEN";
    l1[14] = "FOURTEEN";
    l1[15] = "FIFTEEN";
    l1[16] = "SIXTEEN";
    l1[17] = "SEVENTEEN";
    l1[18] = "EIGHTEEN";
    l1[19] = "NINETEEN";
    l2[2] = "TWENTY";
    l2[3] = "THIRTY";
    l2[4] = "FORTY";
    l2[5] = "FIFTY";
    l2[6] = "SIXTY";
    l2[7] = "SEVENTY";
    l2[8] = "EIGHTY";
    l2[9] = "NINETY";
    l3[0] = "HUNDRED";
    l3[1] = "THOUSAND";
    l3[2] = "MILLION";
    l3[3] = "BILLION";
}

QString numberToWordEn_l1(int num)
{
    return l1[num];
}

QString numberToWordEn_l2(int num) {
    int div = (num - (num % 10)) / 10;
    int mod = num % 10;
    QString result = l2[div];
    if (mod > 0) {
        result += "-" + l1[mod];
    }
    return result;
}

QString numberToWordEn_l3(int num) {
    QString result;
    int h = (num - (num % 100)) / 100;
    result = l1[h] + " " + l3[0];
    int t = num % (h * 100);
    if (t > 0) {
        if (t < 20) {
            result += " " + numberToWordEn_l1(t);
        } else {
            result += " " + numberToWordEn_l2(t);
        }
    }
    return result;
}

QString numberToWords(int num)
{
   QString sign;
   if (num < 0) {
       sign = QObject::tr("MINUS ");
       num *= -1;
   }
   if (num < 20) {
       return sign + numberToWordEn_l1(num);
   }
   if (num < 100) {
       return sign + numberToWordEn_l2(num);
   }
   if (num < 1000) {
       return sign + numberToWordEn_l3(num);
   }
   if (num > 999999999) {
       int h = num / 1000000000;
       QString result = numberToWords(h) + " BILLION";
       h = num / 1000;
       if (h > 0) {
           result += " " + numberToWords(h);
       }
       return sign + result;
   }
   if (num > 999999) {
       int h = num / 1000000;
       QString result = numberToWords(h) + " MILLION";
       h = num - (h * 1000000);
       if (h > 0) {
           result += " " + numberToWords(h);
       }
       return sign + result;
   }
   int h = num / 1000;
   QString result = numberToWords(h) + " THOUSAND";
   h = num % 1000;
   if (h > 0) {
       result += " " + numberToWords(h);
   }
   return sign + result;
}

QString stringListToString(const QStringList &lst)
{
    bool first = true;
    QString result;
    for (QStringList::const_iterator it = lst.begin(); it != lst.end(); it++) {
        if (first) {
            first = false;
        } else {
            result += ",";
        }
        result += *it;
    }
    return result;
}

QString intListToString(const QList<int> &lst)
{
    QStringList l;
    foreach (int v, lst) {
        l << QString::number(v);
    }
    return stringListToString(l);
}

QString separateForQuote(const QString &text)
{
    QStringList l = text.split(",");
    QString result;
    for (int i = 0; i < l.count(); i++) {
        if (i > 0) {
            result += ",";
        }
        result += "'" + l.at(i) + "'";
    }
    return result;
}

void dateEditPrev(EDateEdit *d1, EDateEdit *d2)
{
    if (d1) {
        d1->setDate(d1->date().addDays(-1));
    }
    if (d2) {
        d2->setDate(d2->date().addDays(-1));
    }
}

void dateEditNext(EDateEdit *d1, EDateEdit *d2)
{
    if (d1) {
        d1->setDate(d1->date().addDays(1));
    }
    if (d2) {
        d2->setDate(d2->date().addDays(1));
    }
}

}
