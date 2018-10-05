#include "rdishcomment.h"
#include "ui_rdishcomment.h"

RDishComment::RDishComment(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RDishComment)
{
    ui->setupUi(this);
    connect(ui->wKbd, SIGNAL(textChanged(QString)), this, SLOT(leTextChanged(QString)));
    connect(ui->wKbd, SIGNAL(accept()), this, SLOT(accept()));
    connect(ui->wKbd, SIGNAL(reject()), this, SLOT(reject()));
    ui->tblVariants->setHidden(true);
}

RDishComment::~RDishComment()
{
    delete ui;
}

bool RDishComment::getComment(QString &text, QWidget *parent)
{
    bool result = false;
    RDishComment *c = new RDishComment(parent);
    if (c->exec() == QDialog::Accepted) {
        result = true;
        text = c->ui->leText->text();
    }
    delete c;
    return result;
}

void RDishComment::leTextChanged(const QString &text)
{
    ui->leText->setText(text);
}

void RDishComment::on_leText_textChanged(const QString &arg1)
{
    ui->wKbd->setText(arg1);
}
