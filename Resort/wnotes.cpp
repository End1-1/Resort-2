#include "wnotes.h"
#include "dlgnotes.h"

WNotes::WNotes(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(0, "f_id", tr("Code"))
            .setColumn(0, "f_state", tr("State code"))
            .setColumn(80, "f_state_name", tr("State"))
            .setColumn(0, "f_room", tr("Room code"))
            .setColumn(100, "f_room_short", tr("Room"))
            .setColumn(0, "f_group", tr("Group code"))
            .setColumn(80, "f_groupName", tr("Group"))
            .setColumn(140, "f_dateStart", tr("Date start"))
            .setColumn(100, "f_interval", tr("Interval"))
            .setColumn(100, "f_text", tr("Text"))
            .setColumn(0, "f_guest", tr("Guest code"))
            .setColumn(200, "f_guestName", tr("Guest"))
            .setColumn(100, "f_nationality", tr("Nationality"))
            .setColumn(200, "f_author", tr("Author"));

    fModel->setSqlQuery("select r.f_id, r.f_state, s.f_" + def_lang + ", r.f_room, rm.f_short as f_room_short, "
                        "r.f_group, ug.f_" + def_lang + ", r.f_dateStart, "
                        "r.f_interval, r.f_text, r.f_guest, concat(g.f_firstName, ' ', g.f_lastName) as f_guestName, "
                        "n.f_name as f_nationality, concat(u.f_firstName, ' ', u.f_lastName) "
                        "from f_reminder r "
                        "left join f_room rm on r.f_room=rm.f_id "
                        "left join f_guests g on r.f_guest=g.f_id "
                        "left join f_nationality n on g.f_nation=n.f_short "
                        "left join f_reminder_state s on r.f_state=s.f_id "
                        "left join users_groups ug on ug.f_id=r.f_group "
                        "left join users u on u.f_id=r.f_author "
                        "order by r.f_state, r.f_dateStart ");
    fModel->apply(this);

    fRowEditorDialog = new DlgNotes(fRowValues, this);
    setBtnNewVisible();
}

void WNotes::setupTab()
{
    setupTabTextAndIcon(tr("Notes"), ":/images/notepad.png");
}

