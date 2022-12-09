#ifndef WGLOBALDBCONFIG_H
#define WGLOBALDBCONFIG_H

#include "basewidget.h"
#include "dwselectorhall.h"
#include "dwselectorpaymentmode.h"

namespace Ui {
class WGlobalDbConfig;
}

class WGlobalDbConfig : public BaseWidget
{
    Q_OBJECT

public:
    explicit WGlobalDbConfig(QWidget *parent = 0);
    ~WGlobalDbConfig();
protected:
    virtual void setupTab();
private slots:
    void hall(CI_RestHall *c);

    void on_lwHost_clicked(const QModelIndex &index);

    void on_btnSaveRestaurant_clicked();

    void on_btnRefresh_clicked();

    void on_btnSaveAccess_clicked();

    void on_btnSaveDatabases_clicked();

    void on_btnRemoveStation_clicked();

    void on_btnRefreshReset_clicked();

    void on_btnRemoveRest_clicked();

    void on_chShowLogs_clicked(bool checked);

private:
    Ui::WGlobalDbConfig *ui;
    DWSelectorHall *fDockHall;
    void getCompSettings();
    void getAccess();
    void getDatabases();
};

#endif // WGLOBALDBCONFIG_H
