#ifndef EQCOMBOBOX_H
#define EQCOMBOBOX_H

#include <QComboBox>

class EQComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY (int Cache READ getCache WRITE setCache)
    Q_PROPERTY (QString Field READ getField WRITE setField)
public:
    EQComboBox(QWidget *parent = 0);
    void setIndexForData(const QVariant &data);
    int getCache();
    int asInt();
    void setCache(int cache);
    QString getField();
    void setField(const QString &name);
    int fRow;
protected:
    virtual void wheelEvent(QWheelEvent *e);
private:
    int fCache;
    QString fField;
};

#endif // EQCOMBOBOX_H
