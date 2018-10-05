#include "fstoreentry.h"
#include "ui_fstoreentry.h"
#include "wreportgrid.h"
#include "wstoreentry.h"

FStoreEntry::FStoreEntry(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FStoreEntry)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Store entries"), ":/images/storeentry.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
    fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removeDoc()), this)->setFocusPolicy(Qt::NoFocus);
}

FStoreEntry::~FStoreEntry()
{
    delete ui;
}

QString FStoreEntry::reportTitle()
{
    return QString("%1")
            .arg(tr("Store entry"));
}

QWidget *FStoreEntry::firstElement()
{
    return this;
}

void FStoreEntry::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Code"))
            .setColumn(120, "", tr("Date"))
            .setColumn(200, "", tr("Store"))
            .setColumn(100, "", tr("Amount"));
    QString query = "select e.f_id, e.f_date, s.f_name, e.f_amount "
                    "from st_header e "
                    "left join r_store s on s.f_id=e.f_store "
                    "order by 2, 3 ";
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
}

void FStoreEntry::doubleClickOnRow(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    WStoreEntry::openDoc(row.at(0).toInt());
}

void FStoreEntry::removeDoc()
{
    QList<QVariant> out;
    fReportGrid->fillRowValuesOut(out);
    if (out.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    if (message_confirm(tr("Confirm to remove document") + "<br>#" + out.at(0).toString()) != RESULT_YES) {
        return;
    }
    fDbBind[":f_doc"] = out.at(0);
    fDb.select("delete from st_body where f_doc=:f_doc", fDbBind, fDbRows);
    fDbBind[":f_id"] = out.at(0);
    fDb.select("delete from st_header where f_id=:f_id", fDbBind, fDbRows);
    message_info(tr("Document was removed. Please, refresh report"));
}
