#include "dlgsearchinvoice.h"
#include "ui_dlgsearchinvoice.h"

DlgSearchInvoice::DlgSearchInvoice(const QDate &start, const QDate &end, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSearchInvoice)
{
    ui->setupUi(this);
    ui->deStart->setDate(start);
    ui->deEnd->setDate(end);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                80, 80, 80, 100, 100, 300);
    on_btnRefresh_clicked();
}

DlgSearchInvoice::~DlgSearchInvoice()
{
    delete ui;
}

void DlgSearchInvoice::on_btnClearFilter_clicked()
{
    ui->leFilter->clear();
    filter();
}

void DlgSearchInvoice::filter()
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        ui->tblData->setRowHidden(i, true);
        if (ui->leFilter->text().length()) {
            ui->tblData->setRowHidden(i, false);
            continue;
        }
        for (int j = 0; j < ui->tblData->columnCount(); j++) {
            if (ui->tblData->toString(i, j).contains(ui->leFilter->text(), Qt::CaseInsensitive)) {
                ui->tblData->setRowHidden(i, false);
                goto next;
            }
        }
        next:
        continue;
    }
}

void DlgSearchInvoice::on_btnRefresh_clicked()
{
    fDbBind[":f_date1"] = ui->deStart->date();
    fDbBind[":f_date2"] = ui->deEnd->date();
    fDb.select("select r.f_invoice, r.f_id, r.f_room, r.f_startDate, r.f_endDate, concat(g.f_firstName, ' ' , g.f_lastName) "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_state=3 "
               "and (r.f_endDate between :f_date1 and :f_date2 or r.f_checkinDate between :f_date1 and :f_date2)", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
}

void DlgSearchInvoice::on_tblData_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    fInvoice = ui->tblData->toInt(index.row(), 0);
    accept();
}
