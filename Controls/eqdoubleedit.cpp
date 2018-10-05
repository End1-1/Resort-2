#include "eqdoubleedit.h"
#include "utils.h"
#include <QDoubleValidator>

EQDoubleEdit::EQDoubleEdit(QWidget *parent) :
    EQLineEdit(parent)
{
    fLimit = 12;
    fDecimal = 0;
    setVal();
}

int EQDoubleEdit::getUpLimit()
{
    return fLimit;
}

void EQDoubleEdit::setUpLimit(int limit)
{
    fLimit = limit;
}

int EQDoubleEdit::getDecimal()
{
    return fDecimal;
}

void EQDoubleEdit::setDecimal(int decimal)
{
    fDecimal = decimal;
}

void EQDoubleEdit::setText(double value)
{
    EQLineEdit::setText(float_str(value, 2));
}

void EQDoubleEdit::setVal()
{
    setValidator(new QDoubleValidator(0, fLimit, fDecimal));
}
