#include "wremarks.h"
#include <QHeaderView>

WRemarks::WRemarks(QWidget *parent) :
    WReportGrid(parent)
{

    fTableView->verticalHeader()->setDefaultSectionSize(40);
    fModel->setColumn(100, "f_room", tr("Room"))
            .setColumn(200, "f_guest", tr("Guest"))
            .setColumn(700, "f_remarks", tr("Remarks"));
    fModel->setSqlQuery("select r.f_room, concat(g.f_firstName, ' ' , g.f_lastName), r.f_remarks "
                        "from f_reservation r "
                        "left join f_guests g on g.f_id=r.f_guest "
                        "where r.f_state=1 ");
    fModel->apply(this);
    for (int i = 0; i < fModel->rowCount(); i++) {
        fModel->setBackgroundColor(i, (i % 2 == 0 ? Qt::white : COLOR_DARK_ROW));
    }
}

void WRemarks::setup()
{
    setupTabTextAndIcon(tr("Remarks"), ":/images/remarks.png");
}
