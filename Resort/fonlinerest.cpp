#include "fonlinerest.h"
#include "ui_fonlinerest.h"
#include "wreportgrid.h"
#include "cacheresthall.h"
#include "vauchers.h"

FOnlineRest::FOnlineRest(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FOnlineRest)
{
    ui->setupUi(this);
    fCurrentHall = 0;
    CacheRestHall *ch = CacheRestHall::instance();
    QMapIterator<QString, CI_RestHall*> it(ch->it());
    int col = 1;
    while (it.hasNext()) {
        it.next();
        ui->tblFilter->setItemWithValue(0, col, it.value()->fName, it.value()->fCode);
        col++;
    }
    ui->tblFilter->setItemWithValue(0, 0, tr("All"), 0);
    fReportGrid->setupTabTextAndIcon(tr("Restaurant online"), ":/images/dinner.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
}

FOnlineRest::~FOnlineRest()
{
    delete ui;
}

QWidget *FOnlineRest::firstElement()
{
    return ui->tblFilter;
}

QString FOnlineRest::reportTitle()
{
    return QString("%1 %2").arg(tr("Restaurant online")).arg(QDateTime::currentDateTime().toString(def_date_time_format));
}

void FOnlineRest::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Order"))
            .setColumn(150, "", tr("Opened"))
            .setColumn(100, "", tr("Hall"))
            .setColumn(80, "", tr("Table"))
            .setColumn(200, "", tr("Staff"))
            .setColumn(100, "", tr("Amount"))
            .setColumn(300, "", tr("Comment"))
            ;
    QString query = "select oh.f_id, oh.f_dateOpen, h.f_name, t.f_name, u.f_username, oh.f_total, "
            "oh.f_comment "
            "from o_header oh "
            "left join r_hall h on oh.f_hall=h.f_id "
            "left join r_table t on t.f_id=oh.f_table "
            "left join users u on u.f_id=oh.f_staff "
            "where oh.f_state=" + QString::number(ORDER_STATE_OPENED);
    if (fCurrentHall > 0) {
        query += " and oh.f_hall=" + QString::number(fCurrentHall);
    }
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 5;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FOnlineRest::openOnlineRestaurant()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FOnlineRest *y = new FOnlineRest(rg);
    rg->addFilterWidget(y);
    y->apply(rg);
}

void FOnlineRest::doubleClickOnRow(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        return;
    }
    openVaucher("PS", row.at(0).toString());
}

void FOnlineRest::on_tblFilter_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    fCurrentHall = index.data(Qt::UserRole).toInt();
    apply(fReportGrid);
}
