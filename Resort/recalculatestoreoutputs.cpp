#include "recalculatestoreoutputs.h"
#include "ui_recalculatestoreoutputs.h"
#include "storeoutput.h"
#include "baseorder.h"

RecalculateStoreOutputs::RecalculateStoreOutputs(QSet<int> &idlist, QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::RecalculateStoreOutputs),
    ids(idlist)
{
    ui->setupUi(this);
    stop = false;
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

void RecalculateStoreOutputs::timeout()
{
    StoreOutput so(fDb, 0);
    BaseOrder bo(0);

    fTimer.stop();
    ui->l1->setText(tr("Rollback"));
    int counter = 0;
    for (int order: ids) {
        if (stop) {
            break;
        }
        so.rollbackSale(fDb, order);
        if (++counter % 20 == 0) {
            ui->l2->setText(QString("%1 of %2").arg(counter).arg(ids.count()));
            qApp->processEvents();
        }
    }
    ui->l1->setText(tr("Write"));
    counter = 0;
    for(int order: ids) {
        if (stop) {
            break;
        }
        bo.calculateOutput(fDb, order);
        if (++counter % 20 == 0) {
            ui->l2->setText(QString("%1 of %2").arg(counter).arg(ids.count()));
            qApp->processEvents();
        }
    }
    accept();
}

void RecalculateStoreOutputs::on_btnCancel_clicked()
{
    stop = true;
}
