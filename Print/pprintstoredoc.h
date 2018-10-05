#ifndef PPRINTSTOREDOC_H
#define PPRINTSTOREDOC_H

#include "basewidget.h"

class PPrintStoreDoc : public BaseWidget
{
    Q_OBJECT

public:
    PPrintStoreDoc(QWidget *parent = 0);
    static void print(const QString &id);

private:
    QString fDocId;
    void printDoc();
};

#endif // PPRINTSTOREDOC_H
