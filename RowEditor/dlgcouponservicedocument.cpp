#include "dlgcouponservicedocument.h"
#include "ui_dlgcouponservicedocument.h"
#include "reportquery.h"
#include "dlgcouponservicedocumentplus.h"
#include "database2.h"
#include "doubledatabase.h"
#include "message.h"

#include <QDateTime>
#include <QMarginsF>
#include <QPageLayout>
#include <QPageSize>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextDocument>

DlgCouponServiceDocument::DlgCouponServiceDocument(ReportQuery *rq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServiceDocument)
{
    ui->setupUi(this);
    rq->costumizeCombo(ui->cbPartner, "partners");
    fReportQuery = rq;
    ui->tbl->setColumnWidth(8, 0);
    ui->leValid->setDate(QDate::currentDate().addDays(365));
}

DlgCouponServiceDocument::~DlgCouponServiceDocument()
{
    delete ui;
}

void DlgCouponServiceDocument::openDocument(int id)
{
    ui->leDoc->setInt(id);
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_id"] = id;
    db.exec("select * from talon_documents_header where f_id=:f_id");
    if (!db.next()) {
        return;
    }
    ui->leTotal->setDouble(db.doubleValue("f_amount"));
    ui->cbPartner->setCurrentIndex(ui->cbPartner->findData(db.integer("f_partner")));
    ui->leDate->setDate(db.date("f_date"));
    ui->cbPartner->setEnabled(false);
    ui->btnMinus->setEnabled(false);
    ui->btnPlus->setEnabled(false);
    ui->btnSave->setEnabled(false);
    db[":f_doc"] = ui->leDoc->asInt();
    db.exec("select * from talon_body where f_doc=:f_doc");
    while (db.next()) {
        int r = ui->tbl->rowCount();
        ui->tbl->setRowCount(r + 1);
        ui->tbl->setItem(r, 0, new QTableWidgetItem());
        ui->tbl->setItem(r, 1, new QTableWidgetItem(db.string("f_group")));
        ui->tbl->setItem(r, 2, new QTableWidgetItem(db.string("f_first")));
        ui->tbl->setItem(r, 3, new QTableWidgetItem(db.string("f_last")));
        ui->tbl->setItem(r, 4, new QTableWidgetItem(QString::number(db.doubleValue("f_qty"))));
        ui->tbl->setItem(r, 5, new QTableWidgetItem(QString::number(db.doubleValue("f_price"))));
        ui->tbl->setItem(r, 6, new QTableWidgetItem(QString::number(db.doubleValue("f_discount"))));
        ui->tbl->setItem(r, 7, new QTableWidgetItem(QString::number(db.doubleValue("f_total"))));
    }
}

void DlgCouponServiceDocument::on_btnPlus_clicked()
{
    DlgCouponServiceDocumentPlus d(fReportQuery, this);
    if (d.exec() == QDialog::Accepted) {
        QString couponType, first;
        double qty = 1, price, discount, total;
        int group;
        d.getResult(couponType, first, price, discount, total, group);
        int r = ui->tbl->rowCount();
        ui->tbl->setRowCount(r + 1);
        ui->tbl->setItem(r, 0, new QTableWidgetItem());
        ui->tbl->setItem(r, 1, new QTableWidgetItem(couponType));
        ui->tbl->setItem(r, 2, new QTableWidgetItem(first));
        ui->tbl->setItem(r, 3, new QTableWidgetItem(first));
        ui->tbl->setItem(r, 4, new QTableWidgetItem(QString::number(qty)));
        ui->tbl->setItem(r, 5, new QTableWidgetItem(QString::number(price)));
        ui->tbl->setItem(r, 6, new QTableWidgetItem(QString::number(discount)));
        ui->tbl->setItem(r, 7, new QTableWidgetItem(QString::number(total)));
        ui->tbl->setItem(r, 8, new QTableWidgetItem(QString::number(group)));
        countAmount();
    }
}

void DlgCouponServiceDocument::countAmount()
{
    double amount = 0;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        amount += ui->tbl->item(i, 7)->data(Qt::EditRole).toDouble();
    }
    ui->leTotal->setDouble(amount);
}

void DlgCouponServiceDocument::on_btnSave_clicked()
{
    if (ui->cbPartner->currentIndex() < 0) {
        message_error(tr("Partner is not selected"));
        return;
    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.startTransaction();
    db[":f_type"] = 2;
    db[":f_date"] = ui->leDate->date();
    db[":f_partner"] = ui->cbPartner->currentData();
    db[":f_amount"] = ui->leTotal->asDouble();
    int docid = 0;
    db.insert("talon_documents_header", docid);
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        db[":f_code"] = ui->tbl->item(i, 2)->text();
        db[":f_trsale"] = docid;
        db[":f_group"]  = ui->tbl->item(i, 8)->text().toInt();
        db[":f_partner"] = ui->cbPartner->currentData();
        db[":f_price"] = str_float(ui->tbl->item(i, 5)->text());
        db[":f_validto"] = ui->leValid->date();
        db[":f_discount"] = ui->tbl->item(i, 6)->data(Qt::EditRole).toDouble();
        db.exec("update talon_service set f_trsale=:f_trsale, f_discount=:f_discount, "
                "f_price=:f_price, f_trback=null, f_partner=:f_partner, f_group=:f_group, f_validto=:f_validto where f_code=:f_code");
        db[":f_doc"] = docid;
        db[":f_group"] = ui->tbl->item(i, 1)->text();
        db[":f_first"] = ui->tbl->item(i, 2)->text();
        db[":f_last"] = ui->tbl->item(i, 3)->text();
        db[":f_qty"] = str_float(ui->tbl->item(i, 4)->text());
        db[":f_price"] = str_float(ui->tbl->item(i, 5)->text());
        db[":f_discount"] = str_float(ui->tbl->item(i, 6)->text());
        db[":f_total"] = str_float(ui->tbl->item(i, 7)->text());
        db.insert("talon_body");
    }
    db[":f_type"] = 4;
    db[":f_date"] = ui->leDate->date();
    db[":f_partner"] = ui->cbPartner->currentData();
    db[":f_amount"] = ui->lePaid->asDouble() - ui->leTotal->asDouble();
    int refdocid = 0;
    db.insert("talon_documents_header", refdocid);
    db[":f_document"] = refdocid;
    db[":f_refdocument"] = docid;
    db[":f_debit"] = ui->lePaid->asDouble();
    db[":f_credit"] = ui->leTotal->asDouble();
    db.insert("talon_payment");
    db.commit();
    ui->leDoc->setInt(docid);
    ui->leDate->setEnabled(false);
    ui->cbPartner->setEnabled(false);
    ui->btnMinus->setEnabled(false);
    ui->btnPlus->setEnabled(false);
    ui->btnSave->setEnabled(false);
    message_info(tr("Saved"));
}

void DlgCouponServiceDocument::on_btnPrint_clicked()
{
    if (ui->leDoc->asInt() == 0) {
        message_error(tr("Document wasnt saved"));
        return;
    }
    const auto esc = [](const QString &s) { return s.toHtmlEscaped(); };

    QString tableRows;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        tableRows += QStringLiteral("<tr>"
                                    "<td align='center'>%1</td>"
                                    "<td>%2</td>"
                                    "<td align='right'>%3</td>"
                                    "<td align='right'>%4</td>"
                                    "<td align='right'>%5</td>"
                                    "<td align='right'>%6</td>"
                                    "</tr>")
                .arg(i + 1)
                .arg(esc(ui->tbl->item(i, 1)->text()))
                .arg(esc(ui->tbl->item(i, 4)->text()))
                .arg(esc(ui->tbl->item(i, 5)->text()))
                .arg(esc(ui->tbl->item(i, 6)->text()))
                .arg(esc(ui->tbl->item(i, 7)->text()));
    }

    const QString dateLine = esc(QString("%1 %2").arg(tr("Date"), ui->leDate->text()));
    const QString partner = esc(ui->cbPartner->currentText());
    const QString paid = esc(ui->lePaid->text());
    const QString total = esc(ui->leTotal->text());
    const QString printed = esc(QString("%1 %2")
                                .arg(tr("Printed:"), QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")));

    const QString col = QString(
        "<div style='font-size:9pt;'>"
        "<p style='text-align:center;font-size:14pt;font-weight:bold;margin:0.2em 0;'>%1</p>"
        "<p style='text-align:center;margin:0.15em 0;'>%2</p>"
        "<p style='text-align:center;margin:0.15em 0;'>%3</p>"
        "<p style='text-align:center;margin:0.2em 0;'>%4</p>"

        "<p style='margin:0.3em 0;'><b>%5</b> <span style='display:inline-block;min-width:12em;border-bottom:1px solid #000;'>%6</span></p>"
        "<p style='margin:0.3em 0;'><b>%7</b> <span style='display:inline-block;min-width:12em;border-bottom:1px solid #000;'>%8</span></p>"
        "<p style='margin:0.3em 0;'><b>%9</b> <span style='display:inline-block;min-width:12em;border-bottom:1px solid #000;'>%10</span></p>"

        "<table style='width:100%;border-collapse:collapse;border:1px solid #000;font-size:8pt;margin:0.5em 0;'>"
        "<thead><tr style='font-weight:bold;'>"
        "<th style='border:1px solid #000;'>%11</th>"
        "<th style='border:1px solid #000;'>%12</th>"
        "<th style='border:1px solid #000;'>%13</th>"
        "<th style='border:1px solid #000;'>%14</th>"
        "<th style='border:1px solid #000;'>%15</th>"
        "<th style='border:1px solid #000;'>%16</th>"
        "</tr></thead><tbody>"
        "%17"
        "<tr><td colspan='4' style='border:1px solid #000;'></td><td align='right' style='border:1px solid #000;'><b>%18</b></td>"
        "<td align='right' style='border:1px solid #000;'><b>%19</b></td></tr>"
        "</tbody></table>"

        "<p style='margin:1.2em 0 0.2em;'><b>%20</b> <span style='display:inline-block;min-width:5em;border-bottom:1px solid #000;'></span>"
        " &nbsp; <b>%21</b> <span style='display:inline-block;min-width:5em;border-bottom:1px solid #000;'></span></p>"
        "<p style='margin:0.1em 0 0.8em;'><span style='margin-right:1.5em;'>%22</span><span>%23</span></p>"

        "<p style='margin:0.2em 0;'>%24</p>"
        "</div>")
        .arg(esc(tr("<<ARMPETROL>> LTD")))
        .arg(esc(tr("DEAL")))
        .arg(esc(tr("ACCEPTANCE - DELIVERY")))
        .arg(dateLine)
        .arg(esc(tr("Receiver:")))
        .arg(partner)
        .arg(esc(tr("Payment:")))
        .arg(paid)
        .arg(esc(tr("Paid:")))
        .arg(paid)
        .arg(esc(tr("NN")))
        .arg(esc(tr("Name")))
        .arg(esc(tr("Quantity")))
        .arg(esc(tr("Price")))
        .arg(esc(tr("Discount")))
        .arg(esc(tr("Amount")))
        .arg(tableRows)
        .arg(esc(tr("Total")))
        .arg(total)
        .arg(esc(tr("Receiver:")))
        .arg(esc(tr("Deliver:")))
        .arg(esc(tr("(signature)")))
        .arg(esc(tr("(signature)")))
        .arg(printed);

    const QString html = QStringLiteral("<!DOCTYPE html><html><head><meta charset='utf-8'/></head><body style='font-family:Arial,Helvetica,sans-serif;'>"
                                        "<table style='width:100%;table-layout:fixed;'><tr>"
                                        "<td style='width:50%;vertical-align:top;padding:0 0.3em;'>")
        + col
        + QStringLiteral("</td><td style='width:50%;vertical-align:top;padding:0 0.3em;'>")
        + col
        + QStringLiteral("</td></tr></table></body></html>");

    QPrinter printer(QPrinter::HighResolution);
    {
        QPageLayout layout = printer.pageLayout();
        layout.setPageSize(QPageSize(QPageSize::A4));
        layout.setUnits(QPageLayout::Millimeter);
        layout.setMargins(QMarginsF(10, 8, 10, 8));
        layout.setOrientation(QPageLayout::Landscape);
        printer.setPageLayout(layout);
    }

    QPrintPreviewDialog preview(&printer, this);
    QTextDocument document;
    document.setHtml(html);
    QObject::connect(&preview, &QPrintPreviewDialog::paintRequested, [&](QPrinter *p) {
        document.setHtml(html);
        const QRectF rect = p->pageLayout().paintRectPixels(p->resolution());
        document.setPageSize(rect.size());
        document.setTextWidth(rect.width());
        document.print(p);
    });
    preview.exec();
}

void DlgCouponServiceDocument::on_btnMinus_clicked()
{
    int r = ui->tbl->currentRow();
    if (r > -1) {
        ui->tbl->removeRow(r);
        countAmount();
    }
}

void DlgCouponServiceDocument::on_lePaid_textChanged(const QString &arg1)
{
    if (arg1.toDouble() > ui->leTotal->asDouble()) {
        ui->lePaid->setText(ui->leTotal->text());
    }
}
