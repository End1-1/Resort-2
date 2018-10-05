#include "efilterlineedit.h"
#include "ui_efilterlineedit.h"

EFilterLineEdit::EFilterLineEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EFilterLineEdit)
{
    ui->setupUi(this);
}

EFilterLineEdit::~EFilterLineEdit()
{
    delete ui;
}
