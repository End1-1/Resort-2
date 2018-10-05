#include "dlggettext.h"
#include "ui_dlggettext.h"

DlgGetText::DlgGetText(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgGetText)
{
    ui->setupUi(this);
    connect(ui->wdtKbd, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(ui->wdtKbd, SIGNAL(accept()), this, SLOT(accept()));
    connect(ui->wdtKbd, SIGNAL(reject()), this, SLOT(reject()));
}

DlgGetText::~DlgGetText()
{
    delete ui;
}

bool DlgGetText::getText(QString &out, const QString &prefix)
{
    bool result = false;
    DlgGetText *d = new DlgGetText();
    d->ui->leText->setText(prefix);
    d->ui->leText->setCursorPosition(d->ui->leText->text().length());
    d->ui->wdtKbd->setText(prefix);
    if (d->exec() == QDialog::Accepted) {
        out = d->ui->leText->text();
        result = true;
    }
    delete d;
    return result;
}

void DlgGetText::textChanged(const QString &text)
{
    ui->leText->setText(text.toUpper());
}
