#include "dlggiftcartstatus.h"
#include "ui_dlggiftcartstatus.h"
#include "giftcartstore.h"
#include "basewidget.h"
#include "message.h"

DlgGiftCartStatus::DlgGiftCartStatus(int cardId, const QString &cardCode, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgGiftCartStatus),
    fCardId(cardId),
    fCardCode(cardCode.trimmed()),
    fCurrentStatus(0),
    fSold(false)
{
    ui->setupUi(this);

    if(!loadCard()) {
        reject();
        return;
    }

    populateStatuses();
}

DlgGiftCartStatus::~DlgGiftCartStatus()
{
    delete ui;
}

bool DlgGiftCartStatus::loadCard()
{
    if(!fMainWindow->fDb.fDb.isOpen()) {
        message_error(tr("Database is not connected."));
        return false;
    }

    Database &db = fMainWindow->fDb;
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > rows;

    const QString selectBody = QStringLiteral(
        "select c.f_id, c.f_code, c.f_initialamount, coalesce(c.f_status, 0) as f_status, "
        "coalesce(s.f_name, '') as f_status_name, coalesce(c.f_fiscal, 0) as f_fiscal "
        "from d_gift_cart c "
        "left join d_gift_cart_statuses s on s.f_id=c.f_status ");

    auto runSelect = [&](const QString &whereSql, QMap<QString, QVariant> b) -> bool {
        rows.clear();
        return db.select(selectBody + whereSql, b, rows) >= 0 && !rows.isEmpty();
    };

    bind.clear();
    if(fCardId > 0) {
        bind[":f_id"] = fCardId;
        if(!runSelect(QStringLiteral("where c.f_id=:f_id"), bind) && !fCardCode.isEmpty()) {
            bind.clear();
            bind[":f_code"] = fCardCode;
            runSelect(QStringLiteral("where c.f_code=:f_code"), bind);
        }
    } else if(!fCardCode.isEmpty()) {
        bind[":f_code"] = fCardCode;
        runSelect(QStringLiteral("where c.f_code=:f_code"), bind);
    }

    if(rows.isEmpty()) {
        message_error(tr("Gift card not found."));
        return false;
    }

    fCardId = rows.at(0).at(0).toInt();
    ui->leCode->setText(rows.at(0).at(1).toString());
    ui->leAmount->setText(QString::number(rows.at(0).at(2).toDouble(), 'f', 0));
    fCurrentStatus = rows.at(0).at(3).toInt();
    fSold = rows.at(0).at(5).toInt() != 0;

    QString currentName = rows.at(0).at(4).toString();
    if(currentName.isEmpty() && GiftCartStore::isSoldStatus(fCurrentStatus)) {
        currentName = tr("Sold");
    } else if(currentName.isEmpty() && fCurrentStatus > 0) {
        currentName = QString::number(fCurrentStatus);
    }
    ui->leCurrentStatus->setText(currentName);

    return true;
}

void DlgGiftCartStatus::populateStatuses()
{
    ui->cbNewStatus->clear();

    if(!fMainWindow->fDb.fDb.isOpen()) {
        message_error(tr("Database is not connected."));
        ui->btnOk->setEnabled(false);
        return;
    }

    Database &db = fMainWindow->fDb;
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > rows;
    if(db.select("select f_id, f_name from d_gift_cart_statuses order by f_id", bind, rows) < 0) {
        message_error(db.fLastError);
        ui->btnOk->setEnabled(false);
        return;
    }

    int firstSelectable = -1;
    auto addStatusItem = [&](int statusId, const QString &name) {
        QString label = name;
        if(label.isEmpty()) {
            label = QString::number(statusId);
        }
        if(statusId == fCurrentStatus) {
            label += tr(" (current)");
        }
        const int idx = ui->cbNewStatus->count();
        ui->cbNewStatus->addItem(label);
        ui->cbNewStatus->setItemData(idx, statusId, Qt::UserRole);
        if(statusId != fCurrentStatus && firstSelectable < 0) {
            firstSelectable = idx;
        }
    };

    if(fSold) {
        if(!GiftCartStore::isSoldStatus(fCurrentStatus)) {
            for(const QList<QVariant> &row : rows) {
                if(row.at(0).toInt() == GiftCartStore::SOLD_CARD_STATUS) {
                    addStatusItem(row.at(0).toInt(), row.at(1).toString());
                    break;
                }
            }
        }
    } else {
        for(const QList<QVariant> &row : rows) {
            addStatusItem(row.at(0).toInt(), row.at(1).toString());
        }
    }

    if(firstSelectable >= 0) {
        ui->cbNewStatus->setCurrentIndex(firstSelectable);
    }

    ui->btnOk->setEnabled(firstSelectable >= 0);

    if(firstSelectable < 0) {
        message_error(tr("No available status to select."));
    }
}

void DlgGiftCartStatus::on_btnOk_clicked()
{
    if(ui->cbNewStatus->currentIndex() < 0) {
        message_error(tr("Select a new status."));
        return;
    }

    const int newStatus = ui->cbNewStatus->itemData(ui->cbNewStatus->currentIndex(), Qt::UserRole).toInt();
    if(newStatus <= 0) {
        message_error(tr("Select a new status."));
        return;
    }

    if(newStatus == fCurrentStatus) {
        message_error(tr("Status was not changed."));
        return;
    }

    if(fSold && !GiftCartStore::isSoldStatus(newStatus)) {
        message_error(tr("Sold gift card can be moved only to status %1.").arg(GiftCartStore::SOLD_CARD_STATUS));
        return;
    }

    QString error;

    if(!GiftCartStore::moveStatus(fMainWindow->fDb, fCardId, newStatus, error)) {
        message_error(error);
        return;
    }

    message_info(tr("Status changed."));
    accept();
}

void DlgGiftCartStatus::on_btnCancel_clicked()
{
    reject();
}
