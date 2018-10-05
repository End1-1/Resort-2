#include "dwselector.h"
#include "ui_dwselector.h"
#include "cachebase.h"
#include <QDebug>

DWSelector::DWSelector(int cacheId, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DWSelector)
{
    ui->setupUi(this);
    ui->btnRefresh->setVisible(false);
    hide();
    fTable = ui->tblData;
    fCacheId = cacheId;
    fLineSelector = 0;
    fCurrentRow = -1;
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(thisChangeVisibility(bool)));
}

DWSelector::~DWSelector()
{
    delete ui;
}

void DWSelector::setSelector(EQLineEdit *selector)
{
    if (fLineSelector) {
        disconnect(fLineSelector, SIGNAL(customButtonClicked(bool)), this, SLOT(thisShow(bool)));
        disconnect(fLineSelector, SIGNAL(focusOut()), this, SLOT(selectorFocusOut()));
    }
    fLineSelector = selector;
    if (fLineSelector) {
        fLineSelector->setShowButtonOnFocus(true);
        connect(fLineSelector, SIGNAL(customButtonClicked(bool)), this, SLOT(thisShow(bool)));
        connect(fLineSelector, SIGNAL(focusOut()), this, SLOT(selectorFocusOut()));
    }
}

EQLineEdit *DWSelector::selector()
{
    return fLineSelector;
}

void DWSelector::setFilterValue(const QString &text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

void DWSelector::setFilterList(const QStringList &filters)
{
    fFilterList = filters;
}

void DWSelector::onShow()
{
    if (CacheBase<CI_Base>::isEmpty(fCacheId)) {
        configure();
    }
    for (int i = 0, rowCount = fTable->rowCount(); i < rowCount; i++) {
        fTable->setRowHidden(i, false);
    }
    ui->lineEdit->setFocus();
    on_lineEdit_textEdited(ui->lineEdit->text());
}

void DWSelector::onHide()
{

}

void DWSelector::select(const QVariant &value)
{
    emit selectItem(value.value<CI_Base*>());
}

void DWSelector::refresh()
{

}

void DWSelector::applyTextFilter()
{
    setFilterValue(ui->lineEdit->text());
}

void DWSelector::adjustWidth()
{
    int width = 0;
    for (int i = 0, colCount = fTable->columnCount(); i < colCount; i++) {
        width += fTable->columnWidth(i);
    }
    width += 50;
    setMinimumWidth(width);
    resize(width, height());
}

void DWSelector::setRefreshButtonVisible(bool v)
{
    ui->btnRefresh->setVisible(v);
}

void DWSelector::thisShow(bool v)
{
    Q_UNUSED(v)
    show();
    activateWindow();
    raise();
    setFocus();
    ui->lineEdit->setFocus();
}

void DWSelector::thisChangeVisibility(bool v)
{
    if (v) {
        onShow();
    } else {
        onHide();
    }
}

void DWSelector::selectorFocusOut()
{
    emit selectItem(0);
}

void DWSelector::cacheUpdated(const QString &id)
{
    Q_UNUSED(id);
    configure();
}

void DWSelector::on_lineEdit_textEdited(const QString &arg1)
{
    for (int i = 0, rowCount = fTable->rowCount(); i < rowCount; i++) {
        for (int j = 0, colCount = fTable->columnCount(); j < colCount; j++) {
            if (fTable->item(i, j)->text().contains(arg1, Qt::CaseInsensitive)) {
                fTable->setRowHidden(i, false);
                goto mark;
            }
        }
        fTable->setRowHidden(i, true);
        mark:
        continue;
    }
}

void DWSelector::on_tblData_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QVariant value = fTable->item(index.row(), 0)->data(Qt::UserRole);
    fCurrentRow = index.row();
    select(value);
    hide();
}

void DWSelector::on_btnRefresh_clicked()
{
    refresh();
}
