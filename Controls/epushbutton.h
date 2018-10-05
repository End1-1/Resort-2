#ifndef EPUSHBUTTON_H
#define EPUSHBUTTON_H

#include <QPushButton>

class EPushButton : public QPushButton
{
    Q_OBJECT
public:
    EPushButton(QWidget *parent = 0);
    void setTag(int tag);
    int fRow;
    int fColumn;
    QString fId;
private:
    int fTag;
private slots:
    void clicked();
signals:
    void clickedWithTag(int tag);
    void clickedWithId(const QString &id);
};

#endif // EPUSHBUTTON_H
