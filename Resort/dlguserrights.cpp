#include "dlguserrights.h"
#include "ui_dlguserrights.h"
#include <QProxyStyle>

class CustomTabStyle : public QProxyStyle {
public:
  QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                         const QSize& size, const QWidget* widget) const {
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
      s.transpose();
    }
    return s;
  }

  void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    if (element == CE_TabBarTabLabel) {
      if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
        QStyleOptionTab opt(*tab);
        opt.shape = QTabBar::RoundedNorth;
        QProxyStyle::drawControl(element, &opt, painter, widget);
        return;
      }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
  }


};

DlgUserRights::DlgUserRights(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgUserRights)
{
    ui->setupUi(this);
    ui->tw->tabBar()->setStyle(new CustomTabStyle());
}

DlgUserRights::~DlgUserRights()
{
    delete ui;
}

void DlgUserRights::setup(QList<QVariant> &values)
{
    fGroupId = values.at(0).toInt();
    setWindowTitle(tr("Rights for") + " " + values.at(1).toString());
    fTrackControl = new TrackControl(TRACK_USER_RIGHTS);
    fTrackControl->
            addWidget(ui->chTrackChanges, "Track changes")
            .addWidget(ui->chMenuBookeeping, "All bookkeeping items")
            .addWidget(ui->chMenuRestaurant, "All restaurant items")
            .addWidget(ui->chMenuDirectory, "All directory items")
            .addWidget(ui->chPartners, "Partners")
            .addWidget(ui->chGuests, "Guests")
            .addWidget(ui->chGlobalConfig, "Global config")
            .addWidget(ui->chCreditCard, "Credit cards")
            .addWidget(ui->chServices, "Services")

            .addWidget(ui->choMovement, "Move table / dishes")
            .addWidget(ui->choCancelation, "Cancelation")
            .addWidget(ui->choPrintUnlimitedReceipt, "Print unlimited receipt")
            .addWidget(ui->choPrintReports, "Print reports")



            .addWidget(ui->chVaucherHistory, "Vouchers history")
            .addWidget(ui->chDailyMovement, "Daily movement")
            .addWidget(ui->chMonthlyTotal, "Monthly report")
            .addWidget(ui->chReportRestaurant, "Restaurant report")
            .addWidget(ui->chCheckoutInvoices, "CheckoutInvoices")
            .addWidget(ui->chRoomEditor, "Room editor")
            .addWidget(ui->chInvoiceItems, "Invoice items")
            .addWidget(ui->chSetupTax, "Setup tax printer")
            .addWidget(ui->chMenuCashier, "Menu cashier")
            .addWidget(ui->chCurrencies, "Currencies")
            .addWidget(ui->chAdvanceReceipt, "Advance voucher")
            .addWidget(ui->chPostCharge, "Post charge")
            .addWidget(ui->chTransferAmount, "Transfer amount")
            .addWidget(ui->chReceiptVaucher, "Recept voucher")
            .addWidget(ui->chDiscountVaucher, "Discount voucher")


            .addWidget(ui->chCallInt, "Internal call")
            .addWidget(ui->chCallTransferIn, "Transfer in call")
            .addWidget(ui->chCallTrasferOut, "Transfer out call")
            .addWidget(ui->chCardexAnalysis, "Cardex analysis")
            .addWidget(ui->chRestReportanyDay, "Restuarant report, any day")

            .addWidget(ui->chComplimentary, "Complimentary")
            .addWidget(ui->chExportInvoice, "Export invocie")
            .addWidget(ui->chActiveReserve, "Export active reserve")
            .addWidget(ui->chSingleVaucher, "Export signle vouchers")

            .addWidget(ui->chPrintReceiptByNumber, "Print receipt by number")

            .addWidget(ui->chReportSet, "Reports set")
            .addWidget(ui->chRemoveOrderPermanent, "Remove restaurant order permanently")
            .addWidget(ui->chExportSingleVauchers, "Export single vouchers")
            .addWidget(ui->chPrintTaxByNumber, "Print tax by order number")
            .addWidget(ui->chOnlineRest, "Online restaurant")
            .addWidget(ui->chMoveInvoiceItem, "Move entry in checkout invoice")
            .addWidget(ui->chRestaurantReports, "Restaurant reports")

            .addWidget(ui->chStoreHouseAllItems, "Store house, all items")
            .addWidget(ui->chHotelRoomInventory, "Directory of hotel, room inventory")
            .addWidget(ui->chRoomInventoryState, "Directory of hotel, room inventory state")
            .addWidget(ui->chInvoiceExportToExcel, "Invoice, export to excel")

            .addWidget(ui->chAdvanceReport, "Cashier, advance report")
            .addWidget(ui->chChangeRVDateBank, "Change date for receipt voucher, bank")
            .addWidget(ui->chChangeRVDateCash, "Change date for receipt voucher, cash")
            .addWidget(ui->chChangeRVDateCL, "Change date for receipt voucher, CL")
            .addWidget(ui->chChangeDateRVCard, "Change date for receipt voucher, card")
            .addWidget(ui->chTrakingIndividual, "Changes tracking, individual")

            .addWidget(ui->chTransferAnyDirection, "Transfer amount, any direction")
            .addWidget(ui->chRefundVoucher, "Refund voucher")

            .addWidget(ui->chBookHGuestTrayLegder, "Guest tray ledger")
            .addWidget(ui->chInhouseDetailedBalance, "Inhouse guests detailed balance")
            .addWidget(ui->chYeralyFinancial, "Yearly financial report")
            .addWidget(ui->chMonthlyOccupancyPercentages, "Monthly occupancy percentages")
            .addWidget(ui->chGuestByNationality, "Guests by nationality")
            ;
    fDbBind[":f_group"] = fGroupId;
    fDb.select("select f_right, f_flag from users_rights where f_group=:f_group", fDbBind, fDbRows);
    QMap<int, QString> fTags;
    foreach_rows {
        QListIterator<TrackWidget*> ti = fTrackControl->widgetIterator();
        while (ti.hasNext()) {
            EQCheckBox *check = static_cast<EQCheckBox*>(ti.next()->fWidget);            
            if (check->getTag() == it->at(0).toInt()) {
                if (fTags.contains(check->getTag())) {
                    message_error("Duplicate tag: " + fTags[check->getTag()] + "/" + check->text());
                }
                fTags.insert(check->getTag(), check->text());
                check->setChecked(it->at(1).toInt() == 1);
                break;
            }
        }
    }
    fTrackControl->resetChanges();
}

void DlgUserRights::on_btnCancel_clicked()
{
    reject();
}

void DlgUserRights::on_btnOk_clicked()
{
    fDb.fDb.transaction();
    fDbBind[":f_group"] = fGroupId;
    fDb.select("delete from users_rights where f_group=:f_group", fDbBind, fDbRows);
    QListIterator<TrackWidget*> ti = fTrackControl->widgetIterator();
    QString query = "insert into users_rights (f_group, f_right, f_flag) values ";
    bool first = true;
    while (ti.hasNext()) {
        TrackWidget *t = ti.next();
        if (first) {
            first = false;
        } else {
            query += ",";
        }
        EQCheckBox *check = static_cast<EQCheckBox*>(t->fWidget);
        if (check->getTag() == 0) {
            message_error("Axtung, Tag==0, " + t->fKey);
        }
        query += QString("(%1, %2, %3)")
                .arg(fGroupId)
                .arg(check->getTag())
                .arg(check->isChecked());
    }
    fDb.queryDirect(query);
    if (fTrackControl->hasChanges()) {
        fTrackControl->saveChanges();
    }
    fDb.fDb.commit();
    accept();
}

void DlgUserRights::on_chCheckUncheck_clicked(bool checked)
{
    QListIterator<TrackWidget*> ti = fTrackControl->widgetIterator();
    while (ti.hasNext()) {
        TrackWidget *t = ti.next();
        EQCheckBox *check = static_cast<EQCheckBox*>(t->fWidget);
        check->setChecked(checked);
    }
}
