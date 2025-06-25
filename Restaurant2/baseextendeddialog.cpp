#include "baseextendeddialog.h"

BaseDialog* BaseExtendedDialog::fParent = 0;

BaseExtendedDialog::BaseExtendedDialog(QWidget *parent) :
#ifdef QT_DEBUG
    BaseDialog(parent)
#else
    BaseDialog(parent, Qt::FramelessWindowHint)
#endif
{
}

BaseExtendedDialog::~BaseExtendedDialog()
{
}

bool BaseExtendedDialog::event(QEvent *event)
{
    return BaseDialog::event(event);
}
