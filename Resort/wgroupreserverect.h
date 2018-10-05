#ifndef WGROUPRESERVERECT_H
#define WGROUPRESERVERECT_H

#include "basewidget.h"

namespace Ui {
class WGroupReserveRect;
}

class WGroupReserveRect : public BaseWidget
{
    Q_OBJECT

public:
    explicit WGroupReserveRect(QWidget *parent = 0);
    ~WGroupReserveRect();
    void setRoomName(const QString &code, const QString &name);
    bool checked();
    void uncheck();
    QString code();
    virtual void setEnabled(bool v);
private slots:
    void on_chSelected_clicked(bool checked);

private:
    Ui::WGroupReserveRect *ui;
    QString fCode;
signals:
    checkClicked(bool v);
};

#endif // WGROUPRESERVERECT_H
