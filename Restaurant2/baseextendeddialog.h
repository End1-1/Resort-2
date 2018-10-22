#ifndef BASEEXTENDEDDIALOG_H
#define BASEEXTENDEDDIALOG_H

#include "basedialog.h"
#include "utils.h"

class BaseExtendedDialog : public BaseDialog {
    Q_OBJECT
public:
    BaseExtendedDialog(QWidget *parent = 0);
    ~BaseExtendedDialog();
    static BaseDialog *fParent;
    virtual bool event(QEvent *event);
protected:
};

#endif // BASEEXTENDEDDIALOG_H
