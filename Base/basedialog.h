#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include "base.h"
#include <QDialog>
#include <QKeyEvent>

class BaseDialog : public QDialog, public Base
{
public:
    BaseDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void returnPressed();
    virtual void escapePressed();
};

#endif // BASEDIALOG_H
