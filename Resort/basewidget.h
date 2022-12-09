#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include "base.h"
#include "mainwindow.h"
#include "message.h"
#include "trackcontrol.h"
#include "cacherights.h"
#include "pprintscene.h"
#include "dlgraiseuser.h"
#include <QWidget>
#include <QTabWidget>

extern MainWindow *fMainWindow;

template<class T>
T *addTab() {
    T *t = new T();
    fMainWindow->addTabWidget(t);
    fMainWindow->expandTab();
    t->setup();
    return t;
}

class BaseWidget : public QWidget, public Base
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);
    ~BaseWidget();
    void setTab(QTabWidget *tabWidget, int index);
    virtual void setup();
    virtual QString title();
    virtual bool canClose();
    virtual void closeEvent(QCloseEvent *event);
    virtual void handleBroadcast(const QMap<QString, QVariant> &data);
    virtual bool event(QEvent *event);
    virtual void setupTabTextAndIcon(const QString &text, const QString &iconName);
    QString valueForWidget(const QString &name);
protected:
    QTabWidget *fTabWidget;
    int fTabIndex;
    TrackControl *fTrackControl;
    virtual void setupTab();
    virtual void finalPrint(PPrintScene *ps, int top);
};

#endif // BASEWIDGET_H
