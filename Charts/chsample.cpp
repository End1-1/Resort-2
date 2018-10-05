#include "chsample.h"
#include "ui_chsample.h"

ChSample::ChSample(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ChSample)
{
    ui->setupUi(this);
}

ChSample::~ChSample()
{
    delete ui;
}
