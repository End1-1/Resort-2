#include "dlgsalarytotal.h"
#include "ui_dlgsalarytotal.h"
#include "c5printing.h"
#include "database2.h"
#include "defrest.h"

DlgSalaryTotal::DlgSalaryTotal(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSalaryTotal)
{
    ui->setupUi(this);
    ui->deDate1->setDate(QDate::currentDate());
    ui->deDate2->setDate(QDate::currentDate());
    showMaximized();
}

DlgSalaryTotal::~DlgSalaryTotal()
{
    delete ui;
}

void DlgSalaryTotal::on_btnSave_clicked()
{
    fDbBind[":f_date1"] = ui->deDate1->date();
    fDbBind[":f_date2"] = ui->deDate2->date();
    QString where;
    if (ui->cbBranch->currentIndex() > 0) {
        where += QString(" and s.f_branch=%1 ").arg(ui->cbBranch->currentIndex());
    }
    DatabaseResult dr;
    dr.select(fDb, "select 0, s.f_employee, concat(u.f_firstName, ' ', u.f_lastName) as ename, "
            "sum(abs(s.f_amount)) as f_amount, "
              "p.f_am as position "
               "from salary2 s "
               "left join users u on u.f_id=s.f_employee "
                "left join users_groups p on p.f_id=u.f_group "
                "where s.f_date between :f_date1 and :f_date2 " + where
               + "group by 3 ", fDbBind);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 0, 500, 100, 400);
    ui->tblData->setRowCount(0);
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        ui->tblData->setItemWithValue(row, 0, dr.value(i, "f_id"));
        ui->tblData->setItemWithValue(row, 1, dr.value(i, "f_employee"));
        ui->tblData->setItemWithValue(row, 2, dr.value(i, "ename"));
        ui->tblData->setItemWithValue(row, 3, dr.value(i, "f_amount"));
        ui->tblData->setItemWithValue(row, 4, dr.value(i, "position"));
    }
}

void DlgSalaryTotal::on_btnPrint_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db;
    db.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    C5Printing p;
    p.setSceneParams(2000, 2700, QPrinter::Portrait);
    QFont font(font());
    int fontbase = 26;
    font.setPointSize(fontbase);
    font.setBold(true);
    p.setFont(font);
    p.setFontSize(fontbase + 4);
    p.setFontBold(true);
    p.ctext(tr("<<ԷԼԻՏ>> ԱՎՏՈԼՎԱՑՈՒՄ"));
    p.br();

    p.setFontSize(fontbase);
    p.setFontBold(false);
    p.ctext(tr("ԱՇԽԱՏԱՎԱՐՁԻ ՎՃԱՐՄԱՆ ՓԱՍԹԱԹՈՒՂԹ"));
    p.br();
    p.ctext(ui->cbBranch->currentText());
    p.setFontBold(false);
    p.br();
    p.ctext(ui->deDate1->text() + "-" + ui->deDate2->text());
    p.br();
    p.br();

    QList<qreal> points;
    points << 10 << 100 << 300 << 500 << 250 << 350;
    QStringList vals;
    vals << tr("NN") << tr("ՀԱՍՏԻԿ") << tr("ԱՆՈՒՆ") << tr("ԳՈՒՄԱՐ") <<  tr("ՍՏՈՐԱԳՐՈՒԹՅՈՒՆ");
    p.tableText(points, vals, p.fLineHeight + 30);
    p.br(p.fLineHeight + 30);


    db[":f_date1"] = ui->deDate1->date();
    db[":f_date2"] = ui->deDate2->date();
    QString where;
    if (ui->cbBranch->currentIndex() > 0) {
        where += QString(" and s.f_branch=%1 ").arg(ui->cbBranch->currentIndex());
    }
    db.exec("select ug.f_en as f_groupname, concat_ws(' ', u.f_lastname, u.f_firstname) as f_employeename, "
            "sum(s.f_amount) as f_amount "
            "from salary2 s "
            "left join users u on u.f_id=s.f_employee "
            "left join users_groups ug on ug.f_id=u.f_group "
            "where s.f_date between :f_date1 and :f_date2 "
            + where
            + " group by 2 ");

    double total = 0;
    int row = 1;
    while (db.next()) {
        total += db.doubleValue("f_amount");
        vals.clear();
        vals << QString::number(row++)
             << db.string("f_groupname")
             << db.string("f_employeename")
             << float_str(db.doubleValue("f_amount"), 2)
             << "";
        p.tableText(points, vals, p.fLineHeight + 30);
        p.br(p.fLineHeight + 30);
    }
    points.clear();
    points << 910 << 250 << 350;
    vals.clear();
    vals << tr("ԸՆԴԱՄԵՆԸ") << float_str(total, 2);
    p.tableText(points, vals, p.fLineHeight + 30);

#ifdef QT_DEBUG
    p.print("local", QPrinter::A4);
#else
    p.print("salary", QPrinter::A4);
#endif
}

void DlgSalaryTotal::on_btnClose_clicked()
{
    accept();
}
