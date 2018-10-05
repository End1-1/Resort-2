#ifndef DWMAINDESKHINT_H
#define DWMAINDESKHINT_H

#include "base.h"
#include <QDockWidget>

class QTableWidget;

namespace Ui {
class DWMainDeskHint;
}

class DWMainDeskHint : public QDockWidget, public Base
{
    Q_OBJECT
public:
    explicit DWMainDeskHint(QWidget *parent = 0);
    ~DWMainDeskHint();
    void commonFilter(const QString &filter, int col = -1);
    void filterEqual(const QStringList &values, int col = -1);
    void checkInFilter();
    void checkOutFilter();
    void airFilter();
    void hide();
    QTableWidget *tableWidget();
private:
    Ui::DWMainDeskHint *ui;
    bool fCheckInFilter;
private slots:
    void tblHeaderSectionClicked(int logicalIndex);
    void thisVisibilityChanged(bool v);
    void on_btnCheckIn_clicked();
};

#endif // DWMAINDESKHINT_H
