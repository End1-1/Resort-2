#include "recustomaction.h"
#include "wreportgrid.h"

RECustomAction::RECustomAction(QWidget *parent) :
    BaseExtendedDialog(parent)
{
    fParent = static_cast<WReportGrid*>(parent);
}

void RECustomAction::setup(const QString &icon, const QString &text)
{
    fParent->addToolBarButton(icon, text, SLOT(clicked()), this);
    connect(this, SIGNAL(processValues(QList<QVariant>)), this, SLOT(handleValues(QList<QVariant>)));
}

void RECustomAction::clicked()
{
    QList<QVariant> out;
    fParent->fillRowValuesOut(out);
    emit processValues(out);
}
