#ifndef ECOLORLINEEDIT_H
#define ECOLORLINEEDIT_H

#include "eqlineedit.h"

class EColorLineEdit : public EQLineEdit
{
    Q_OBJECT
    Q_PROPERTY (QString Field READ getField WRITE setField)
public:
    EColorLineEdit(QWidget *parent = 0);
    QString getField();
    void setField(const QString &name);
    QColor color();
private:
    QString fField;
private slots:
    void colorChanged(const QString &arg);
};

#endif // ECOLORLINEEDIT_H
