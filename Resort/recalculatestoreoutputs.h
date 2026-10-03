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
    explicit RecalculateStoreOutputs(QSet<int> &idlist, const QString &runId, QWidget *parent = nullptr);
    ~RecalculateStoreOutputs();
    virtual int exec() override;

private slots:
    void timeout();
    void on_btnCancel_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::RecalculateStoreOutputs *ui;
    bool stop;
    bool fFinished;
    QSet<int> &ids;
    QString fRunId;
    QTimer fTimer;
};

#endif // RECALCULATESTOREOUTPUTS_H
