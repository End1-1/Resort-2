#ifndef ERADIOBUTTON_H
#define ERADIOBUTTON_H

#include <QRadioButton>
#include <QMap>
#include <QVariant>

class ERadioButton : public QRadioButton
{
public:
    ERadioButton(QWidget *parent =0);
    QMap<QString, QVariant> fData;
};

#endif // ERADIOBUTTON_H
