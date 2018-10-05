#ifndef PPRINTINVOICE_H
#define PPRINTINVOICE_H

#include "basewidget.h"

class PPrintInvoice : BaseWidget
{
    Q_OBJECT
public:
    PPrintInvoice(const QString &id, int side, QWidget *parent = 0);
private:
    QString fId;
    int fSide;
    void previewInvoice();
};

#endif // PPRINTINVOICE_H
