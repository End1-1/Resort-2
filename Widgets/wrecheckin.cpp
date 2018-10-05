#include "wrecheckin.h"
#include "ui_wrecheckin.h"
#include "wreservation.h"

WReCheckIn::WReCheckIn(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WReCheckIn)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

WReCheckIn::~WReCheckIn()
{
    delete ui;
}

void WReCheckIn::setupTab()
{
    setupTabTextAndIcon(tr("Today check out room"), ":/images/room-key.png");
}

void WReCheckIn::btnClick(int tag)
{
    QString id = ui->tblData->item(tag, 0)->data(Qt::EditRole).toString();
    WReservation::openReserveWindows(id);
}

void WReCheckIn::on_btnRefresh_clicked()
{
    ui->tblData->clearContents();
    ui->tblData->setRowCount(0);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                100, 100, 300, 30);
    fDbBind[":f_state"] = RESERVE_CHECKOUT;
    fDbBind[":f_endDate"] = WORKING_DATE;
    fDb.select("select r.f_id, r.f_room, concat(g.f_title, ' ' , g.f_firstName, ' ', g.f_lastName), ' ' "
               "from f_reservation r "
               "inner join f_guests g on g.f_id=r.f_guest "
               "where r.f_endDate=:f_endDate and r.f_state=:f_state", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        EPushButton *b = new EPushButton();
        b->setTag(i);
        b->setIcon(QIcon(":/images/double-right.png"));
        connect(b, SIGNAL(clickedWithTag(int)), this, SLOT(btnClick(int)));
        ui->tblData->setCellWidget(i, 3, b);
    }
}
