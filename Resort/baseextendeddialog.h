#ifndef BASEEXTENDEDDIALOG_H
#define BASEEXTENDEDDIALOG_H

#include "basedialog.h"
#include "message.h"
#include "epushbutton.h"
#include "trackcontrol.h"
#include "eqtablewidget.h"
#include "utils.h"
#include <QHostInfo>

class BaseExtendedDialog : public BaseDialog
{
    Q_OBJECT
public:
    BaseExtendedDialog(QWidget *parent = nullptr);
    ~BaseExtendedDialog();

protected:
    virtual bool event(QEvent *event);

protected:
    TrackControl *fTrackControl;
};

#endif // BASEEXTENDEDDIALOG_H
