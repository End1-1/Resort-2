#include "wreportroom.h"
#include "dlgroom.h"

WReportRoom::WReportRoom(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(100, "f_id", tr("Code"))
            .setColumn(0, "f_class", tr("Class code"))
            .setColumn(80, "f_class_short", tr("Class"))
            .setColumn(0, "f_view", tr("View code"))
            .setColumn(80, "f_view_short", tr("View"))
            .setColumn(80, "f_floor", tr("Floor"))
            .setColumn(100, "f_short", tr("Short name"))
            .setColumn(300, "f_description", tr("Description"))
            .setColumn(80, "f_rate", tr("Rate"))
            .setColumn(0, "f_bed", tr("Bed code"))
            .setColumn(80, "f_bed_name", tr("Bed"))
            .setColumn(80, "f_bedQty", tr("Qty"))
            .setColumn(50, "f_queue", tr("Queue"))
            .setColumn(80, "f_smoke", tr("Smoke"));
    fModel->setSqlQuery("select r.f_id, r.f_class, c.f_short, r.f_view, v.f_short as f_view_short, r.f_floor, r.f_short, r.f_description, "
                        "r.f_rate, r.f_bed, b.f_name as f_bed_name, r.f_bedQty, r.f_queue, r.f_smoke "
                        "from f_room r  "
                        "left join f_room_classes c on r.f_class=c.f_id "
                        "left join f_room_bed b on r.f_bed=b.f_id "
                        "left join f_room_view v on v.f_id=r.f_view "
                        "order by c.f_short, r.f_short");
    fModel->apply(this);
    fRowEditorDialog = new DlgRoom(fRowValues, this);
}

WReportRoom::~WReportRoom()
{

}

void WReportRoom::setupTab()
{
    setupTabTextAndIcon(tr("Rooms"), ":/images/room-key.png");
}
