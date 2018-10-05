#ifndef RECUSTOMACTION_H
#define RECUSTOMACTION_H

#include "baseextendeddialog.h"

class WReportGrid;

class RECustomAction : public BaseExtendedDialog
{
    Q_OBJECT
public:
    RECustomAction(QWidget *parent);
    virtual void setup(const QString &icon, const QString &text);
protected:
    WReportGrid *fParent;
protected slots:
    virtual void handleValues(const QList<QVariant> &out) = 0;
private slots:
    virtual void clicked();
signals:
    void processValues(const QList<QVariant> &out);
};

#endif // RECUSTOMACTION_H
