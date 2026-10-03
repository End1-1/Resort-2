#ifndef FRESORTLOG_H
#define FRESORTLOG_H

#include "wfilterbase.h"
#include "dwselectorusers.h"

namespace Ui {
class FResortLog;
}

class FResortLog : public WFilterBase
{
    Q_OBJECT
public:
    explicit FResortLog(QWidget *parent = 0);
    ~FResortLog();
    void apply(WReportGrid *rg) override;
    QWidget *firstElement() override;
    QString reportTitle() override;

private slots:
    void user(CI_User *c);
    void doubleClickOnRow(const QList<QVariant> &row);

private:
    Ui::FResortLog *ui;
    DWSelectorUsers *fDockUser;
};

#endif // FRESORTLOG_H
