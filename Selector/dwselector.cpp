#include "dwselector.h"
#include "ui_dwselector.h"
#include "cachebase.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QSizePolicy>

DWSelector::DWSelector(int cacheId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DWSelector)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window, true);
    setWindowModality(Qt::NonModal);
    ui->btnRefresh->setVisible(false);
    ui->tblData->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hide();
    fTable = ui->tblData;
    fCacheId = cacheId;
    fLineSelector = 0;
    fCurrentRow = -1;
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

void DWSelector::setFilterColumn(const QMap<int, QString> &filter)
{
    fColumnFilter = filter;
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
    onShow();

    show();
    if (fLineSelector) {
        QPoint p = fLineSelector->mapToGlobal(QPoint(0, fLineSelector->height()));
        QScreen *scr = QGuiApplication::screenAt(p);
        if (!scr) {
            scr = QGuiApplication::primaryScreen();
        }
        if (scr) {
            const QRect ar = scr->availableGeometry();
            QSize s = frameGeometry().size();
            if (!s.isValid()) {
                s = sizeHint();
            }
            const int x = qMax(ar.left(), qMin(p.x(), ar.right() - s.width() + 1));
            const int y = qMax(ar.top(), qMin(p.y(), ar.bottom() - s.height() + 1));
            p = QPoint(x, y);
        }
        move(p);
    }
    activateWindow();
    raise();
    setFocus();
    ui->lineEdit->setFocus();
}

void DWSelector::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
}

void DWSelector::hideEvent(QHideEvent *event)
{
    onHide();
    QDialog::hideEvent(event);
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
    QList<int> cols;
    for (int i = 0; i < fTable->columnCount(); i++) {
        cols << i;
    }
    for (int i = 0, rowCount = fTable->rowCount(); i < rowCount; i++) {
        if (fColumnFilter.count() > 0) {
            for (QMap<int, QString>::const_iterator it = fColumnFilter.begin(); it != fColumnFilter.end(); it++) {
                if (!fTable->item(i, it.key())->text().contains(it.value(), Qt::CaseInsensitive)) {
                    fTable->setRowHidden(i, true);
                    goto mark;
                }
            }
        }
        foreach (int j, cols) {
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
