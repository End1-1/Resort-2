#include "wguests.h"
#include "wguest.h"

WGuests::WGuests(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(0, "f_id", tr("Code"))
            .setColumn(50, "f_title", tr("Title"))
            .setColumn(150, "f_firstName", tr("First name"))
            .setColumn(150, "f_lastName", tr("Last name"))
            .setColumn(0, "f_sex", tr("Sex code"))
            .setColumn(100, "f_sex_name", tr("Sex"))
            .setColumn(140, "f_dateBirth", tr("Birth date"))
            .setColumn(150, "f_placeBirth", tr("Birth place"))
            .setColumn(0, "f_nationality", tr("Nationality code"))
            .setColumn(100, "nationality", tr("Nationality"))
            .setColumn(100, "f_passport", tr("Passport"))
            .setColumn(200, "f_address", tr("Address"))
            .setColumn(100, "f_tel1", tr("Tel. 1"))
            .setColumn(100, "f_tel2", tr("Tel. 2"))
            .setColumn(100, "f_email", tr("Email"))
            .setColumn(200, "f_postBox", tr("Postbox"))
            .setColumn(300, "f_remarks", tr("Remarks"));

    fModel->setSqlQuery("select g.f_id, g.f_title, g.f_firstName, g.f_lastName, g.f_sex, s.f_" + def_lang + ", g.f_dateBirth, "
                        "g.f_placeBirth, "
                        "n.f_short, n.f_name as nationality, g.f_passport, g.f_address, g.f_tel1, g.f_tel2, "
                        "g.f_email, g.f_postBox, g.f_remarks "
                        "from f_guests g "
                        "left join f_nationality n on g.f_nation=n.f_short "
                        "left join f_guests_sex s on s.f_id=g.f_sex");
    fModel->apply(this);

    fRowEditorDialog = WGuest::guest(fRowValues);
    setBtnNewVisible();
}

void WGuests::setupTab()
{
    setupTabTextAndIcon(tr("Guests"), ":/images/guests.png");
}
