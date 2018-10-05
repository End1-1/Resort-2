#include "baseextendeddialog.h"
#include "cacherights.h"
#include "cacheusers.h"
#include "rlogin.h"
#include "user.h"

BaseDialog *BaseExtendedDialog::fParent = 0;

BaseExtendedDialog::BaseExtendedDialog(QWidget *parent) :
    BaseDialog(parent, Qt::FramelessWindowHint)
{

}
