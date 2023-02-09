#include "dlgreservation.h"
#include "ui_dlgreservation.h"
#include "rmessage.h"

DlgReservation::DlgReservation(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReservation)
{
    ui->setupUi(this);
    ui->tblItem->setVisible(false);
    fItemCode = 0;
    fMode = 0;
    connect(ui->kbd, SIGNAL(textChanged(QString)), this, SLOT(kdbText(QString)));
    connect(ui->kbd, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->kbd, SIGNAL(accepted()), this, SLOT(kbdAccept()));
}

DlgReservation::~DlgReservation()
{
    delete ui;
}

void DlgReservation::loadCars()
{
    fMode = 3;
    ui->tblData->setColumnCount(2);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 400);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  tr("Code"), tr("Name"));
    QString query = "select f_id, concat(f_model, ' ', f_class) from d_car_model order by 2";
    QSqlQuery *q = 0;
    q = fDb.select(q, query, fDbBind);
    if (!q) {
        return;
    }
    while (q->next()) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        for (int i = 0; i < ui->tblData->columnCount(); i++) {
            ui->tblData->setItem(row, i, new QTableWidgetItem(q->value(i).toString()));
        }
    }
    delete q;
}

void DlgReservation::loadEmployes()
{
    ui->leHint->setText(tr("Employes"));
    fMode = 4;
    ui->tblData->setColumnCount(2);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 400);
    Utils::tableSetHeaderCaptions(ui->tblData, ui->tblData->columnCount(),
                                  tr("Code"), tr("Name"));
    QString query = "select f_id, concat(f_firstName, ' ', f_lastName) from users where f_state=1 order by 2";
    QSqlQuery *q = 0;
    q = fDb.select(q, query, fDbBind);
    if (!q) {
        return;
    }
    while (q->next()) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        for (int i = 0; i < ui->tblData->columnCount(); i++) {
            ui->tblData->setItem(row, i, new QTableWidgetItem(q->value(i).toString()));
        }
    }
    delete q;
}

void DlgReservation::kdbText(const QString &text)
{
    ui->leSearch->setText(text);
}

void DlgReservation::kbdAccept()
{
    if (fMode == 1) {
        if (fItemCode == 0) {
            return;
        }
    }
    if (fMode == 2) {
        if (fReservationId.isEmpty()) {
            return;
        }
    }
    if (fMode == 3) {
        if (fItemCode == 0) {
            return;
        }
    }
    if (fMode == 4) {
        if (fItemCode == 0) {
            return;
        }
    }
    accept();
}

void DlgReservation::on_leSearch_textChanged(const QString &arg1)
{
    ui->kbd->setText(arg1);
    for (int i = 0, rowCount = ui->tblData->rowCount(); i < rowCount; i++) {
        ui->tblData->setRowHidden(i, true);
        for (int j = 0, colCount = ui->tblData->columnCount(); j < colCount; j++) {
            if (ui->tblData->item(i, j)->text().contains(arg1, Qt::CaseInsensitive)) {
                ui->tblData->setRowHidden(i, false);
                goto mark;
            }
        }
        mark:
        continue;
    }
}

void DlgReservation::on_btnClear_clicked()
{
    ui->leSearch->clear();
}

void DlgReservation::on_tblData_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    /// room
    if (fMode == 1) {
        fItemCode = 1;
        fReservationId = ui->tblData->item(index.row(), 0)->text();
        ui->leHint->setText(tr("Room #") + ui->tblData->item(index.row(), 1)->text() + ", "
                            + ui->tblData->item(index.row(), 2)->text());
    }
    /// cl
    if (fMode == 2) {
        fReservationId = ui->tblData->item(index.row(), 0)->text();
        ui->leHint->setText(ui->tblData->item(index.row(), 0)->text() + ", "
                            + ui->tblData->item(index.row(), 1)->text());
    }
    /// car
    if (fMode == 3) {
        fItemCode = ui->tblData->item(index.row(), 0)->text().toInt();
        fReservationId = ui->tblData->item(index.row(), 1)->text();
    }
    /// employee
     if (fMode == 4) {
         fItemCode = ui->tblData->item(index.row(), 0)->text().toInt();
         fReservationId = ui->tblData->item(index.row(), 1)->text();
     }
}

void DlgReservation::on_tblItem_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    fItemCode = index.data(Qt::UserRole).toInt();
}
