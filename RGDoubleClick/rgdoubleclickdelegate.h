#ifndef RGDOUBLECLICKDELEGATE_H
#define RGDOUBLECLICKDELEGATE_H

#include "tablemodel.h"

class RGDoubleClickDelegate : public QObject
{
    Q_OBJECT
public:
    explicit RGDoubleClickDelegate(QObject *parent = 0);
    virtual void doubleClick(TableModel *t, int row) = 0;
signals:

public slots:
};

#endif // RGDOUBLECLICKDELEGATE_H
