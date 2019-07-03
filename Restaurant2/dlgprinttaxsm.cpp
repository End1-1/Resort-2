#include "dlgprinttaxsm.h"
#include "ui_dlgprinttaxsm.h"
#include "message.h"
#include <QJsonDocument>
#include <QJsonObject>

DlgPrintTaxSM::DlgPrintTaxSM(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPrintTaxSM)
{
    ui->setupUi(this);
    fTaxCode = 0;
}

DlgPrintTaxSM::~DlgPrintTaxSM()
{
    delete ui;
}

void DlgPrintTaxSM::addGoods(const QString &dep, const QString &adgt, const QString &code, const QString &name, double price, double qty)
{
    fPrintTax.addGoods(dep, adgt, code, name, price, qty, 0);
}

//int DlgPrintTaxSM::printTax(const QString &dep, const QString &order, double cardAmount)
//{
//    DlgPrintTaxSM *d = new DlgPrintTaxSM(fPreferences.getDefaultParentForMessage());
//    d->ui->teResult->setPlainText(QString("%1: %2").arg(tr("Printing")).arg(order));
//    d->fOrder = order;
//    d->fDep = dep;
//    d->fCardAmount = cardAmount;
//    int result = d->exec();
//    delete d;
//    return result;
//}

int DlgPrintTaxSM::exec()
{
    fPrintMode = tm_receipt;
    connect(&fTimer, &QTimer::timeout, [this](){
       load();
    });
    fTimer.start(2000);
    return BaseExtendedDialog::exec();
}

int DlgPrintTaxSM::execAdvance()
{
    fPrintMode = tm_advance;
    connect(&fTimer, &QTimer::timeout, [this](){
       loadAdvance();
    });
    fTimer.start(2000);
    return BaseExtendedDialog::exec();
}

int DlgPrintTaxSM::execTaxback()
{
    fPrintMode = tm_taxback;
    connect(&fTimer, &QTimer::timeout, [this]() {
        loadTaxback();
    });
    fTimer.start(2000);
    return BaseExtendedDialog::exec();
}

bool DlgPrintTaxSM::printAdvance(double amountCash, double amountCard, const QString &vaucher, int &taxCode)
{
    DlgPrintTaxSM *d = new DlgPrintTaxSM(fPreferences.getDefaultParentForMessage());
    d->ui->teResult->setPlainText(tr("Printing advance") + " " + vaucher);
    d->fOrder = vaucher;
    d->fCashAmount = amountCash;
    d->fCardAmount = amountCard;
    bool result = d->execAdvance() == TAX_OK;
    taxCode = d->fTaxCode;
    delete d;
    return result;
}

int DlgPrintTaxSM::printTaxback(int number, const QString &vaucher, int &taxCode)
{
    DlgPrintTaxSM *d = new DlgPrintTaxSM(fPreferences.getDefaultParentForMessage());
    d->ui->teResult->setPlainText(tr("Tax back") + " " + vaucher);
    d->fOrder = vaucher;
    d->fTaxback = number;
    bool result = d->execTaxback() == TAX_OK;
    taxCode = d->fTaxCode;
    delete d;
    return result;
}

void DlgPrintTaxSM::load()
{
    fTimer.stop();
    fPrintTax.setParams(fPreferences.getDb(def_tax_address).toString(),
                fPreferences.getDb(def_tax_port).toInt(),
                fPreferences.getDb(def_tax_password).toString());
    QString inJson, outJson, err;
    int result = fPrintTax.makeJsonAndPrint(fCardAmount, fPrepaid, inJson, outJson, err);

    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_db"] = 1;
    fDbBind[":f_order"] = fOrder;
    fDbBind[":f_queryCode"] = opcode_PrintTaxN;
    fDbBind[":f_queryJson"] = inJson;
    fDbBind[":f_replyJson"] = outJson;
    fDbBind[":f_replyResult"] = result;
    fDbBind[":f_replyJson"] = outJson;
    fDbBind[":f_replyText"] = err;
    fDbBind[":f_replyTaxCode"] = "-";
    fAirDb.open();
    int id = fAirDb.insert("tax_print", fDbBind);
    fAirDb.close();

    if (result == pt_err_ok) {
        QJsonDocument jd = QJsonDocument::fromJson(outJson.toUtf8());
        QJsonObject jo = jd.object();
        fTaxCode = jo["rseq"].toInt();
        fDbBind[":f_replyTaxCode"] = QString::number(fTaxCode);
        fDbBind[":f_id"] = id;
        fAirDb.open();
        fAirDb.select("update tax_print set f_replyTaxCode=:f_replyTaxCode where f_id=:f_id", fDbBind, fDbRows);
        fAirDb.close();
        done(TAX_OK);
        return;
    }
    ui->teResult->setPlainText(outJson + "\r\n\r\n" + err);
    ui->btnRetry->setEnabled(true);
    //ui->btnNoNeed->setEnabled(true);
}

void DlgPrintTaxSM::loadAdvance()
{
    fTimer.stop();
    fPrintTax.setParams(fPreferences.getDb(def_tax_address).toString(),
                fPreferences.getDb(def_tax_port).toInt(),
                fPreferences.getDb(def_tax_password).toString());
    QString inJson;
    QString outJson;
    QString err;
    int result = fPrintTax.printAdvanceJson(fCashAmount, fCardAmount, inJson, outJson, err);

    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_db"] = 1;
    fDbBind[":f_order"] = fOrder;
    fDbBind[":f_queryCode"] = opcode_PrintTaxN;
    fDbBind[":f_queryJson"] = inJson;
    fDbBind[":f_replyJson"] = outJson;
    fDbBind[":f_replyResult"] = result;
    fDbBind[":f_replyJson"] = outJson;
    fDbBind[":f_replyText"] = err;
    fDbBind[":f_replyTaxCode"] = "-";
    fAirDb.open();
    int id = fAirDb.insert("tax_print", fDbBind);
    fAirDb.close();

    if (result == pt_err_ok) {
        QJsonDocument jd = QJsonDocument::fromJson(outJson.toUtf8());
        QJsonObject jo = jd.object();
        fTaxCode = jo["rseq"].toInt();
        fDbBind[":f_replyTaxCode"] = QString::number(fTaxCode);
        fDbBind[":f_id"] = id;
        fAirDb.open();
        fAirDb.select("update tax_print set f_replyTaxCode=:f_replyTaxCode where f_id=:f_id", fDbBind, fDbRows);
        fAirDb.close();
        fDbBind[":f_fiscal"] = fTaxCode;
        fDb.update("m_register", fDbBind, where_id(ap(fOrder)));
        done(TAX_OK);
        return;
    }
    ui->teResult->setPlainText(outJson + "\r\n\r\n" + err);
    ui->btnRetry->setEnabled(true);
}

void DlgPrintTaxSM::loadTaxback()
{
    fTimer.stop();
    fPrintTax.setParams(fPreferences.getDb(def_tax_address).toString(),
                fPreferences.getDb(def_tax_port).toInt(),
                fPreferences.getDb(def_tax_password).toString());
    QString inJson;
    QString outJson;
    QString err;
    fDbBind[":f_replyTaxCode"] = fTaxback;
    DatabaseResult dr;
    dr.select(fDb, "select * from airwick.tax_print where f_replyTaxCode=:f_replyTaxCode", fDbBind);
    QString crn;
    int result = pt_err_ok;
    if (dr.rowCount() == 0) {
        result = pt_err_no_tax_in_db;
        err = tr("The given tax number not exists in databases");
        done(result);
        return;
    } else {
        QJsonDocument jDoc = QJsonDocument::fromJson(dr.value("f_replyJson").toString().toUtf8());
        QJsonObject jObj = jDoc.object();
        crn = jObj["crn"].toString();
        if (crn.isEmpty()) {
            message_error(tr("Crn is empty") + "<br>" + dr.value("f_replyJson").toString());
            result = pt_err_no_tax_in_db;
            err = tr("The given tax number not exists in databases");
            done(result);
            return;
        }
    }

    result = fPrintTax.printTaxback(fTaxback, crn, inJson, outJson, err);
    fAirDb.open();
    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_db"] = 1;
    fDbBind[":f_order"] = fOrder;
    fDbBind[":f_queryCode"] = opcode_taxback;
    fDbBind[":f_queryJson"] = inJson;
    fDbBind[":f_replyJson"] = outJson;
    fDbBind[":f_replyResult"] = result;
    fDbBind[":f_replyJson"] = outJson;
    fDbBind[":f_replyText"] = err;
    fDbBind[":f_replyTaxCode"] = "-";
    int id = fAirDb.insert("tax_print", fDbBind);
    fAirDb.close();

    if (result == pt_err_ok) {
        QJsonDocument jd = QJsonDocument::fromJson(outJson.toUtf8());
        QJsonObject jo = jd.object();
        fTaxCode = jo["rseq"].toInt();
        fDbBind[":f_replyTaxCode"] = QString::number(fTaxCode);
        fDbBind[":f_id"] = id;
        fAirDb.open();
        fAirDb.select("update tax_print set f_replyTaxCode=:f_replyTaxCode where f_id=:f_id", fDbBind, fDbRows);
        fAirDb.close();
        done(TAX_OK);
        return;
    }
    ui->teResult->setPlainText(outJson + "\r\n\r\n" + err);
    ui->btnRetry->setEnabled(true);
}

void DlgPrintTaxSM::on_btnCancel_clicked()
{
    done(TAX_FAIL);
}

void DlgPrintTaxSM::on_btnRetry_clicked()
{
    ui->btnRetry->setEnabled(false);
    //ui->btnNoNeed->setEnabled(false);
    ui->teResult->setPlainText(QString("%1: %2").arg(tr("Retry printing")).arg(fOrder));
    qApp->processEvents();
    switch (fPrintMode) {
    case tm_receipt:
        load();
        break;
    case tm_advance:
        loadAdvance();
        break;
    case tm_taxback:
        loadTaxback();
        break;
    }
}

void DlgPrintTaxSM::on_btnNoNeed_clicked()
{
    done(TAX_NONEED);
}
