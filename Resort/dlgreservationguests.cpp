#include "dlgreservationguests.h"
#include "ui_dlgreservationguests.h"

DlgReservationGuests::DlgReservationGuests(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReservationGuests)
{
    ui->setupUi(this);
}

DlgReservationGuests::~DlgReservationGuests()
{
    delete ui;
}

void DlgReservationGuests::viewGuests(const QString &id)
{
    DlgReservationGuests *rg = new DlgReservationGuests(fPreferences.getDefaultParentForMessage());
    rg->load(id);
    rg->exec();
    delete rg;
}

void DlgReservationGuests::load(const QString &id)
{
    fDbBind[":f_reservation"] = id;
    DatabaseResult dr;
    dr.select(fDb, "select g.f_title, g.f_firstName, g.f_lastName, g.f_nation, g.f_passport "
                   "from f_reservation_guests r "
                   "left join f_guests g on g.f_id=r.f_guest "
                   "where r.f_reservation=:f_reservation "
                   "order by r.f_first desc", fDbBind);
    ui->tblData->clearContents();
    ui->tblData->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        for (int j =0; j < dr.columnCount(); j++) {
            ui->tblData->setItemWithValue(i, j, dr.value(i, j));
        }
    }
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                80, 200, 200, 100, 200);
}
