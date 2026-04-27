#ifndef DLGCONNECTTOSERVER_H
#define DLGCONNECTTOSERVER_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgConnectToServer;
}

class DlgConnectToServer : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgConnectToServer(QWidget *parent = nullptr);
    ~DlgConnectToServer();
    void loadFromConfig();

private:
    Ui::DlgConnectToServer *ui;
};

#endif // DLGCONNECTTOSERVER_H
