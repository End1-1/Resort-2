#include "dlgprinttax.h"
#include "ui_dlgprinttax.h"
#include <QJsonDocument>
#include <QJsonObject>

DlgPrintTax::DlgPrintTax(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPrintTax)
{
    ui->setupUi(this);
    connect(&fTimer, &QTimer::timeout, [this]() {
        load(fDep, fOrder);
    });
    fTaxCode = 0;
}

DlgPrintTax::~DlgPrintTax()
{
    delete ui;
}

int DlgPrintTax::printTax(const QString &dep, const QString &order, double cardAmount, int &taxCode)
{
    DlgPrintTax *d = new DlgPrintTax(fPreferences.getDefaultParentForMessage());
    d->ui->teResult->setPlainText(QString("%1: %2").arg(tr("Printing")).arg(order));
    d->fOrder = order;
    d->fDep = dep;
    d->fCardAmount = cardAmount;
    int result = d->exec();
    taxCode = d->fTaxCode;
    delete d;
    return result;
}

int DlgPrintTax::exec()
{
    fTimer.start(2000);
    return BaseExtendedDialog::exec();
}

void DlgPrintTax::load(const QString &dep, const QString &order)
{
    fTimer.stop();
    PrintTaxN pt(fPreferences.getDb(def_tax_address).toString(),
                fPreferences.getDb(def_tax_port).toInt(),
                fPreferences.getDb(def_tax_password).toString());
    fDbBind[":f_header"] = order;
    fDbBind[":f_state"] = DISH_STATE_READY;
    DatabaseResult dr;
    dr.select(fDb, "select d.f_adgt, od.f_dish, d.f_am, od.f_price, od.f_qty "
                   "from o_dish od "
                   "left join r_dish d on d.f_id=od.f_dish "
                   "where od.f_header=:f_header and od.f_state=:f_state "
                   "and (f_complex=0 or (f_complex>0 and f_complexId=0))", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        pt.addGoods(dep, dr.value(i, "f_adgt").toString(), dr.value(i, "f_code").toString(),
                      dr.value(i, "f_am").toString(), dr.value(i, "f_price").toDouble(), dr.value(i, "f_qty").toDouble());
    }
    QString inJson, outJson, err;
    int result = pt.makeJsonAndPrint(fCardAmount, 0, inJson, outJson, err);

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
#ifdef QT_DEBUG
        outJson = "{\"rseq\":38432,\"crn\":\"53208770\",\"sn\":\"V90413501919\",\"tin\":\"02596277\",\"taxpayer\":\"«ՊԼԱԶԱ ՍԻՍՏԵՄՍ»\",\"address\":\"ԵՐԵՎԱՆ Ամիրյան 4/3\",\"time\":1522831916000.0,\"fiscal\":\"18371620\",\"lottery\":\"00000000\",\"prize\":0,\"total\":20.0,\"change\":0.0}";
#endif
        int pos = outJson.indexOf("\"rseq\":") + 7;
        int pos2 = outJson.indexOf(",", pos );
        QString rseq = outJson.mid(pos, pos2 - pos);
        pos = outJson.indexOf("\"fiscal\":") + 9;
        pos2 = outJson.indexOf(",", pos + 1);
        QString fiscal = outJson.mid(pos, pos2 - pos);
        fiscal.replace("\"\"", "");
        pos = outJson.indexOf("\"tin\":") + 6;
        pos2 = outJson.indexOf(",", pos + 1);
        QString hvhh = outJson.mid(pos, pos2 - pos);
        hvhh.replace("\"", "");
        fTaxCode = rseq.toInt();

        fDbBind[":f_replyTaxCode"] = QString::number(fTaxCode);
        fDbBind[":f_id"] = id;
        fAirDb.open();
        fAirDb.select("update tax_print set f_replyTaxCode=:f_replyTaxCode where f_id=:f_id", fDbBind, fDbRows);
        fAirDb.close();
        fDbBind[":f_tax"] = fTaxCode;
        fDb.update("o_header", fDbBind,  ap(fOrder));
        done(TAX_OK);
        return;
    }
    ui->teResult->setPlainText(outJson + "\r\n\r\n" + err);
    ui->btnRetry->setEnabled(true);
    ui->btnNoNeed->setEnabled(true);
}

void DlgPrintTax::on_btnCancel_clicked()
{
    done(TAX_FAIL);
}

void DlgPrintTax::on_btnRetry_clicked()
{
    ui->btnRetry->setEnabled(false);
    ui->btnNoNeed->setEnabled(false);
    ui->teResult->setPlainText(QString("%1: %2").arg(tr("Retry printing")).arg(fOrder));
    qApp->processEvents();
    load(fDep, fOrder);
}

void DlgPrintTax::on_btnNoNeed_clicked()
{
    done(TAX_NONEED);
}
