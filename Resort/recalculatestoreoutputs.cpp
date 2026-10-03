#include "recalculatestoreoutputs.h"
#include "ui_recalculatestoreoutputs.h"
#include "storeoutput.h"
#include "baseorder.h"
#include "message.h"
#ifdef RESORT_AUDIT_LOG
#include "resortlog.h"
#include <QJsonObject>
#endif
#include <QCloseEvent>

RecalculateStoreOutputs::RecalculateStoreOutputs(QSet<int> &idlist, const QString &runId, QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::RecalculateStoreOutputs),
    ids(idlist),
    fRunId(runId)
{
    ui->setupUi(this);
    stop = false;
    fFinished = false;
    connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    fTimer.start(1000);
}

RecalculateStoreOutputs::~RecalculateStoreOutputs()
{
    delete ui;
}

int RecalculateStoreOutputs::exec()
{
    return QDialog::exec();
}

void RecalculateStoreOutputs::closeEvent(QCloseEvent *event)
{
    if(!fFinished) {
        stop = true;
    }
    BaseDialog::closeEvent(event);
}

void RecalculateStoreOutputs::timeout()
{
    StoreOutput so(fDb, 0);
    BaseOrder bo(0);
    fTimer.stop();
    ui->l1->setText(tr("Rollback"));
    int rollbackDone = 0;

    for(int order : ids) {
        if(order == 0) {
            message_error(tr("Invalid order id: 0"));
#ifdef RESORT_AUDIT_LOG
            if(!fRunId.isEmpty()) {
                QJsonObject data;
                data[QStringLiteral("order_count")] = ids.count();
                data[QStringLiteral("rollback_done")] = rollbackDone;
                data[QStringLiteral("reason")] = QStringLiteral("invalid_order_id");
                ResortLog::logJobEvent(fRunId, QStringLiteral("store_recalc_aborted"), data);
            }
#endif
            fFinished = true;
            reject();
            return;
        }

        if(stop) {
            break;
        }

        so.rollbackSale(fDb, order);
        ++rollbackDone;

        if(rollbackDone % 20 == 0) {
            ui->l2->setText(QString("%1 of %2").arg(rollbackDone).arg(ids.count()));
            qApp->processEvents();
        }
    }

    ui->l1->setText(tr("Write"));
    int writeDone = 0;

    for(int order : qAsConst(ids)) {
        if(stop) {
            break;
        }

        bo.calculateOutput(fDb, order);
        ++writeDone;

        if(writeDone % 20 == 0) {
            ui->l2->setText(QString("%1 of %2").arg(writeDone).arg(ids.count()));
            qApp->processEvents();
        }
    }

    const bool fullSuccess = !stop
            && rollbackDone == ids.count()
            && writeDone == ids.count();

#ifdef RESORT_AUDIT_LOG
    if(!fRunId.isEmpty()) {
        QJsonObject data;
        data[QStringLiteral("order_count")] = ids.count();
        data[QStringLiteral("rollback_done")] = rollbackDone;
        data[QStringLiteral("write_done")] = writeDone;
        data[QStringLiteral("cancelled")] = stop;
        ResortLog::logJobEvent(fRunId,
                               fullSuccess ? QStringLiteral("store_recalc_done")
                                           : QStringLiteral("store_recalc_aborted"),
                               data);
    }
#endif

    fFinished = true;

    if(fullSuccess) {
        accept();
    } else {
        reject();
    }
}

void RecalculateStoreOutputs::on_btnCancel_clicked()
{
    stop = true;
}
