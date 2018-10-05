#ifndef ROWEDITORDIALOG_H
#define ROWEDITORDIALOG_H

#include "baseextendeddialog.h"
#include "broadcastthread.h"
#include "cacherights.h"

class RowEditorDialog : public BaseExtendedDialog
{
    Q_OBJECT
public:
    RowEditorDialog(QList<QVariant> &values, int trackId, QWidget *parent = 0);
    ~RowEditorDialog();
    virtual void setValues();
    RowEditorDialog &addWidget(QWidget *w, const QString &trackName);
protected:
    bool isNew;
    QString fTable;
    int fCacheId;
    QList<QVariant> &fValues;
    QList<QWidget*> fWidgets;
    QStringList fDataErrors;
    int fTrackId;
    virtual void valuesToWidgets();
    void widgetsToValues();
    virtual void clearWidgets();
    virtual bool saveOnly();
    virtual void save();
    virtual bool isDataCorrect();
    virtual void returnPressed();
    virtual void escapePressed();
};

#endif // ROWEDITORDIALOG_H
