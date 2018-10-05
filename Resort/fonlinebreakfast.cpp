#include "fonlinebreakfast.h"
#include "ui_fonlinebreakfast.h"

FOnlineBreakfast::FOnlineBreakfast(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FOnlineBreakfast)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Draft breakfast"), ":/images/dinner.png");
}

FOnlineBreakfast::~FOnlineBreakfast()
{
    delete ui;
}

QWidget *FOnlineBreakfast::firstElement()
{
    return this;
}

QString FOnlineBreakfast::reportTitle()
{
    return QString("%1")
            .arg(tr("Online breakfast"));
}

void FOnlineBreakfast::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("Code"))
            .setColumn(120, "", tr("Opened"))
            .setColumn(300, "", tr("User"))
            .setColumn(80, "", tr("Pax"))
            .setColumn(80, "", tr("Price"))
            .setColumn(80, "", tr("Total"));
    QString sql = "select b.f_id, b.f_open, concat(u.f_firstName, ' ', u.f_lastName), b.f_pax, b.f_price, "
                  "b.f_pax*b.f_price "
                  "from o_breakfast b "
                  "left join users u on u.f_id=b.f_user "
            "where b.f_state=1 ";
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> col;
    col << 2 << 4;
    QList<double> val;
    rg->fModel->sumOfColumns(col, val);
    rg->setTblTotalData(col, val);
}
