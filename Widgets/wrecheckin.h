#ifndef WRECHECKIN_H
#define WRECHECKIN_H

#include "basewidget.h"

namespace Ui {
class WReCheckIn;
}

class WReCheckIn : public BaseWidget
{
    Q_OBJECT

public:
    explicit WReCheckIn(QWidget *parent = 0);
    ~WReCheckIn();
    virtual void setupTab();
private:
    Ui::WReCheckIn *ui;
private slots:
    void btnClick(int tag);
    void on_btnRefresh_clicked();
};

#endif // WRECHECKIN_H
