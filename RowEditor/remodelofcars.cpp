#include "remodelofcars.h"
#include "ui_remodelofcars.h"
#include "wreportgrid.h"

REModelOfCars::REModelOfCars(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_MODELOF_CARS, parent),
    ui(new Ui::REModelOfCars)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leModel, "Model");
    addWidget(ui->leClass, "Class");
    fTable = "d_car_model";
    //fCacheId = 0;
}

REModelOfCars::~REModelOfCars()
{
    delete ui;
}

void REModelOfCars::openReport()
{
    QList<int> widths;
    widths << 100
           << 200
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_model"
           << "f_class";
    QStringList titles;
    titles << tr("Code")
           << tr("Model")
           << tr("Class");
    QString title = tr("Model of cars");
    QString icon = ":/images/car.png";
    QString query = "select f_id, f_model, f_class from d_car_model";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<REModelOfCars>(widths, fields, titles, title, icon, query);
}

void REModelOfCars::on_btnSave_clicked()
{
    save();
}

void REModelOfCars::on_btnCancel_clicked()
{
    reject();
}
