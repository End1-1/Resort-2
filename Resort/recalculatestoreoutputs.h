#ifndef RECALCULATESTOREOUTPUTS_H
#define RECALCULATESTOREOUTPUTS_H

#include "basedialog.h"
#include <QTimer>

namespace Ui {
class RecalculateStoreOutputs;
}

class RecalculateStoreOutputs : public BaseDialog
{
    Q_OBJECT

public:
    explicit RecalculateStoreOutputs(QSet<int> &idlist, QWidget *parent = nullptr);
    ~RecalculateStoreOutputs();
    virtual int exec() override;

private slots:
    void timeout();
    void on_btnCancel_clicked();

private:
    Ui::RecalculateStoreOutputs *ui;
    bool stop;
    QSet<int> &ids;
    QTimer fTimer;
};

#endif // RECALCULATESTOREOUTPUTS_H
