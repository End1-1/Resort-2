#include "dlgpayment.h"
#include "ui_dlgpayment.h"
#include "rmessage.h"
#include "dlglist.h"
#include "rnumbers.h"
#include "cachecouponseria.h"
#include "dlgdeptholder.h"
#include "database2.h"
#include "printtaxn.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QInputDialog>
#include <QJsonParseError>

DlgPayment::DlgPayment(int order, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPayment)
{
    ui->setupUi(this);
    fCannotContinue = false;
    fUpdateHeader = false;
    fCanReject = true;
    ui->btnPrintTax->setChecked(true);
    ui->btnPrintTax->setStyleSheet("QPushButton:checked {background-color:green};");
    ui->btnCouponService->setStyleSheet("QPushButton:checked {background-color:green};");
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;

    if(!db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        QMessageBox::critical(0, "Db error", db2.lastDbError());
        exit(0);
    }

    bool hasgift = false;
    bool hasnogift = false;
    db2[":f_order"] = order;
    db2.exec("select t.f_id "
             "from o_dish od "
             "left join r_dish d on d.f_id=od.f_dish "
             "left join r_dish_type t on t.f_id=d.f_type "
             "where od.f_header=:f_order");

    while(db2.next()) {
        if(db2.integer("f_id") == 75) {
            hasgift = true;
        } else {
            hasnogift = true;
        }
    }

    if(hasgift && hasnogift) {
        message_error("Անթուլատրելի վաճառքի տեսակ");
        fCannotContinue = true;
    }

    ui->btnPrepaid->setEnabled(hasgift);
    ui->leGiftCardCode->setEnabled(hasgift);
    ui->leDiscount->setEnabled(!hasgift);
    ui->btnOk->setEnabled(!hasgift);

    if(hasgift) {
        ui->btnDeptHolder->setEnabled(true);
    }
}

DlgPayment::~DlgPayment()
{
    delete ui;
}

bool DlgPayment::payment(int order, int hallid)
{
    bool result;
    DlgPayment *d = new DlgPayment(order);

    if(d->fCannotContinue) {
        d->deleteLater();
        return false;
    }

    d->fOrder = order;
    d->fHall = hallid;
    d->readFiscalMachines();
    DatabaseResult dr;
    d->fDbBind[":f_id"] = order;
    dr.select(d->fDb, "select f_servicevalue from o_header where f_id=:f_id", d->fDbBind);
    d->fDbBind[":f_id"] = order;
    dr.select(d->fDb, "select f_id from o_header_payment where f_id=:f_id", d->fDbBind);
    d->ui->leCard->setDouble(0);

    if(dr.rowCount() == 0) {
        d->fDbBind[":f_header"] = order;
        d->fDbBind[":f_state"] = DISH_STATE_READY;
        dr.select(d->fDb,
                  "select sum(f_svcamount) as f_svcamount, sum(f_total) as f_total from o_dish where f_header=:f_header and f_state=:f_state",
                  d->fDbBind);

        if(dr.rowCount() > 0) {
            d->ui->leFinalAmount->setDouble(dr.value("f_total").toDouble());
        }
    } else {
        d->fUpdateHeader = true;
    }

    d->getGiftAmount();
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;

    if(!db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        QMessageBox::critical(0, "Db error", db2.lastDbError());
        exit(0);
    }

    db2[":f_id"] = order;
    db2.exec("select * from o_header where f_id=:f_id");
    db2.next();
    d->ui->leFiscalNumber->setText(db2.string("f_tax"));

    if(d->ui->leFiscalNumber->asInt() > 0) {
        d->ui->btnPrintTax->setEnabled(false);
        d->ui->btnPrintTax->setChecked(false);
    }

    result = d->exec() == QDialog::Accepted;
    delete d;
    return result;
}

void DlgPayment::accept()
{
    BaseExtendedDialog::accept();
}

void DlgPayment::reject()
{
    if(fCanReject) {
        BaseExtendedDialog::reject();
    }
}

void DlgPayment::on_btnCancel_clicked()
{
    reject();
}

void DlgPayment::on_btnOk_clicked()
{
    if(ui->leCard->asDouble()
            + ui->leCash->asDouble()
            + ui->leCouponAmount->asDouble()
            + ui->leIdram->asDouble()
            + ui->leDept->asDouble() < ui->leFinalAmount->asDouble()) {
        message_error(tr("Payment not complete"));
        return;
    }

    if(ui->leCard->asDouble()
            + ui->leCash->asDouble()
            + ui->leIdram->asDouble()
            + ui->leCouponAmount->asDouble()
            + ui->leDept->asDouble() > ui->leFinalAmount->asDouble()) {
        message_error(tr("Amount greater than need to pay"));
        return;
    }

    QString sn, firm, address, fiscal, hvhh, rseq, devnum, time;
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    QList<int> idForUpdateFiscal;
    int fiscalNumber = 0;

    if(ui->btnPrintTax->isChecked()) {
        QMap<QString, QVariant> s = fFiscalMachines[ui->cbFiscalMachine->currentText()];
        PrintTaxN pn(s.value("ip").toString(),
                     s.value("port").toInt(),
                     s.value("password").toString(),
                     s.value("extpos").toString(),
                     s.value("opcode").toString(),
                     s.value("oppin").toString());
        db2[":f_header"] = fOrder;
        db2[":f_state"] = DISH_STATE_READY;
        db2.exec("select d.f_en, d.f_adgt, od.f_qty, od.f_price, od.f_dctvalue, d.f_taxdebt, d.f_id, od.f_emark, od.f_id as f_od_id "
                 "from o_dish od "
                 "left join r_dish d on d.f_id=od.f_dish "
                 "where od.f_header=:f_header and od.f_state=:f_state ");

        while(db2.next()) {
            if(db2.doubleValue("f_price") < 0.01) {
                continue;
            }

            if(db2.string("f_emark").isEmpty() == false) {
                pn.fEmarks.append(db2.string("f_emark"));
            }

            idForUpdateFiscal.append(db2.integer("f_od_id"));
            pn.addGoods(db2.string("f_taxdebt").toInt(),
                        db2.string("f_adgt"),
                        db2.string("f_id"),
                        db2.string("f_name"),
                        db2.doubleValue("f_price"),
                        db2.doubleValue("f_qty"),
                        db2.doubleValue("f_dctvalue"));
        }

        if(ui->leTaxpayerId->asInt() > 0) {
            pn.fPartnerTin = ui->leTaxpayerId->text();
        }

        QString in, out, err;
        int fiscalrecid;
        int result = pn.makeJsonAndPrint(ui->leCard->asDouble() + ui->leIdram->asDouble(),
                                         ui->leGiftFiscal->text().isEmpty() ? 0 : ui->leCouponAmount->asDouble(),
                                         in, out, err);
#ifdef QT_DEBUG
        out =
            "{\"rseq\":77,\"crn\":\"63219817\",\"sn\":\"V98745506068\",\"tin\":\"01588771\",\"taxpayer\":\"«Ռոգա էնդ կոպիտա ՍՊԸ»\",\"address\":\"Արշակունյանց 34\",\"time\":1676794194840,\"fiscal\":\"98198105\",\"lottery\":\"00000000\",\"prize\":0,\"total\":1540.0,\"change\":0.0}";
        out =
            "{\"address\":\"ԿԵՆՏՐՈՆ ԹԱՂԱՄԱՍ Ամիրյան 4/3 \",\"change\":0.0,\"crn\":\"53235782\",\"fiscal\":\"54704153\",\"lottery\":\"\",\"prize\":0,\"rseq\":1327,\"sn\":\"00022154380\",\"taxpayer\":\"«ՊԼԱԶԱ ՍԻՍՏԵՄՍ»\",\"time\":1709630105632,\"tin\":\"02596277\",\"total\":93600.0}";
        result = 0;
#endif
        db2[":f_order"] = fOrder;
        db2[":f_in"] = QByteArray(in.toUtf8()).toBase64();
        db2[":f_out"] = out;
        db2[":f_err"] = err;
        db2.insert("o_tax_log", fiscalrecid);

        if(result != pt_err_ok) {
            message_error(tr("Fiscal error.") + "\r\n" + err);
            return;
        }

        QJsonObject jo = QJsonDocument::fromJson(out.toUtf8()).object();
        fiscalNumber = jo["rseq"].toInt();
        db2[":f_fiscal"] = jo["rseq"].toInt();
        db2.update("o_tax_log", "f_id", fiscalrecid);
        db2[":f_tax"] = jo["rseq"].toInt();
        db2.update("o_header", "f_id", fOrder);
    } else {
        // PRINT ALL EXCEPT AVTOLVACUM
        QMap<QString, QVariant> s = fFiscalMachines[ui->cbFiscalMachine->currentText()];
        PrintTaxN pn(s.value("ip").toString(),
                     s.value("port").toInt(),
                     s.value("password").toString(),
                     s.value("extpos").toString(),
                     s.value("opcode").toString(),
                     s.value("oppin").toString());
        db2[":f_header"] = fOrder;
        db2[":f_state"] = DISH_STATE_READY;
        db2.exec(R"(
            select distinct(od.f_id) as f_id, d.f_en, d.f_adgt, od.f_qty,
            od.f_price, od.f_dctvalue, d.f_taxdebt, d.f_id, od.f_emark, od.f_id as f_od_id
            from o_dish od
            left join r_dish d on d.f_id=od.f_dish
            inner join r_menu m on m.f_dish=d.f_id and m.f_menu in (2,3)  and m.f_state=1
            where od.f_header=:f_header and od.f_state=:f_state
    union
            select distinct(od.f_id) as f_id, d.f_en, d.f_adgt, od.f_qty,
            od.f_price, od.f_dctvalue, d.f_taxdebt, d.f_id, od.f_emark, od.f_id as f_od_id
            from o_dish od
            left join r_dish d on d.f_id=od.f_dish
            inner join r_menu m on m.f_dish=d.f_id
            where od.f_header=:f_header and od.f_state=:f_state and od.f_dish=558
)");
        bool f = false;

        while(db2.next()) {
            if(db2.doubleValue("f_price") < 0.01) {
                continue;
            }

            if(db2.string("f_emark").isEmpty() == false) {
                pn.fEmarks.append(db2.string("f_emark"));
            }

            f = true;
            idForUpdateFiscal.append(db2.integer("f_od_id"));
            pn.addGoods(db2.string("f_taxdebt").toInt(),
                        db2.string("f_adgt"),
                        QString::number(db2.integer("f_id")),
                        db2.string("f_en"),
                        db2.doubleValue("f_price"),
                        db2.doubleValue("f_qty"),
                        db2.doubleValue("f_dctvalue"));
        }

        if(ui->leTaxpayerId->asInt() > 0) {
            pn.fPartnerTin = ui->leTaxpayerId->text();
        }

        if(f) {
            QString in, out, err;
            int fiscalrecid;
            int result = pn.makeJsonAndPrint(ui->leCard->asDouble() + ui->leIdram->asDouble(),
                                             ui->leGiftFiscal->text().isEmpty() ? 0 : ui->leCouponAmount->asDouble(),
                                             in, out, err);
#ifdef QT_DEBUG
            out =
                "{\"rseq\":77,\"crn\":\"63219817\",\"sn\":\"V98745506068\",\"tin\":\"01588771\",\"taxpayer\":\"«Ռոգա էնդ կոպիտա ՍՊԸ»\",\"address\":\"Արշակունյանց 34\",\"time\":1676794194840,\"fiscal\":\"98198105\",\"lottery\":\"00000000\",\"prize\":0,\"total\":1540.0,\"change\":0.0}";
            result = 0;
#endif
            db2[":f_order"] = fOrder;
            db2[":f_in"] = QByteArray(in.toUtf8()).toBase64();
            db2[":f_out"] = out;
            db2[":f_err"] = err;
            db2.insert("o_tax_log", fiscalrecid);

            if(result != pt_err_ok) {
                message_error(tr("Fiscal error.") + "\r\n" + err);
                return;
            }

            QJsonObject jo = QJsonDocument::fromJson(out.toUtf8()).object();
            fiscalNumber = jo["rseq"].toInt();
            db2[":f_fiscal"] = jo["rseq"].toInt();
            db2.update("o_tax_log", "f_id", fiscalrecid);
            db2[":f_tax"] = jo["rseq"].toInt();
            db2.update("o_header", "f_id", fOrder);
        }
    }

    for (auto fn: idForUpdateFiscal) {
        db2[":f_fiscal"] = fiscalNumber;
        db2[":f_id"] = fn;
        db2.exec("update o_dish set f_fiscal=:f_fiscal where f_id=:f_id");
    }

    fDbBind[":f_id"] = fOrder;
    DatabaseResult drDisc;
    drDisc.select(fDb, "select * from o_temp_disc where f_id=:f_id", fDbBind);

    if(ui->leDept->asDouble() > 0.1) {
        if(ui->leDeptHolder->fHiddenText.toInt() == 0) {
            message_error(tr("Costumer is not defined"));
            return;
        }
    }

    fDbBind[":f_cash"] = (ui->btnCouponService->isChecked() || ui->btnOldTalon->isChecked()) ? 0 : ui->leCash->asDouble();
    fDbBind[":f_idram"] = ui->leIdram->asDouble();
    fDbBind[":f_card"] = ui->leCard->asDouble();
    fDbBind[":f_couponservice"] = (ui->btnCouponService->isChecked() || ui->btnOldTalon->isChecked()) ? ui->leCash->asDouble() : 0;
    fDbBind[":f_coupon"] = ui->leCouponAmount->asDouble();
    fDbBind[":f_discount"] = ui->leDiscountAmount->asDouble();
    fDbBind[":f_couponSeria"] = ui->leCouponSerial->text();
    fDbBind[":f_couponNumber"] = ui->leCouponNumber->text();
    fDbBind[":f_discountCard"] = ui->leCouponNumber->property("card");
    fDbBind[":f_costumer"] = ui->leCardHolder->fHiddenText.toInt();
    fDbBind[":f_finalAmount"] = ui->leFinalAmount->asDouble();
    fDbBind[":f_debt"] = ui->leDept->asDouble();
    fDbBind[":f_debtHolder"] = ui->leDeptHolder->fHiddenText.toInt();

    if(drDisc.rowCount() > 0) {
        fDbBind[":f_discount"] = drDisc.value("f_amount");
        fDbBind[":f_costumer"] = drDisc.value("f_costumer");
    }

    if(fUpdateHeader) {
        fDb.update("o_header_payment", fDbBind, where_id(fOrder));
    } else {
        fDbBind[":f_id"] = fOrder;

        if(!fDb.insertWithoutId("o_header_payment", fDbBind)) {
            message_error(fDb.fLastError);
        }
    }

    if(ui->leDept->asDouble() > 0) {
        fDbBind[":f_id"] = fOrder;
        fDbBind[":f_holder"] = ui->leDeptHolder->fHiddenText.toInt();
        fDbBind[":f_debt"] = ui->leDept->asDouble();
        fDb.insertWithoutId("o_debts", fDbBind);
    }

    if(ui->leCouponNumber->asInt() > 0) {
        fDbBind[":f_seria"] = ui->leCouponSerial->fHiddenText.toInt();
        fDbBind[":f_number"] = ui->leCouponNumber->asInt();
        fDb.select("update d_coupon set f_used=1 where f_seria=:f_seria and cast(f_number as signed)=:f_number", fDbBind,
                   fDbRows);
    }

    accept();
}

void DlgPayment::on_btnCard_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_order"] = fOrder;
    db2.exec("delete from d_gift_cart_use where f_order=:f_order");
    calcCard();
}

void DlgPayment::calcCash()
{
    ui->leCash->setText(ui->leFinalAmount->text());
    ui->leCard->setText("0");
    ui->leCouponAmount->setText("0");
    ui->leIdram->setText("0");
    ui->leDept->setText("0");
    ui->leCouponNumber->clear();
    ui->leCouponSerial->clear();
    ui->leCouponNumber->fHiddenText.clear();
    ui->leCouponSerial->fHiddenText.clear();
    ui->leDiscount->clear();
    ui->leDiscountAmount->setText("0");
    ui->leCardHolder->clear();
    ui->leCardHolder->fHiddenText.clear();

    if(!ui->btnCouponService->isChecked()) {
        if(message_question(tr("Print?"))) {
            ui->btnPrintTax->setChecked(true);
        } else {
            ui->btnPrintTax->setChecked(false);
        }
    }
}

void DlgPayment::calcDebt()
{
    ui->leDept->setText(ui->leFinalAmount->text());
    ui->leCard->setText("0");
    ui->leCouponAmount->setText("0");
    ui->leCash->setText("0");
    ui->leIdram->setText("0");
    ui->leCouponNumber->clear();
    ui->leCouponSerial->clear();
    ui->leCouponNumber->fHiddenText.clear();
    ui->leCouponSerial->fHiddenText.clear();
    ui->leDiscount->clear();
    ui->leDiscountAmount->setText("0");
    ui->leCardHolder->clear();
    ui->leCardHolder->fHiddenText.clear();
    ui->btnPrintTax->setChecked(false);
}

void DlgPayment::calcCard()
{
    ui->leCard->setText(ui->leFinalAmount->text());
    ui->leCash->setText("0");
    ui->leCouponAmount->setText("0");
    ui->leDept->setText("0");
    ui->leCouponNumber->clear();
    ui->leCouponSerial->clear();
    ui->leCouponNumber->fHiddenText.clear();
    ui->leCouponSerial->fHiddenText.clear();
    ui->leDiscount->clear();
    ui->leDiscountAmount->setText("0");
    ui->leCardHolder->clear();
    ui->leCardHolder->fHiddenText.clear();
    ui->btnPrintTax->setChecked(true);
}

void DlgPayment::calcIdram()
{
    ui->leIdram->setText(ui->leFinalAmount->text());
    ui->leCard->setText("0");
    ui->leCash->setText("0");
    ui->leCouponAmount->setText("0");
    ui->leDept->setText("0");
    ui->leCouponNumber->clear();
    ui->leCouponSerial->clear();
    ui->leCouponNumber->fHiddenText.clear();
    ui->leCouponSerial->fHiddenText.clear();
    ui->leDiscount->clear();
    ui->leDiscountAmount->setText("0");
    ui->leCardHolder->clear();
    ui->leCardHolder->fHiddenText.clear();
    ui->btnPrintTax->setChecked(true);
}

void DlgPayment::readFiscalMachines()
{
    QSettings s(QString("%1\\fiscal.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);
    QStringList groups = s.childGroups();

    for(const QString &g : qAsConst(groups)) {
        ui->cbFiscalMachine->addItem(g);
        s.beginGroup(g);

        if(s.value("default").toBool()) {
            fDefaultFiscalMachine = g;
        }

        QStringList keys = s.childKeys();

        for(const QString &k : keys) {
            fFiscalMachines[g][k] = s.value(k);
        }

        s.endGroup();
    }

    ui->cbFiscalMachine->setCurrentIndex(-1);

    for(QMap<QString, QMap<QString, QVariant> >::const_iterator it = fFiscalMachines.constBegin();
            it != fFiscalMachines.constEnd(); it++) {
        if(it.value()["hall"].toInt() == fHall) {
            ui->cbFiscalMachine->setCurrentIndex(ui->cbFiscalMachine->findText(it.key()));
            return;
        }
    }

    if(ui->cbFiscalMachine->count() == 1) {
        ui->cbFiscalMachine->setCurrentIndex(0);
    }

    if(ui->cbFiscalMachine->currentIndex() == -1 && !fDefaultFiscalMachine.isEmpty()) {
        ui->cbFiscalMachine->setCurrentIndex(ui->cbFiscalMachine->findText(fDefaultFiscalMachine));
    }
}

void DlgPayment::getGiftAmount()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;

    if(!db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        QMessageBox::critical(0, "Db error", db2.lastDbError());
        exit(0);
    }

    db2[":f_order"] = fOrder;
    db2.exec("select abs(sum(f_amount)) as f_amount from d_gift_cart_use where f_order=:f_order");

    if(db2.next()) {
        ui->leCouponAmount->setDouble(db2.doubleValue("f_amount"));

        if(db2.doubleValue("f_amount") > 0.01) {
            ui->leCash->setDouble(ui->leFinalAmount->asDouble() - ui->leCouponAmount->asDouble());
        }

        if(ui->leCash->asDouble() < 0) {
            ui->leCash->setDouble(0);
        }

        db2[":f_order"] = fOrder;
        db2.exec("select distinct(g.f_info) as f_info, u.f_code "
                 "from d_gift_cart_use u "
                 "left join d_gift_cart g on g.f_code=u.f_code "
                 "where u.f_order=:f_order");
        QString name, code;

        while(db2.next()) {
            if(!name.isEmpty()) {
                name += ",";
                code += ",";
            }

            name += db2.string("f_info");
            code += db2.string("f_code");
        }

        ui->leCouponNumber->setText(name);
        ui->leCouponNumber->setProperty("cart", code);
        db2[":f_order"] = fOrder;
        db2.exec("select group_concat(right(f_code, 4)) as f_code from d_gift_cart_use where f_order=:f_order");

        if(db2.next()) {
            ui->leGiftFiscal->setText(db2.string("f_code"));
        }
    }
}

void DlgPayment::on_btnDept_clicked()
{
    calcDebt();
}

void DlgPayment::on_leDept_textChanged(const QString &arg1)
{
    ui->btnDeptHolder->setEnabled(arg1.toDouble() > 0.1);
}

void DlgPayment::on_btnDeptHolder_clicked()
{
    int debtId;
    QString debtName;

    if(!DlgDeptHolder::getHolder(debtId, debtName)) {
        return;
    }

    ui->leDeptHolder->setText(debtName);
    ui->leDeptHolder->fHiddenText = QString::number(debtId);
}

void DlgPayment::on_btnCouponNumber_clicked()
{
    if(ui->leCouponSerial->isEmpty()) {
        message_error(tr("Coupon seria is not defined"));
        return;
    }

    float num = 0;

    if(RNumbers::getFloat(num, 0, "ԿՏՐՈՆԻ ԿՈԴ", this)) {
        ui->leCouponNumber->setText(QString::number(num, 'f', 0));
    }

    DatabaseResult dr;
    fDbBind[":f_seria"] = ui->leCouponSerial->fHiddenText.toInt();
    fDbBind[":f_number"] = ui->leCouponNumber->asInt();
    dr.select(fDb,
              "select f_id, f_value from d_coupon where f_seria=:f_seria and cast(f_number as signed)=:f_number and f_used=0",
              fDbBind);

    if(dr.rowCount() == 0) {
        message_error(tr("Invalid coupon number"));
        ui->leCouponNumber->clear();
        return;
    }

    CI_CouponSeria *ci = CacheCouponSeria::instance()->get(ui->leCouponSerial->fHiddenText);

    if(!ci) {
        message_error(tr("Invalide coupon seria"));
        return;
    }

    ui->leCouponAmount->setText(float_str(ci->fPrice, 2));
    fCountId = dr.value("f_id").toInt();
    calcCash();
}

void DlgPayment::on_btnCouponSerail_clicked()
{
    if(ui->leCardHolder->asInt() > 0) {
        message_error(tr("Cannot use coupon with discount"));
        return;
    }

    QString name;
    QVariant res;

    if(!DlgList::getValue(tr("Coupon seria"), name, res, "select f_id, f_name from d_coupon_seria ")) {
        return;
    }

    CI_CouponSeria *c = CacheCouponSeria::instance()->get(res.toString());

    if(!c) {
        message_error(tr("Invalid coupon seria"));
        return;
    }

    DatabaseResult dr;
    fDbBind[":f_header"] = fOrder;
    fDbBind[":f_state"] = DISH_STATE_READY;
    dr.select(fDb, "select f_dish from o_dish where f_header=:f_header and f_state=:f_state", fDbBind);
    bool found = false;

    for(int i = 0; i < dr.rowCount(); i++) {
        if(c->fItems.contains(dr.value(i, "f_dish").toString())) {
            found = true;
            break;
        }
    }

    if(!found) {
        message_error(tr("This order is not contains any items for this coupon seria"));
        return;
    }

    ui->leCouponSerial->setText(name);
    ui->leCouponSerial->fHiddenText = res.toString();
}

void DlgPayment::on_btnCash_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_order"] = fOrder;
    db2.exec("delete from d_gift_cart_use where f_order=:f_order");
    calcCash();
}

void DlgPayment::on_btnPrintTax_clicked()
{
}

void DlgPayment::on_leDiscount_returnPressed()
{
    if(ui->leCouponNumber->asInt() > 0) {
        message_error(tr("Cannot discount with coupon"));
        return;
    }

    QString code = ui->leDiscount->text().replace("?", "").replace(";", "");
    ui->leDiscount->clear();
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    //COUPON OF SERIVCE?
    db2[":f_code"] = code;
    db2.exec("select t.*, p.f_name as f_partnername "
             "from talon_service t "
             "left join r_partners p on p.f_id=t.f_partner "
             "where f_code=:f_code");

    if(db2.next()) {
        if(db2.integer("f_trsale") == 0) {
            ui->leDiscount->clear();
            message_error(tr("This coupon not sold"));
            return;
        }

        if(db2.integer("f_trback") > 0) {
            ui->leDiscount->clear();
            message_error(tr("This coupon used"));
            return;
        }

        int partnerid = db2.integer("f_partner");
        double price = db2.doubleValue("f_price");
        ui->btnCouponService->setChecked(true);
        ui->leDeptHolder->fHiddenText = QString::number(db2.integer("f_partner"));
        ui->leDeptHolder->setText(db2.string("f_partnername"));
        ui->btnCash->click();
        ui->btnPrintTax->setChecked(false);
        db2[":f_costumer"] = db2.integer("f_partner");
        db2[":f_order"] = fOrder;
        db2.exec("update o_car set f_costumer=:f_costumer where f_order=:f_order");
        db2[":f_couponservice"] = 1;
        db2.update("o_header", "f_id", fOrder);
        db2[":f_date"] = QDate::currentDate();
        db2[":f_partner"] = partnerid;
        db2[":f_amount"] = price;
        int docid = db2.insert("talon_documents_header");
        db2[":f_doc"] = docid;
        db2[":f_group"] = "";
        db2[":f_first"] = code;
        db2[":f_last"] = code;
        db2[":f_qty"] = 1;
        db2[":f_price"] = price;
        db2[":f_total"] = price;
        db2.insert("talon_body");
        db2[":f_trback"] = docid;
        db2[":f_code"] = code;
        db2.exec("update talon_service set f_trback=:f_trback where f_code=:f_code");
        return;
    }

    db2[":f_code"] = code;
    db2.exec("select di.f_info , di.f_fiscal, sum(du.f_amount)as f_sum "
             "from d_gift_cart_use du "
             "inner join d_gift_cart di on di.f_code=du.f_code "
             "where du.f_code=:f_code HAVING SUM(du.f_amount) IS NOT NULL ");
    double amountneeded = ui->leFinalAmount->asDouble() - ui->leCouponAmount->asDouble();

    if(db2.next()) {
        if(db2.doubleValue("f_sum") > 0.001) {
            ui->leGiftFiscal->setText(ui->leGiftFiscal->text()  + db2.string("f_fiscal"));
            double amount = db2.doubleValue("f_sum");
            QString name = db2.string("f_info");

            if(amountneeded < 0.01) {
                ui->leDiscount->clear();
                message_info(tr("No more gift card required"));
                return;
            }

            if(amount > amountneeded) {
                ui->leCouponAmount->setDouble(ui->leFinalAmount->asDouble());
                db2[":f_code"] = code;
                db2[":f_amount"] = amountneeded * -1;
                db2[":f_order"] = fOrder;
                db2.insert("d_gift_cart_use");
            } else {
                ui->leCouponAmount->setDouble(amount);
                db2[":f_code"] = code;
                db2[":f_amount"] = amount * -1;
                db2[":f_order"] = fOrder;
                db2.insert("d_gift_cart_use");
            }

            ui->leCouponNumber->setText(name);

            if(ui->leCouponNumber->property("card").toString().isEmpty()) {
                ui->leCouponNumber->setProperty("card", code);
            } else {
                ui->leCouponNumber->setProperty("card", QString("%1,%2").arg(ui->leCouponNumber->property("card").toString(), code));
            }

            getGiftAmount();

            if(ui->leCard->asDouble() + ui->leCash->asDouble() + ui->leCouponAmount->asDouble() > ui->leFinalAmount->asDouble()) {
                ui->leCard->setDouble(0);
                ui->leCash->setDouble(ui->leFinalAmount->asDouble() - ui->leCouponAmount->asDouble());
            }

            ui->btnPrintTax->setChecked(true);
            return;
        } else {
            ui->leDiscount->clear();
            message_info(tr("Cart amount spent"));
            return;
        }
    }

    ui->leDiscount->setText(code);
    DatabaseResult dr;
    fDbBind[":f_card"] = code;
    dr.select(fDb, "select f_id, f_name, f_mode, f_model from d_car_client where f_card=:f_card", fDbBind);

    if(dr.rowCount() == 0) {
        ui->leDiscount->clear();
        message_error(tr("Invalid card"));
        return;
    }

    int m = dr.value("f_model").toInt();
    ui->leCardHolder->setText(dr.value("f_name").toString());
    ui->leCardHolder->fHiddenText = dr.value("f_id").toString();
    QStringList mode = dr.value("f_mode").toString().split(";", QString::SkipEmptyParts);

    if(mode.count() == 0) {
        ui->leDiscount->clear();
        message_error(tr("Card params is not defined"));
        return;
    }

    DatabaseResult dri;
    fDbBind[":f_header"] = fOrder;
    fDbBind[":f_state"] = DISH_STATE_READY;
    dri.select(fDb,
               "select f_id, f_dish, f_qty, f_price, f_total from o_dish where f_header=:f_header and f_state=:f_state", fDbBind);
    QString visit = mode.at(1);
    QString value = mode.at(2);
    QStringList items = mode.at(3).split(",", QString::SkipEmptyParts);
    bool disc = false;
    double totalDisc = 0;

    switch(m) {
    case 1:
    case 2: {
        DatabaseResult drv;
        fDbBind[":f_costumer"] = dr.value("f_id");
        fDbBind[":f_state"] = ORDER_STATE_CLOSED;
        drv.select(fDb, "select count(h.f_id) as visits from o_header_payment h "
                        "left join o_header o on o.f_id=h.f_id where f_costumer=:f_costumer and o.f_state=:f_state", fDbBind);
        int v = drv.value("visits").toInt();
        int cur = 0;

        if(visit.toInt() > 0) {
            cur = (v + 1) % (visit.toInt());
        }

        ui->lbVisit->setText(QString::number(cur));

        if((dr.value("visits").toInt() > 0) && (cur == 0)) {
            disc = true;
        }

        if(disc) {
            for(int i = 0; i < dri.rowCount(); i++) {
                if(items.contains(dri.value(i, "f_dish").toString(), Qt::CaseInsensitive) || items.at(0) == "*") {
                    double newPrice = dri.value(i, "f_price").toDouble() - (dri.value(i, "f_price").toDouble() * (value.toDouble() / 100));
                    totalDisc += dri.value(i, "f_total").toDouble() - (dri.value(i, "f_qty").toDouble() * newPrice);
                    fDbBind[":f_price"] = newPrice;
                    fDbBind[":f_total"] = newPrice * dri.value(i, "f_qty").toDouble();
                    //fDbBind[":f_totalUSD"] = newPrice * def_usd;
                    fDb.update("o_dish", fDbBind, where_id(ap(dri.value(i, "f_id").toString())));
                }
            }
        }

        break;
    }

    case 3: {
        if(value.toDouble() < 0.001) {
            disc = false;
            return;
        }

        disc = true;
        double balance = value.toDouble();

        if(disc) {
            for(int i = 0; i < dri.rowCount(); i++) {
                if(balance < 0.01) {
                    continue;
                }

                if(items.contains(dri.value(i, "f_dish").toString(), Qt::CaseInsensitive) || items.at(0) == "*") {
                    double itemTotal = dri.value(i, "f_qty").toDouble() * dri.value(i, "f_price").toDouble();

                    if(balance >= itemTotal) {
                        balance -= itemTotal;
                        totalDisc += itemTotal;
                        itemTotal = 0;
                    } else {
                        itemTotal -= balance;
                        totalDisc += balance;
                        balance = 0;
                    }

                    fDbBind[":f_price"] = itemTotal / dri.value(i, "f_qty").toDouble();
                    fDbBind[":f_total"] = itemTotal;
                    //fDbBind[":f_totalUSD"] = newPrice * def_usd;
                    fDb.update("o_dish", fDbBind, where_id(ap(dri.value(i, "f_id").toString())));
                }
            }

            fDbBind[":f_mode"] = QString("%1;%2;%3;%4;").arg("1", "0", QString::number(balance, 'f', 0), items.at(0));
            fDb.update("d_car_client", fDbBind, where_id(dr.value(0).toInt()));
        }
    }
    break;
    }

    if(!disc) {
        return;
    } else {
        DatabaseResult drDisc;
        fDbBind[":f_id"] = fOrder;
        drDisc.select(fDb, "select * from o_temp_disc where f_id=:f_id", fDbBind);

        if(drDisc.rowCount() > 0) {
            ui->leDiscount->clear();
            message_error(tr("Discount already used"));
            return;
        }
    }

    if(ui->leCouponNumber->property("card").toString().isEmpty()) {
        ui->leCouponNumber->setProperty("card", code);
    } else {
        ui->leCouponNumber->setProperty("card", QString("%1,%2").arg(ui->leCouponNumber->property("card").toString(), code));
    }

    fDbBind[":f_total"] = totalDisc;
    fDbBind[":f_id"] = fOrder;
    fDb.select("update o_header set f_total=f_total-:f_total where f_id=:f_id", fDbBind, fDbRows);
    ui->leFinalAmount->setDouble(ui->leFinalAmount->asDouble() - totalDisc);
    ui->leCash->setText(ui->leFinalAmount->text());
    ui->leCouponAmount->setText("0");
    ui->leCard->setText("0");
    ui->leCouponNumber->clear();
    ui->leCouponSerial->clear();
    ui->leCouponSerial->fHiddenText.clear();
    ui->leDiscountAmount->setDouble(totalDisc);
    ui->btnCancel->setEnabled(false);
    fCanReject = false;
}

void DlgPayment::on_leCard_textChanged(const QString &arg1)
{
    if(arg1.toDouble() > 0.01) {
        ui->btnPrintTax->setChecked(true);
    } else {
    }
}

void DlgPayment::on_btnTaxPayerId_clicked()
{
    QString num;

    if(RNumbers::getString(num, "ԳՆՈՐԴԻ ՀՎՀՀ", this)) {
        ui->leTaxpayerId->setText(QString("%1").arg(num));
    }
}

void DlgPayment::on_btnOldTalon_clicked(bool checked)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_order"] = fOrder;

    if(checked) {
        int debtId;
        QString debtName;

        if(!DlgDeptHolder::getHolder(debtId, debtName)) {
            return;
        }

        db2[":f_costumer"] = debtId;
        ui->leDeptHolder->fHiddenText = QString::number(debtId);
        ui->leDeptHolder->setText(debtName);
        ui->btnPrintTax->setChecked(false);
        ui->btnCash->click();
    } else {
        db2[":f_costumer"] = 0;
        ui->leDeptHolder->fHiddenText.clear();
        ui->leDeptHolder->clear();
    }

    db2.exec("update o_car set f_costumer=:f_costumer where f_order=:f_order");
    db2[":f_couponservice"] = checked ? 1 : 0;
    db2.update("o_header", "f_id", fOrder);
    ui->btnPrintTax->setChecked(!checked);
}

void DlgPayment::on_btnCouponService_clicked(bool checked)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    if (!db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        message_error(db2.lastDbError());
        return;
    }

    if (checked) {
        bool ok = false;
        QString code = QInputDialog::getText(this, tr("Code"), tr("Code"), QLineEdit::Password, "", &ok);
        code = code.replace("tel:", "", Qt::CaseInsensitive);
        if (!ok) {
            ui->btnCouponService->setChecked(false);
            return;
        }
        if (code.isEmpty()) {
            return;
        }

        db2[":f_code"] = code;
        db2.exec("select  t.*, p.f_name as f_partnername from talon_service t "
                 "left join r_partners p on p.f_id=t.f_partner "
                 "where t.f_code=:f_code ");
        if (db2.next() == false) {
            ui->btnCouponService->setChecked(false);
            message_error(tr("Invalid code"));
            return;
        }
        if (db2.integer("f_used") != 0)  {
            ui->btnCouponService->setChecked(false);
            message_error(tr("Talon used"));
            return;
        }
        int partner = db2.integer("f_partner");
        QString partnerName = db2.string("f_parnername");
        db2[":f_code"] =code;
        db2[":f_order"] = fOrder;
        db2.exec("update talon_service set f_used=1, f_order=:f_order where f_code=:f_code");
        db2[":f_partner"] = partner;
        db2[":f_order"] = fOrder;
        db2.exec("update o_car set f_partner=:f_partner where f_order=:f_order");

        ui->leDeptHolder->fHiddenText = QString::number(partner);
        ui->leDeptHolder->setText(partnerName);
        ui->btnPrintTax->setChecked(false);
        ui->btnCash->click();
    } else {
        db2[":f_order"] = fOrder;
        db2.exec("update talon_service set f_used=0, f_order=null where f_order=:f_order");

        db2[":f_order"] = fOrder;
        db2.exec("update o_car set f_partner=null where f_order=:f_order");
        ui->leDeptHolder->fHiddenText.clear();
        ui->leDeptHolder->clear();
    }


    // db2[":f_order"] = fOrder;
    // if(checked) {
    //     int debtId;
    //     QString debtName;

    //     if(!DlgDeptHolder::getHolder(debtId, debtName)) {
    //         return;
    //     }

    //     db2[":f_costumer"] = debtId;
    //     ui->leDeptHolder->fHiddenText = QString::number(debtId);
    //     ui->leDeptHolder->setText(debtName);
    //     ui->btnPrintTax->setChecked(false);
    //     ui->btnCash->click();
    // } else {
    //     db2[":f_costumer"] = 0;
    //     ui->leDeptHolder->fHiddenText.clear();
    //     ui->leDeptHolder->clear();
    // }

    // db2.exec("update o_car set f_costumer=:f_costumer where f_order=:f_order");
    db2[":f_couponservice"] = checked ? 1 : 0;
    db2.update("o_header", "f_id", fOrder);
    ui->btnPrintTax->setChecked(!checked);
}

void DlgPayment::on_btnPrepaid_clicked()
{
    if(ui->leDeptHolder->isEmpty()) {
        message_error("Նշեք հաճախորդի անունը");
        return;
    }

    if(ui->leGiftCardCode->text().isEmpty()) {
        message_error("Անցկացրեք քարտը");
        return;
    }

    if(ui->leCard->asDouble()
            + ui->leCash->asDouble()
            + ui->leIdram->asDouble()
            + ui->leCouponAmount->asDouble()
            + ui->leDept->asDouble() <
            ui->leFinalAmount->asDouble()) {
        message_error(tr("Payment not complete"));
        return;
    }

    if(ui->leCard->asDouble()
            + ui->leCash->asDouble()
            + ui->leIdram->asDouble()
            + ui->leCouponAmount->asDouble()
            + ui->leDept->asDouble() >
            ui->leFinalAmount->asDouble()) {
        message_error(tr("Amount greater than need to pay"));
        return;
    }

    QMap<QString, QVariant> s = fFiscalMachines[ui->cbFiscalMachine->currentText()];
    PrintTaxN pn(s.value("ip").toString(),
                 s.value("port").toInt(),
                 s.value("password").toString(),
                 s.value("extpos").toString(),
                 s.value("opcode").toString(),
                 s.value("oppin").toString());
    QString in, out, err;
    int result = pn.printAdvanceJson(ui->leCash->asDouble(), ui->leCard->asDouble()  + ui->leIdram->asDouble(), in, out, err);
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    if(ui->leTaxpayerId->asInt() > 0) {
        pn.fPartnerTin = ui->leTaxpayerId->text();
    }

    int fiscalrecid;
    db2[":f_order"] = fOrder;
    db2[":f_in"] = in;
    db2[":f_out"] = out;
    db2[":f_err"] = err;
    db2.insert("o_tax_log", fiscalrecid);

    if(result != pt_err_ok) {
        message_error(tr("Fiscal error.") + "\r\n" + err);
        return;
    }

    QJsonObject jo = QJsonDocument::fromJson(out.toUtf8()).object();
    db2[":f_fiscal"] = jo["rseq"].toInt();
    db2.update("o_tax_log", "f_id", fiscalrecid);
    db2[":f_tax"] = jo["rseq"].toInt();
    db2.update("o_header", "f_id", fOrder);
    db2[":f_code"] = ui->leGiftCardCode->text();
    db2[":f_fiscal"] = jo["rseq"].toInt();
    db2[":f_holder"] = ui->leDeptHolder->text();
    db2.exec("update d_gift_cart set f_fiscal=:f_fiscal, f_info=concat_ws(' ', f_info, :f_holder) where f_code=:f_code");
    fDbBind[":f_id"] = fOrder;
    DatabaseResult drDisc;
    drDisc.select(fDb, "select * from o_temp_disc where f_id=:f_id", fDbBind);

    if(ui->leDept->asDouble() > 0.1) {
        if(ui->leDeptHolder->fHiddenText.toInt() == 0) {
            message_error(tr("Dept holder is not defined"));
            return;
        }
    }

    fDbBind[":f_cash"] = ui->btnCouponService->isChecked() ? 0 : ui->leCash->asDouble();
    fDbBind[":f_card"] = ui->leCard->asDouble();
    fDbBind[":f_couponservice"] = ui->btnCouponService->isChecked() ? ui->leCash->asDouble() : 0;
    fDbBind[":f_coupon"] = ui->leCouponAmount->asDouble();
    fDbBind[":f_discount"] = ui->leDiscountAmount->asDouble();
    fDbBind[":f_couponSeria"] = ui->leCouponSerial->text();
    fDbBind[":f_couponNumber"] = ui->leCouponNumber->text();
    fDbBind[":f_discountCard"] = ui->leCouponNumber->property("card");
    fDbBind[":f_costumer"] = ui->leCardHolder->fHiddenText.toInt();
    fDbBind[":f_finalAmount"] = ui->leFinalAmount->asDouble();
    fDbBind[":f_debt"] = ui->leDept->asDouble();
    fDbBind[":f_debtHolder"] = ui->leDeptHolder->fHiddenText.toInt();

    if(drDisc.rowCount() > 0) {
        fDbBind[":f_discount"] = drDisc.value("f_amount");
        fDbBind[":f_costumer"] = drDisc.value("f_costumer");
    }

    if(fUpdateHeader) {
        fDb.update("o_header_payment", fDbBind, where_id(fOrder));
    } else {
        fDbBind[":f_id"] = fOrder;

        if(!fDb.insertWithoutId("o_header_payment", fDbBind)) {
            message_error(fDb.fLastError);
        }
    }

    if(ui->leDept->asDouble() > 0) {
        fDbBind[":f_id"] = fOrder;
        fDbBind[":f_holder"] = ui->leDeptHolder->fHiddenText.toInt();
        fDbBind[":f_debt"] = ui->leDept->asDouble();
        fDb.insertWithoutId("o_debts", fDbBind);
    }

    if(ui->leCouponNumber->asInt() > 0) {
        fDbBind[":f_seria"] = ui->leCouponSerial->fHiddenText.toInt();
        fDbBind[":f_number"] = ui->leCouponNumber->asInt();
        fDb.select("update d_coupon set f_used=1 where f_seria=:f_seria and cast(f_number as signed)=:f_number", fDbBind,
                   fDbRows);
    }

    accept();
}

void DlgPayment::on_leGiftCardCode_returnPressed()
{
    ui->leGiftCardCode->setText(ui->leGiftCardCode->text().replace(";", "").replace("?", ""));
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_code"] = ui->leGiftCardCode->text();
    db2.exec("select f_fiscal from d_gift_cart where f_code=:f_code");

    if(!db2.next()) {
        ui->leGiftCardCode->clear();
        message_error("Անհայտ քարտ");
        return;
    }

    if(db2.integer("f_fiscal") > 0) {
        ui->leGiftCardCode->clear();
        message_error("Այս քարտը հնարավոր չէ վաճառել");
        return;
    }

    db2[":f_code"] = ui->leGiftCardCode->text();
    db2.exec("select * from d_gift_cart_use where f_code=:f_code and f_amount<0");

    if(db2.next()) {
        ui->leGiftCardCode->clear();
        message_error("Այս քարտը հնարավոր չէ վաճառել");
        return;
    }
}

void DlgPayment::on_btnCard_2_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_order"] = fOrder;
    db2.exec("delete from d_gift_cart_use where f_order=:f_order");
    calcIdram();
}
