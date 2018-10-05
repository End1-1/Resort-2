#include "rchangehall.h"
#include "ui_rchangehall.h"

RChangeHall::RChangeHall(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RChangeHall)
{
    ui->setupUi(this);
    showFullScreen();
    qApp->processEvents();
}

RChangeHall::~RChangeHall()
{
    delete ui;
}

void RChangeHall::setup(QList<HallStruct*> &hall)
{
    Utils::setupTableFullColumnWidth(ui->tblHall, 150, hall.count());
    int row = 0, col = 0;
    for (QList<HallStruct*>::const_iterator it = hall.begin(); it != hall.end(); it++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::EditRole, (*it)->fName);
        item->setData(Qt::UserRole, (*it)->fId);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tblHall->setItem(row, col++, item);
        if (col == ui->tblHall->columnCount()) {
            col = 0;
            row++;
        }
    }
}

int RChangeHall::hall()
{
    return fHall;
}


void RChangeHall::on_tblHall_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    fHall = index.data(Qt::UserRole).toInt();
    if (fHall == 0) {
        return;
    }
    accept();
}

void RChangeHall::on_btnCancel_clicked()
{
    reject();
}
