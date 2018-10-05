#ifndef PPRINTSTOREENTRY_H
#define PPRINTSTOREENTRY_H

#include "basewidget.h"

class PPrintStoreEntry : public BaseWidget
{
    Q_OBJECT
public:
    PPrintStoreEntry(QWidget *parent = 0);
    static void print(const QString &id);
private:
    QString fDockId;
    void printDoc();
};

#endif // PPRINTSTOREENTRY_H
