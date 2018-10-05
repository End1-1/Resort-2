#ifndef EQDATEEDIT_H
#define EQDATEEDIT_H

#include <QDateEdit>

class EQDateEdit : public QDateEdit
{
    Q_OBJECT
    Q_PROPERTY(QString Field READ getField WRITE setField)
public:
    EQDateEdit(QWidget *parent = 0);
    QString getField();
    void setField(const QString &field);
    void setBgColor(const QColor &color);
    QString toString();
    int fRow;
    int fColumn;
private:
    QString fField;
};

#endif // EQDATEEDIT_H
