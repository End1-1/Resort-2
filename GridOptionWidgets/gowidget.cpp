#include "gowidget.h"

GOWidget::GOWidget(const QString &name, QWidget *parent) :
    QWidget(parent)
{
    fName = name;
    fSettings = new QSettings(_APPLICATION_, _ORGANIZATION_ + QString("\\GridOptionWidgets\\") + fName);
}

GOWidget::~GOWidget()
{
    delete fSettings;
}

QVariant GOWidget::value(const QString &name)
{
    return fSettings->value(name);
}

QVariant GOWidget::value(const QString &name, const QString &path)
{
    QSettings s(_APPLICATION_, _ORGANIZATION_ + QString("\\GridOptionWidgets\\") + path);
    return s.value(name);
}
