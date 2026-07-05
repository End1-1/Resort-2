#include "dlgpaydebt.h"
#include "ui_dlgpaydebt.h"
#include "debtpay.h"
#include "defrest.h"
#include "rmessage.h"
#include "utils.h"
#include <QHeaderView>
#include <QTableWidgetItem>

DlgPayDebt::DlgPayDebt(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPayDebt)
{
    ui->setupUi(this);
    ui->tblDebts->horizontalHeader()->setStretchLastSection(true);
    ui->tblDebts->verticalHeader()->setVisible(false);
    ui->tblDebts->setColumnWidth(0, 120);
    ui->tblDebts->setColumnWidth(1, 180);
    ui->tblDebts->setColumnWidth(2, 140);
    ui->tblDebts->setStyleSheet(
        "QTableWidget#tblDebts::item:selected { background-color: #008000; color: white; }"
        "QTableWidget#tblDebts { selection-background-color: #008000; selection-color: white; }");

    connect(ui->wKbd, &RKeyboard::textChanged, ui->leGovNumber, &QLineEdit::setText);
    connect(ui->wKbd, &RKeyboard::accepted, this, &DlgPayDebt::on_btnSearch_clicked);
    connect(ui->wKbd, &RKeyboard::rejected, this, &DlgPayDebt::on_btnCancel_clicked);
    connect(ui->leGovNumber, &QLineEdit::returnPressed, this, &DlgPayDebt::on_btnSearch_clicked);
    connect(ui->leTalon, &QLineEdit::returnPressed, this, &DlgPayDebt::on_btnPayTalon_clicked);
}

DlgPayDebt::~DlgPayDebt()
{
    delete ui;
}

void DlgPayDebt::prepareToShow()
{
#ifdef QT_DEBUG
    showMaximized();
#else
    showFullScreen();
#endif
    qApp->processEvents();
}

void DlgPayDebt::showDialog(QWidget *parent)
{
    DlgPayDebt *d = new DlgPayDebt(parent);
    d->prepareToShow();
    d->exec();
    delete d;
}

void DlgPayDebt::on_btnSearch_clicked()
{
    searchDebts();
}

void DlgPayDebt::on_btnShowUnpaid_clicked()
{
    showUnpaidDebts();
}

void DlgPayDebt::searchDebts()
{
    fShowAllUnpaid = false;
    ui->tblDebts->setRowCount(0);
    const QString govNumber = ui->leGovNumber->text();

    if(DebtPay::normalizeGovNumber(govNumber).isEmpty()) {
        message_error(tr("Enter plate number"));
        return;
    }

    QList<OpenDebtRow> rows;

    if(!DebtPay::loadOpenDebts(govNumber, rows)) {
        message_error(tr("Database error"));
        return;
    }

    fillDebts(rows);
}

void DlgPayDebt::showUnpaidDebts()
{
    fShowAllUnpaid = true;
    ui->tblDebts->setRowCount(0);

    QList<OpenDebtRow> rows;
    const int branch = defrest(dr_branch).toInt();

    if(!DebtPay::loadAllOpenDebts(branch, rows)) {
        message_error(tr("Database error"));
        return;
    }

    fillDebts(rows);
}

void DlgPayDebt::fillDebts(const QList<OpenDebtRow> &rows)
{
    ui->tblDebts->setRowCount(0);

    for(const OpenDebtRow &row : rows) {
        const int tableRow = ui->tblDebts->rowCount();
        ui->tblDebts->insertRow(tableRow);

        auto *orderItem = new QTableWidgetItem(QString::number(row.orderId));
        orderItem->setData(Qt::UserRole, row.orderId);
        orderItem->setData(Qt::UserRole + 1, row.balance);
        orderItem->setData(Qt::UserRole + 2, row.govNumber);
        ui->tblDebts->setItem(tableRow, 0, orderItem);
        ui->tblDebts->setItem(tableRow, 1, new QTableWidgetItem(row.dateTime.isValid()
                                                                ? row.dateTime.toString("yyyy-MM-dd HH:mm")
                                                                : QString()));
        ui->tblDebts->setItem(tableRow, 2, new QTableWidgetItem(row.govNumber));
        ui->tblDebts->setItem(tableRow, 3, new QTableWidgetItem(float_str(row.balance, 2)));
    }

    if(rows.isEmpty()) {
        message_info(tr("No open debts found"));
    }
}

void DlgPayDebt::refreshDebts()
{
    if(fShowAllUnpaid) {
        showUnpaidDebts();
    } else {
        searchDebts();
    }
}

bool DlgPayDebt::selectedDebt(int &orderId, QString &govNumber, double &balance)
{
    const int row = ui->tblDebts->currentRow();

    if(row < 0) {
        message_error(tr("Select debt row"));
        return false;
    }

    QTableWidgetItem *item = ui->tblDebts->item(row, 0);

    if(!item) {
        message_error(tr("Select debt row"));
        return false;
    }

    orderId = item->data(Qt::UserRole).toInt();
    balance = item->data(Qt::UserRole + 1).toDouble();
    govNumber = item->data(Qt::UserRole + 2).toString();
    return orderId > 0 && balance > 0.001;
}

bool DlgPayDebt::paySelected(int paymentMode)
{
    int orderId = 0;
    QString govNumber;
    double balance = 0;

    if(!selectedDebt(orderId, govNumber, balance)) {
        return false;
    }

    QString talonCode;

    if(paymentMode == DebtPay::PAYMENT_TALON) {
        talonCode = ui->leTalon->text().trimmed();

        if(talonCode.isEmpty()) {
            message_error(tr("Enter talon code"));
            return false;
        }
    }

    if(!message_question(tr("Confirm to pay debt %1 for order %2 (%3)?")
                         .arg(float_str(balance, 2))
                         .arg(orderId)
                         .arg(govNumber))) {
        return false;
    }

    QString error;

    if(!DebtPay::payDebt(orderId, govNumber, balance, paymentMode, talonCode, error)) {
        message_error(error);
        return false;
    }

    message_info(tr("Debt paid"));
    ui->leTalon->clear();
    refreshDebts();
    return true;
}

void DlgPayDebt::on_btnCash_clicked()
{
    paySelected(DebtPay::PAYMENT_CASH);
}

void DlgPayDebt::on_btnCard_clicked()
{
    paySelected(DebtPay::PAYMENT_CARD);
}

void DlgPayDebt::on_btnPayTalon_clicked()
{
    paySelected(DebtPay::PAYMENT_TALON);
}

void DlgPayDebt::on_btnCancel_clicked()
{
    reject();
}
