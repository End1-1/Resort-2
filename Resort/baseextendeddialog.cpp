#include "baseextendeddialog.h"

BaseExtendedDialog::BaseExtendedDialog(QWidget *parent) :
    BaseDialog(parent)
{
    fTrackControl = nullptr;
}

BaseExtendedDialog::~BaseExtendedDialog()
{
    if (fTrackControl) {
        delete fTrackControl;
    }
}

bool BaseExtendedDialog::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        switch (ke->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            focusNextChild();
            event->ignore();
            return false;
        }
    }
    return BaseDialog::event(event);
}
