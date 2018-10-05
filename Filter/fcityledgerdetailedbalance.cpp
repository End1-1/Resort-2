#include "fcityledgerdetailedbalance.h"
#include "ui_fcityledgerdetailedbalance.h"
#include "wreportgrid.h"
#include "vauchers.h"
#include "ptextrect.h"
#include "dlgchangeclofvaucher.h"
#include "pimage.h"

FCityLedgerDetailedBalance::FCityLedgerDetailedBalance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCityLedgerDetailedBalance)
{
    ui->setupUi(this);
    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCLCode);
    connect(fDockCL, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));
    QString query = "call cl_detailed_balance(:d1, :d2, :cl);select * from rep;";

    fReportGrid->fStaticQuery = query;
    fReportGrid->addFilterWidget(this);
    fReportGrid->setupTabTextAndIcon(tr("C/L detailded balance"), ":/images/balance.png");
    fReportGrid->addToolBarButton(":/images/invoice.png", tr("Open invoice"), SLOT(openInvoice()), this)
            ->setFocusPolicy(Qt::ClickFocus);
    ui->btnChangeCityLedger->setVisible(r__(cr__super_correction));
}

FCityLedgerDetailedBalance::~FCityLedgerDetailedBalance()
{
    delete ui;
}

void FCityLedgerDetailedBalance::apply(WReportGrid *rg)
{
    QString query = rg->fStaticQuery;
    query = query.replace(":d1", ui->deFrom->dateMySql())
            .replace(":d2", ui->deTo->dateMySql())
            .replace(":cl", QString::number(ui->leCLCode->text().toInt()));
    rg->fModel->clearColumns();
    rg->fModel->
             setColumn(0, "", tr(""))
            .setColumn(100, "", tr("DATE"))
            .setColumn(350, "", tr("NAME"))
            .setColumn(80, "", tr("DEBIT AMD"))
            .setColumn(80, "", tr("CREDIT AMD"))
            .setColumn(110, "", tr("BALANCE AMD"))
            .setColumn(100, "", tr("ROOM"))
            .setColumn(100, "", tr("VOUCHER"))
            ;
   // rg->fModel->setSqlQuery();
    rg->fModel->apply(query.split(";", QString::SkipEmptyParts));
    QList<int> sumCols;
    QList<double> sumVals;
    sumCols << 3 << 4;
    rg->fModel->sumOfColumns(sumCols, sumVals);
    QList<QVariant> emptyRow;
    emptyRow << "" << "" << "" << "" << "" << "" << "" << "" ;
    double openBalance = 0;
    if (rg->fModel->data(0, 3, Qt::EditRole).toDouble() != 0) {
        openBalance = rg->fModel->data(0, 3, Qt::EditRole).toDouble();
    } else {
        openBalance = rg->fModel->data(0, 4, Qt::EditRole).toDouble();
    }

    double debt = 0, cred = 0;
    for (int i = 0; i < rg->fModel->rowCount(); i++) {
        if (rg->fModel->data(i, 3, Qt::EditRole).toDouble() < 0) {
            rg->fModel->setData(i, 4, rg->fModel->data(i, 3, Qt::EditRole).toDouble() * -1);
            rg->fModel->setData(i, 3, "0");
            if (i > 0) {
                cred += rg->fModel->data(i, 4, Qt::EditRole).toDouble();
            }
        } else {
            if (i > 0) {
                debt += rg->fModel->data(i, 3, Qt::EditRole).toDouble();
            }
        }
        rg->fModel->setData(i, 5, rg->fModel->data(i, 3, Qt::EditRole).toDouble() - rg->fModel->data(i, 4, Qt::EditRole).toDouble());
        if (i > 0) {
            rg->fModel->setData(i, 5, rg->fModel->data(i, 5, Qt::EditRole).toDouble() + rg->fModel->data(i - 1, 5, Qt::EditRole).toDouble());
        }
    }
    rg->fModel->insertRow(rg->fModel->rowCount(), emptyRow);
    rg->fTableView->setSpan(rg->fModel->rowCount() - 1, 0, 1, rg->fModel->columnCount());
    QList<QVariant> rowOpen = emptyRow;
    rowOpen[5] = tr("BROUGHT FORWARD");
    rowOpen[7] =  openBalance;
    QList<QVariant> rowDebt = emptyRow;
    rowDebt[5] = tr("DEBIT");
    rowDebt[7] = debt;
    QList<QVariant> rowCred = emptyRow;
    rowCred[5] = tr("CREDIT");
    rowCred[7] = cred;
    QList<QVariant> rowBalPeriod = emptyRow;
    rowBalPeriod[5] = tr("BALANCE FOR PERIOD");
    rowBalPeriod[7] = debt - cred;
    QList<QVariant> rowTotal = emptyRow;
    rowTotal[5] = tr("BALANCE");
    rowTotal[7] = openBalance + (debt - cred);
    rg->fModel->insertRow(rg->fModel->rowCount(), rowOpen);
    rg->fModel->insertRow(rg->fModel->rowCount(), rowDebt);
    rg->fModel->insertRow(rg->fModel->rowCount(), rowCred);
    rg->fModel->insertRow(rg->fModel->rowCount(), rowBalPeriod);
    rg->fModel->insertRow(rg->fModel->rowCount(), rowTotal);
    for (int i = rg->fModel->rowCount() - 1, count = rg->fModel->rowCount() - 6; i > count; i--) {
        rg->fTableView->setSpan(i, 1, 1, 3);
        rg->fTableView->setSpan(i, 5, 1, 2);
    }
    rg->fModel->setData(rg->fModel->rowCount() - 5, 1, Utils::numberToWords((int)rowTotal[7].toDouble()));
}

QString FCityLedgerDetailedBalance::reportTitle()
{
    QString title = QString("STATEMENT OF ACCOUNT\r\n%1/%2\r\n%3: %4\r\n%5: %6")
            .arg(ui->leCL->fHiddenText)
            .arg(ui->leCL->text())
            .arg(tr("FROM DATE"))
            .arg(ui->deFrom->text())
            .arg(tr("TO DATE"))
            .arg(ui->deTo->text());
    return title;
}

QWidget *FCityLedgerDetailedBalance::firstElement()
{
    return ui->deFrom;
}

bool FCityLedgerDetailedBalance::officialTitle()
{
    return true;
}

bool FCityLedgerDetailedBalance::handlePrint()
{
    PrintOrientation po = Portrait;
    //QSize paperSize = sizePortrait;
    int top = 10;
    int footerTop = sizePortrait.height() - 40;
    int page = 1;
    QBrush b(Qt::white, Qt::SolidPattern);
    PTextRect trFooter;
    trFooter.setBrush(b);
    QFont f(qApp->font().family(), 20);
    trFooter.setFont(f);
    trFooter.setBorders(false, false, false, false);
    trFooter.setTextAlignment(Qt::AlignLeft);

    PTextRect th;
    th.setWrapMode(QTextOption::WordWrap);
    th.setFont(QFont("Times", 20));
    th.setBorders(true, true, true, true);
    QPen pen;
    pen.setWidth(2);
    th.setRectPen(pen);
    PTextRect prHead(th, "");
    prHead.setFont(QFont("Times", 20));
    prHead.setBrush(QBrush(QColor::fromRgb(215, 215, 215), Qt::SolidPattern));
    prHead.setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    int rowHeight = ((double) fReportGrid->fTableView->verticalHeader()->defaultSectionSize() * resize_factor * 2);
    PPrintPreview *pp = new PPrintPreview(this);
    PPrintScene *ps = pp->addScene(0, po);
    QString title = reportTitle();

    PTextRect *trInfo = new PTextRect(1500, 20, 600, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                      0, QFont("Arial", 25));
    trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
    trInfo->setBorders(false, false, false, false);
    ps->addItem(trInfo);
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(20, 10, 500, 300));
    QFont f1("Arial", 25);
    f1.setBold(true);
    th.setFont(f1);
    th.setTextAlignment(Qt::AlignCenter);
    th.setBorders(false, false, false, false);
    ps->addTextRect(20, 50, 2100, rowHeight, title, &th);
    top = 330;

    QBrush brush(Qt::SolidPattern);
    brush.setColor(QColor::fromRgb(215, 215, 215));
    th.setBrush(brush);
    th.setBorders(true, true, true, true);
    QList<int> cols;
    QStringList vals;
    cols << 20 << 200 << 800 << 210 << 210 << 190 << 150 << 220;
    vals << tr("DATE") << tr("DESCRIPTION") << tr("DEBIT AMD") << tr("CREDIT AMD")
         << tr("BALANCE AMD") << tr("ROOM") << tr("VOUCHER");
    ps->addTableRow(top, rowHeight, cols, vals, &th);

    f1.setBold(false);
    th.setFont(f1);
    brush.setColor(Qt::white);
    th.setBrush(brush);
    th.setTextAlignment(Qt::AlignLeft);
    th.setWrapMode(QTextOption::NoWrap);
    int r = 0;
    for (int i = 0; i < fReportGrid->fModel->rowCount(); i++) {
        r = i;
        if (i > 0 && fReportGrid->fModel->data(i, 0).toString() == "") {
            break;
        }
        for (int j = 1; j < fReportGrid->fModel->columnCount(); j++) {
            vals << fReportGrid->fModel->data(i, j).toString();
        }
        ps->addTableRow(top, rowHeight, cols, vals, &th);
        if (top > sizePortrait.height() - 300) {
            ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                            .arg(tr("Printed"))
                            .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                            .arg(WORKING_USERNAME), &trFooter);
            trFooter.setTextAlignment(Qt::AlignRight);
            ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                            .arg(tr("Page"))
                            .arg(page++), &trFooter);
            trFooter.setTextAlignment(Qt::AlignLeft);
            top = 10;
            ps = pp->addScene(0, Portrait);
        }
    }

    if (top + (rowHeight * 8) > sizePortrait.height() - 200) {
        ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                        .arg(WORKING_USERNAME), &trFooter);
        trFooter.setTextAlignment(Qt::AlignRight);
        ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                        .arg(tr("Page"))
                        .arg(page++), &trFooter);
        trFooter.setTextAlignment(Qt::AlignLeft);
        top = 10;
        ps = pp->addScene(0, Portrait);
    }

    cols.clear();
    r++;
    top += (rowHeight / 2);
    f1.setBold(true);
    th.setFont(f1);
    th.setBorders(false, false, false, false);
    ps->addTextRect(20, top, 1000, rowHeight, tr("BEING THE TOTAL OF:"), &th);
    ps->addTextRect(20, top + (rowHeight / 2), 1000, rowHeight, fReportGrid->fModel->data(r, 1).toString(), &th);

    if (top > sizePortrait.height() - 200) {
        ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                        .arg(WORKING_USERNAME), &trFooter);
        trFooter.setTextAlignment(Qt::AlignRight);
        ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                        .arg(tr("Page"))
                        .arg(page++), &trFooter);
        trFooter.setTextAlignment(Qt::AlignLeft);
        top = 10;
        ps = pp->addScene(0, Portrait);
    }

    th.setBorders(true, true, true, true);
    cols << 1200 << 400 << 500;
    vals << tr("OPEN BALANCE") << fReportGrid->fModel->data(r, 7).toString();
    ps->addTableRow(top, rowHeight , cols, vals, &th);


    vals << tr("DEBIT") << fReportGrid->fModel->data(r + 1, 7).toString();
    ps->addTableRow(top, rowHeight , cols, vals, &th);
    if (top > sizePortrait.height() - 200) {
        ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                        .arg(WORKING_USERNAME), &trFooter);
        trFooter.setTextAlignment(Qt::AlignRight);
        ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                        .arg(tr("Page"))
                        .arg(page++), &trFooter);
        trFooter.setTextAlignment(Qt::AlignLeft);
        top = 10;
        ps = pp->addScene(0, Portrait);
    }

    vals << tr("CREDIT") << fReportGrid->fModel->data(r + 2, 7).toString();
    ps->addTableRow(top, rowHeight , cols, vals, &th);

    if (top > sizePortrait.height() - 200) {
        ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                        .arg(WORKING_USERNAME), &trFooter);
        trFooter.setTextAlignment(Qt::AlignRight);
        ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                        .arg(tr("Page"))
                        .arg(page++), &trFooter);
        trFooter.setTextAlignment(Qt::AlignLeft);
        top = 10;
        ps = pp->addScene(0, Portrait);
    }
    vals << tr("BALANCE FOR PERIOD") << fReportGrid->fModel->data(r + 3, 7).toString();
    ps->addTableRow(top, rowHeight , cols, vals, &th);

    if (top > sizePortrait.height() - 200) {
        ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                        .arg(WORKING_USERNAME), &trFooter);
        trFooter.setTextAlignment(Qt::AlignRight);
        ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                        .arg(tr("Page"))
                        .arg(page++), &trFooter);
        trFooter.setTextAlignment(Qt::AlignLeft);
        top = 10;
        ps = pp->addScene(0, Portrait);
    }
    vals << tr("BALANCE") << fReportGrid->fModel->data(r + 4, 7).toString();
    ps->addTableRow(top, rowHeight , cols, vals, &th);

    f1.setBold(false);
    th.setFont(f1);
    th.setBorders(false, false, false, false);
    if (top > sizePortrait.height() - 400) {
        ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                        .arg(WORKING_USERNAME), &trFooter);
        trFooter.setTextAlignment(Qt::AlignRight);
        ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                        .arg(tr("Page"))
                        .arg(page++), &trFooter);
        trFooter.setTextAlignment(Qt::AlignLeft);
        top = 10;
        ps = pp->addScene(0, Portrait);
    }
    QString we = "We would be hightly grateful if you can arrange for the payment as soon as possible.\r\n"
                "Thanking you, in advance for you understanding and support.\r\n"
                "Kindly issue all cheques infavors of:\r\n\r\n"
                "For Bank Transfer Please find below our bank address.\r\n";
    top += 50;
    top += ps->addTextRect(20, top, 2100, rowHeight * 5, we, &th)->textHeight();

    top += rowHeight;


    ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                    .arg(tr("Printed"))
                    .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                    .arg(WORKING_USERNAME), &trFooter);
    trFooter.setTextAlignment(Qt::AlignRight);
    ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                    .arg(tr("Page"))
                    .arg(page), &trFooter);
    trFooter.setTextAlignment(Qt::AlignLeft);

    pp->exec();
    delete pp;
    return true;
}

void FCityLedgerDetailedBalance::setData(const QDate &date, const QString &cl)
{
    ui->deFrom->setDate(date);
    CI_CityLedger *c = CacheCityLedger::instance()->get(cl);
    if (c) {
        ui->leCL->fHiddenText = cl;
        ui->leCLCode->setText(cl);
        ui->leCL->setText(c->fName);
    }
}

void FCityLedgerDetailedBalance::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCityLedgerDetailedBalance *clb = new FCityLedgerDetailedBalance(rg);
    clb->apply(rg);
}

void FCityLedgerDetailedBalance::cityLedger(CI_CityLedger *c)
{
    dock<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCL, c);
}

void FCityLedgerDetailedBalance::openInvoice()
{
    QList<QVariant> out;
    if (fReportGrid->fillRowValuesOut(out) < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    QString inv = out.at(0).toString();
    openVaucherInvoice(inv);
}

void FCityLedgerDetailedBalance::on_btnNextDate_clicked()
{
    ui->deFrom->setDate(ui->deFrom->date().addDays(1));
    ui->deTo->setDate(ui->deTo->date().addDays(1));
    apply(fReportGrid);
}

void FCityLedgerDetailedBalance::on_btnPrevDate_clicked()
{
    ui->deFrom->setDate(ui->deFrom->date().addDays(-1));
    ui->deTo->setDate(ui->deTo->date().addDays(-1));
    apply(fReportGrid);
}

void FCityLedgerDetailedBalance::on_btnChangeCityLedger_clicked()
{
    QList<QVariant> out;
    int row = fReportGrid->fillRowValuesOut(out);
    if (out.count() == 0) {
        return;
    }
    if (out.at(0).toString() == 0) {
        return;
    }
    if (DlgChangeCLOfVaucher::changeCL(out.at(0).toString(),
                                   ui->leCLCode->text(),
                                   out.at(2).toString(),
                                       out.at(3).toDouble() > 0.1 ? out.at(3).toString() : out.at(4).toString())) {
        fReportGrid->fModel->removeRow(row);
    }
}
