#ifndef BASEEXTENDEDDIALOG_H
#define BASEEXTENDEDDIALOG_H

#include "basedialog.h"
#include "utils.h"

class BaseExtendedDialog : public BaseDialog {
    Q_OBJECT
public:
    BaseExtendedDialog(QWidget *parent = 0);
    static BaseDialog *fParent;
protected:
};

#endif // BASEEXTENDEDDIALOG_H
