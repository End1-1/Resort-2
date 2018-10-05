#include "ecombomonth.h"
#include <QCryptographicHash>

EComboMonth::EComboMonth(QWidget *parent) :
    EComboBoxCompleter(parent)
{

}

void EComboMonth::setData()
{
    clear();
    QMap<QString, QString> &map = fCache[QCryptographicHash::hash(QString(fSql + fDatabaseName).toLatin1(), QCryptographicHash::Md5).toHex()];
    QMap<int, QString> invertMap;
    foreach (QString k, map.keys()) {
        invertMap.insert(map[k].toInt(), k);
    }
    foreach (int k, invertMap.keys()) {
        addItem(invertMap[k], k);
    }

    setCurrentIndex(-1);
}
