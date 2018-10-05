#include "reroomcategoryraterow.h"
#include "ui_reroomcategoryraterow.h"

RERoomCategoryRateRow::RERoomCategoryRateRow(int category, QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_ROOM_CATEGORY_RATE, parent),
    ui(new Ui::RERoomCategoryRateRow)
{
    ui->setupUi(this);
    ui->leCategory->setValidator(new QIntValidator());
    ui->leRate->setValidator(new QDoubleValidator(0, 9999999, 0));
    ui->leMonth1->setValidator(new QIntValidator());
    ui->leMonth2->setValidator(new QIntValidator());
    ui->leDay1->setValidator(new QIntValidator());
    ui->leDay2->setValidator(new QIntValidator());
    ui->leCode->setVisible(false);
    ui->leCategory->setVisible(false);
    ui->lbCategory->setVisible(false);
    ui->lbCode->setVisible(false);
    fDockBed = new DWSelectorBed(this);
    fDockBed->configure();
    fDockBed->setSelector(ui->leBedCode);
    connect(fDockBed, SIGNAL(bed(CI_Bed*)), this, SLOT(bed(CI_Bed*)));
    fCategory = category;
    adjustSize();
    addWidget(ui->leCode, "Code");
    addWidget(ui->leCategory, "Category");
    addWidget(ui->leMonth1, "Month1");
    addWidget(ui->leDay1, "Day1");
    addWidget(ui->leMonth2, "Month2");
    addWidget(ui->leDay2, "Day2");
    addWidget(ui->leBedCode, "");
    addWidget(ui->leBedName, "Bed");
    addWidget(ui->leRate, "Rate");
    fTable = "f_room_rate";
}

RERoomCategoryRateRow::~RERoomCategoryRateRow()
{
    delete ui;
}

void RERoomCategoryRateRow::valuesToWidgets()
{
    if (fValues.count() == 0) {
        ui->leCategory->setInt(fCategory);
    } else {
        RowEditorDialog::valuesToWidgets();
    }
}

void RERoomCategoryRateRow::bed(CI_Bed *c)
{
    dockResponse<CI_Bed, CacheBed>(ui->leBedCode, ui->leBedName, c);
}

void RERoomCategoryRateRow::on_btnCancel_clicked()
{
    reject();
}

void RERoomCategoryRateRow::on_btnOk_clicked()
{
    save();
}
