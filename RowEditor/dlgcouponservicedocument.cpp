#include "dlgcouponservicedocument.h"
#include "ui_dlgcouponservicedocument.h"
#include "reportquery.h"
#include "dlgcouponservicedocumentplus.h"
#include "database2.h"
#include "c5printing.h"
#include "doubledatabase.h"
#include "c5printpreview.h"
#include "message.h"

DlgCouponServiceDocument::DlgCouponServiceDocument(ReportQuery *rq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServiceDocument)
{
    ui->setupUi(this);
    rq->costumizeCombo(ui->cbPartner, "partners");
    fReportQuery = rq;
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
        QString couponType, first, last;
        double qty, price, discount, total;
        d.getResult(couponType, first, last, qty, price, discount, total);
        int r = ui->tbl->rowCount();
        ui->tbl->setRowCount(r + 1);
        ui->tbl->setItem(r, 0, new QTableWidgetItem());
        ui->tbl->setItem(r, 1, new QTableWidgetItem(couponType));
        ui->tbl->setItem(r, 2, new QTableWidgetItem(first));
        ui->tbl->setItem(r, 3, new QTableWidgetItem(last));
        ui->tbl->setItem(r, 4, new QTableWidgetItem(QString::number(qty)));
        ui->tbl->setItem(r, 5, new QTableWidgetItem(QString::number(price)));
        ui->tbl->setItem(r, 6, new QTableWidgetItem(QString::number(discount)));
        ui->tbl->setItem(r, 7, new QTableWidgetItem(QString::number(total)));
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
        db.exec("select f_id from talon_service where f_code=:f_code");
        db.next(true);
        int firstId = db.integer("f_id");
        db[":f_code"] = ui->tbl->item(i, 3)->text();
        db.exec("select f_id from talon_service where f_code=:f_code");
        db.next(true);
        int lastId = db.integer("f_id");
        db[":f_id1"] = firstId;
        db[":f_id2"] = lastId;
        if (!db.exec("select f_id, f_trsale from talon_service where f_id between :f_id1 and :f_id2 for update")) {
            message_error(tr("Document save error") + "\r\n" + db.lastDbError());
            db.rollback();
            return;
        }
        QList<QMap<QString, QVariant> > datacheck;
        while (db.next()) {
            QMap<QString, QVariant> d;
            d["id"] = db.integer("f_id");
            d["tr"] = db.integer("f_trsale");
            datacheck.append(d);
        }
        for (int j = 0; j < datacheck.count(); j++) {
            const QMap<QString, QVariant> &d = datacheck.at(j);
            if (d["tr"].toInt() > 0) {
                message_error(tr("Some coupons already saled. Check document"));
                db.rollback();
                return;
            }
            db[":f_id"] = d["id"];
            db[":f_trsale"] = docid;
            db[":f_discount"] = ui->tbl->item(i, 6)->data(Qt::EditRole).toDouble();
            db.exec("update talon_service set f_trsale=:f_trsale, f_discount=:f_discount where f_id=:f_id");
        }
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

    C5Printing p;
    p.setSceneParams(2800, 2000, QPrinter::Landscape);
    QFont font(font());
    int fontbase = 26;
    font.setPointSize(fontbase);
    //font.setBold(true);
    p.setFont(font);
    int baseleft = 100;
    int sl = 1400 + baseleft;
    int cl = (2700 / 2) / 2;
    int cr = (2700 / 2) + cl;
    p.setFontSize(fontbase + 4);
    p.setFontBold(true);
    p.ltext(tr("<<ARMPETROL>> LTD"), baseleft);
    p.ltext(tr("<<ARMPETROL>> LTD"), baseleft + sl);
    p.br();
    p.setFontSize(fontbase);
    p.setFontBold(false);
    p.ctextof(tr("DEAL"), cl);
    p.ctextof(tr("DEAL"), cr);
    p.br();
    p.ctextof(tr("ACCEPTANCE - DELIVERY"), cl);
    p.ctextof(tr("ACCEPTANCE - DELIVERY"), cr);
    p.br();
    QString strTemp = QString("%1 %2").arg(tr("Date"), ui->leDate->text());
    p.ctextof(strTemp, cl);
    p.ctextof(strTemp, cr);
    p.br();
    p.ltext(tr("Receiver:"), baseleft);
    p.ltext(tr("Receiver:"), baseleft + sl);
    p.line(baseleft + 300, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 300, p.fTop + p.fLineHeight, sl + 1200, p.fTop + p.fLineHeight);
    p.ltext(ui->cbPartner->currentText(), baseleft + 450);
    p.ltext(ui->cbPartner->currentText(), baseleft + sl + 450);
    p.br();
    p.ltext(tr("Payment:"), baseleft);
    p.ltext(tr("Payment:"), baseleft + sl);
    p.line(baseleft + 300, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 300, p.fTop + p.fLineHeight, baseleft + sl + 1200, p.fTop + p.fLineHeight);
    p.ltext(ui->lePaid->text(), baseleft + 450);
    p.ltext(ui->lePaid->text(), baseleft + sl + 450);
    p.br();
    p.ltext(tr("Paid:"), baseleft);
    p.ltext(tr("Paid:"), baseleft + sl);
    p.line(baseleft + 300, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 300, p.fTop + p.fLineHeight, sl + 1200, p.fTop + p.fLineHeight);
    p.ltext(ui->lePaid->text(), 450);
    p.ltext(ui->lePaid->text(), sl + 450);
    p.br();
    p.br();

    QList<qreal> points;
    points << baseleft << 100 << 450 << 150 << 150 << 150 << 250;
    QList<qreal> points2 = points;
    points2[0] = points2[0] + sl;
    QStringList vals;
    vals << tr("NN") << tr("Name") << tr("Quantity") << tr("Price") <<  tr("Discount") << tr("Amount");
    p.tableText(points, vals, p.fLineHeight + 30);
    p.tableText(points2, vals, p.fLineHeight + 30);
    p.br(p.fLineHeight + 30);


    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        vals.clear();
        vals << QString::number(i + 1)
             << ui->tbl->item(i, 1)->text()
             << ui->tbl->item(i, 4)->text()
             << ui->tbl->item(i, 5)->text()
             << ui->tbl->item(i, 6)->text()
             << ui->tbl->item(i, 7)->text();
        p.tableText(points, vals, p.fLineHeight + 30);
        p.tableText(points2, vals, p.fLineHeight + 30);
        p.br(p.fLineHeight + 30);
    }
    points.clear();
    points << baseleft + 700 << 300 << 250;
    points2 = points;
    points2[0] = points2[0] + sl;
    vals.clear();
    vals << tr("Total") << ui->leTotal->text();
    p.tableText(points, vals, p.fLineHeight + 30);
    p.tableText(points2, vals, p.fLineHeight + 20);
    p.br(p.fLineHeight + 20);
    p.br();
    p.br();
    p.br(p.fLineHeight + 20);

    p.ltext(tr("Receiver:"), baseleft);
    p.ltext(tr("Receiver:"), baseleft + sl);
    p.ltext(tr("Deliver:"), baseleft + 750);
    p.ltext(tr("Deliver:"), baseleft + sl + 750);
    p.line(baseleft, p.fTop + p.fLineHeight, 500, p.fTop + p.fLineHeight);
    p.line(baseleft + sl, p.fTop + p.fLineHeight, sl + 500, p.fTop + p.fLineHeight);
    p.line(baseleft + 750, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 750, p.fTop + p.fLineHeight, sl + 1200, p.fTop + p.fLineHeight);
    p.br();
    p.ltext(tr("(signature)"), baseleft + 200);
    p.ltext(tr("(signature)"), baseleft + sl + 200);
    p.ltext(tr("(signature)"), baseleft + 900);
    p.ltext(tr("(signature)"), baseleft + sl + 900);
    p.br();
    p.br();
    p.ltext(QString("%1 %2").arg(tr("Printed:"), QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")), baseleft);
    p.ltext(QString("%1 %2").arg(tr("Printed:"), QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")), sl + baseleft);

    C5PrintPreview pp(&p, this);
    pp.exec();
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
