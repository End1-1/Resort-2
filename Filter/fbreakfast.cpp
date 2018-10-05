#include "fbreakfast.h"
#include "ui_fbreakfast.h"
#include "wreportgrid.h"

FBreakfast::FBreakfast(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FBreakfast)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Breakfast"), ":/images/dinner.png");
    QToolButton *b = fReportGrid->addToolBarButton(":/images/biohazard.png", tr("Eliminate"), SLOT(eliminate()), this);
    b->setFocusPolicy(Qt::NoFocus);
    b->setVisible(r__(cr__super_correction));
}

FBreakfast::~FBreakfast()
{
    delete ui;
}

QString FBreakfast::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Breakfast report"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FBreakfast::firstElement()
{
    return ui->deStart;
}

void FBreakfast::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(100, "", tr("Code"))
            .setColumn(200, "", tr("Name"))
            .setColumn(80, "", tr("Pax"))
            .setColumn(80, "", tr("Amount"))
            .setColumn(100, "", tr("Payment"))
            .setColumn(150, "", tr("Comment"))
            .setColumn(80, "", tr("Room"))
            .setColumn(200, "", tr("Guest"))
            .setColumn(200, "", tr("User"));
    QString sql = "select m.f_wdate, m.f_id,  m.f_finalName, m.f_amountAmd/6500, m.f_amountAmd, \
            pm.f_en, m.f_paymentComment,m.f_room, m.f_guest, u.f_username \
            from m_register m \
            left join f_payment_type pm on pm.f_id =m.f_paymentMode \
            left join users u on u.f_id=m.f_user \
            where m.f_wdate between :f_date1 and :f_date2 and m.f_canceled=0 and m.f_itemCode in (17, 82) "    ;
    sql.replace(":f_date1", ui->deStart->dateMySql()).replace(":f_date2", ui->deEnd->dateMySql());
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 3 << 4;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FBreakfast::eliminate()
{
    QList<QVariant> out;
    int row = fReportGrid->fillRowValuesOut(out);
    if (row < 0) {
        message_info_tr("Nothing was selected");
        return;
    }
    if (message_confirm(tr("THIS WILL REMOVE PERMANENTLY BREAKFAST DATA!")) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] =out.at(0);
    fDb.select("delete from o_breakfast where f_id=:f_id", fDbBind, fDbRows);
    fDbBind[":f_id"] =out.at(0);
    fDb.select("delete from m_register where f_id=:f_id", fDbBind, fDbRows);
    fReportGrid->fModel->removeRow(row);
}
