#include "favailablerooms.h"
#include "ui_favailablerooms.h"
#include "wreportgrid.h"
#include "cachereservation.h"

FAvailableRooms::FAvailableRooms(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAvailableRooms)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Available rooms"), ":/images/bed.png");
    connect(ui->wd, &WDate2::changed, [this](){
        apply(fReportGrid);
    });
}

FAvailableRooms::~FAvailableRooms()
{
    delete ui;
}

QString FAvailableRooms::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Available rooms"))
            .arg(ui->wd->ds1())
            .arg(ui->wd->ds2());
}

QWidget *FAvailableRooms::firstElement()
{
    return ui->wd->fw();
}

void FAvailableRooms::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("View"))
            .setColumn(100, "", tr("Cat"))
            .setColumn(100, "", tr("Room"));
    QString query = "select v.f_short, c.f_short, r.f_id "
            "from f_room r "
            "left join f_room_view v on v.f_id=r.f_view "
            "left join f_room_classes c on c.f_id=r.f_class ";
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QSet<int> list;
    CacheReservation::instance()->exludeList(ui->wd->d1(), ui->wd->d2(), list);
    for (int i = rg->fModel->rowCount() - 1; i > -1; i--) {
        if (list.contains(rg->fModel->data(i, 2).toInt())) {
            rg->fModel->removeRow(i);
        }
    }
}
