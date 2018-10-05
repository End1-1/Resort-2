#ifndef EQDOUBLEEDIT_H
#define EQDOUBLEEDIT_H

#include "eqlineedit.h"

class EQDoubleEdit : public EQLineEdit
{
    Q_OBJECT
    Q_PROPERTY(int UpLimit READ getUpLimit WRITE setUpLimit)
    Q_PROPERTY(int Decimal READ getDecimal WRITE setDecimal)
public:
    EQDoubleEdit(QWidget *parent = 0);
    int getUpLimit();
    void setUpLimit(int limit);
    int getDecimal();
    void setDecimal(int decimal);
    void setText(double value);
private:
    int fLimit;
    int fDecimal;
    void setVal();
};

#endif // EQDOUBLEEDIT_H
