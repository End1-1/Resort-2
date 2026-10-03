#include "fgiftcart.h"
#include <QDialog>
#include <QMenu>
#include <QSignalBlocker>
#include "basewidget.h"
#include "dlggiftcartstatus.h"
#include "fgiftcartusage.h"
#include "message.h"
#include "ui_fgiftcart.h"
#include "wreportgrid.h"

namespace {
const int FILTER_ALL = -1;
const int VIEW_DETAILED = 0;
const int VIEW_SUMMARY = 1;
}

FGiftCart::FGiftCart(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FGiftCart),
    fFiltersLoaded(false)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Gift cards"), ":/images/car.png");

    ui->cbViewMode->addItem(tr("Detailed"), VIEW_DETAILED);
    ui->cbViewMode->addItem(tr("Summary"), VIEW_SUMMARY);

    connect(ui->cbViewMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        apply(fReportGrid);
    });
    connect(ui->cbStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        apply(fReportGrid);
    });
    connect(ui->cbAmount, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        apply(fReportGrid);
    });

    fReportGrid->fGridMenu->addAction(tr("Show usage"), this, SLOT(showCardUsage()));
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(onRowDoubleClick(QList<QVariant>)));
}

FGiftCart::~FGiftCart()
{
    delete ui;
}

bool FGiftCart::isSummaryMode() const
{
    return ui->cbViewMode->currentData().toInt() == VIEW_SUMMARY;
}

void FGiftCart::loadFilters()
{
    QSignalBlocker blockStatus(ui->cbStatus);
    QSignalBlocker blockAmount(ui->cbAmount);

    ui->cbStatus->clear();
    ui->cbStatus->addItem(tr("All"));
    ui->cbStatus->setItemData(0, FILTER_ALL, Qt::UserRole);

    ui->cbAmount->clear();
    ui->cbAmount->addItem(tr("All"));
    ui->cbAmount->setItemData(0, FILTER_ALL, Qt::UserRole);

    if(!fMainWindow->fDb.fDb.isOpen()) {
        message_error(tr("Database is not connected."));
        return;
    }

    Database &db = fMainWindow->fDb;
    QMap<QString, QVariant> bind;

    fDbRows.clear();
    if(db.select("select f_id, f_name from d_gift_cart_statuses order by f_id", bind, fDbRows) < 0) {
        message_error(db.fLastError);
    } else {
        for(const QList<QVariant> &row : fDbRows) {
            const int idx = ui->cbStatus->count();
            ui->cbStatus->addItem(row.at(1).toString());
            ui->cbStatus->setItemData(idx, row.at(0).toInt(), Qt::UserRole);
        }
    }

    fDbRows.clear();
    if(db.select("select distinct f_initialamount from d_gift_cart where f_status>0 order by f_initialamount",
                 bind, fDbRows) < 0) {
        message_error(db.fLastError);
        return;
    }

    for(const QList<QVariant> &row : fDbRows) {
        const int amount = qRound(row.at(0).toDouble());
        const int idx = ui->cbAmount->count();
        ui->cbAmount->addItem(QString::number(amount));
        ui->cbAmount->setItemData(idx, amount, Qt::UserRole);
    }
}

QString FGiftCart::filterSql() const
{
    QString sql;

    const int statusId = ui->cbStatus->currentData(Qt::UserRole).toInt();
    if(statusId != FILTER_ALL) {
        sql += QString(" AND c.f_status=%1 ").arg(statusId);
    }

    const int amountId = ui->cbAmount->currentData(Qt::UserRole).toInt();
    if(amountId != FILTER_ALL) {
        sql += QString(" AND c.f_initialamount = %1 AND c.f_status > 0 ").arg(amountId);
    }

    return sql;
}

QString FGiftCart::reportTitle()
{
    if(isSummaryMode()) {
        return tr("Gift cards summary");
    }

    return tr("Gift cards");
}

QWidget *FGiftCart::firstElement()
{
    return ui->cbViewMode;
}

void FGiftCart::apply(WReportGrid *rg)
{
    if(!fFiltersLoaded) {
        fFiltersLoaded = true;
        loadFilters();
    }

    if(isSummaryMode()) {
        applySummary(rg);
    } else {
        applyDetailed(rg);
    }
}

void FGiftCart::applyDetailed(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "f_id", "")
            .setColumn(100, "f_code", tr("Code"))
            .setColumn(100, "f_num", tr("Number"))
            .setColumn(300, "f_info", tr("Comment"))
            .setColumn(100, "f_initialamount", tr("Amount"))
            .setColumn(100, "f_fiscal", tr("Fiscal"))
            .setColumn(100, "f_spent", tr("Balance"))
            .setColumn(150, "f_status_name", tr("Status"));

    QString sql = R"(
SELECT c.f_id, c.f_code, right(c.f_code, 4) AS f_num, c.f_info, c.f_initialamount, c.f_fiscal,
       coalesce(u.f_spent, 0) AS f_spent, coalesce(s.f_name, '') AS f_status_name
FROM d_gift_cart c
LEFT JOIN d_gift_cart_statuses s ON s.f_id = c.f_status
LEFT JOIN (SELECT f_code, SUM(f_amount) AS f_spent FROM d_gift_cart_use GROUP BY 1) u ON u.f_code = c.f_code
WHERE 1=1
)";

    sql += filterSql();
    sql += " ORDER BY CAST(RIGHT(c.f_code, 4) AS UNSIGNED)";

    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
}

void FGiftCart::applySummary(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(200, "f_status_name", tr("Status"))
            .setColumn(100, "f_initialamount", tr("Amount"))
            .setColumn(100, "f_qty", tr("Qty"));

    QString sql = R"(
SELECT coalesce(s.f_name, '') AS f_status_name,
       c.f_initialamount,
       COUNT(*) AS f_qty
FROM d_gift_cart c
LEFT JOIN d_gift_cart_statuses s ON s.f_id = c.f_status
WHERE 1=1
)";

    sql += filterSql();
    sql += " GROUP BY c.f_status, s.f_name, c.f_initialamount ";
    sql += " ORDER BY c.f_status, c.f_initialamount ";

    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);

    QList<int> cols;
    QList<double> vals;
    cols << 2;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FGiftCart::onRowDoubleClick(const QList<QVariant> &row)
{
    if(isSummaryMode()) {
        return;
    }

    if(row.size() < 2) {
        message_error(tr("Select a gift card row."));
        return;
    }

    int cardId = row.at(0).toInt();
    if(cardId <= 0) {
        cardId = static_cast<int>(row.at(0).toLongLong());
    }

    const QString code = row.at(1).toString().trimmed();
    if(cardId <= 0 && code.isEmpty()) {
        message_error(tr("Select a gift card row."));
        return;
    }

    DlgGiftCartStatus dlg(cardId, code, fReportGrid);
    if(dlg.exec() == QDialog::Accepted) {
        apply(fReportGrid);
    }
}

void FGiftCart::showCardUsage()
{
    if(isSummaryMode()) {
        message_error(tr("Switch to detailed view."));
        return;
    }

    QList<QVariant> row;
    if(fReportGrid->fillRowValuesOut(row) < 0 || row.size() < 3) {
        message_error(tr("Select a gift card row."));
        return;
    }

    const QString code = row.at(1).toString();
    const QString label = row.at(2).toString().isEmpty() ? code : row.at(2).toString();
    FGiftCartUsage::openReport(code, label);
}
