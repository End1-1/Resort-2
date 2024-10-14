#include "wreportgrid.h"
#include "ui_wreportgrid.h"
#include "excel.h"
#include "dlgfiltervalues.h"
#include "wfilterbase.h"
#include "pprintscene.h"
#include "pimage.h"
#include "ptextrect.h"
#include "dlgconfiggrid.h"
#include "dlghelp.h"
#include <QScrollBar>
#include <QMenu>
#include <QClipboard>

WReportGrid::WReportGrid(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WReportGrid)
{
    ui->setupUi(this);
    ui->tblTotals->setVisible(false);
    ui->tblMain->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tblMain->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->btnFilter->setVisible(false);
    ui->btnHelp->setVisible(false);
    fModel = new TableModel(ui->tblMain);
    connect(fModel, SIGNAL(rowCount(int)), this, SLOT(rowCount(int)));
    fModel->fTableView = ui->tblMain;
    fTableView = ui->tblMain;
    fTableTotal = ui->tblTotals;
    fRowEditorDialog = 0;
    ui->btnNew->setVisible(false);
    fFilter = 0;
    fTitleWidget = 0;
    ui->wFilter->hide();
    ui->tblMain->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    fTableMenu = new QMenu();
    connect(fTableMenu->addAction("Filter on column values"), SIGNAL(triggered(bool)), this, SLOT(actionFilterColumn(bool)));
    connect(ui->tblMain->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tblMainCustomeMenu(QPoint)));
    connect(ui->tblTotals->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(totalGridHScroll(int)));
    connect(ui->tblMain->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(tblMainGridHScroll(int)));
    connect(ui->tblMain->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(tblMainHeaderResized(int,int,int)));
    connect(ui->tblMain->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(tblMainHeaderClicked(int)));
    fGridMenu = new QMenu();
    connect(fGridMenu->addAction("Copy"), SIGNAL(triggered(bool)), this, SLOT(actionCopyGrid(bool)));
    connect(ui->tblMain, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tblMaingridCustomMenu(QPoint)));
    fTrackControl = new TrackControl(TRACK_REPORT);
    connect(fModel, SIGNAL(newSum(QList<int>,QList<double>)), this, SLOT(newSum(QList<int>,QList<double>)));
    connect(fModel, SIGNAL(endApply()), this, SLOT(endApply()));
}

WReportGrid::~WReportGrid()
{
    delete ui;
}

void WReportGrid::getModelData(QList<QList<QVariant> > &out)
{
    out.clear();
}

void WReportGrid::setHelp(const QString &helpFile)
{
    fHelp = helpFile;
    ui->btnHelp->setVisible(!fHelp.isEmpty());
}

void WReportGrid::dontResizeSave(bool v)
{
    fSaveResizedColumn = !v;
}

void WReportGrid::processValues(int row, bool isNew)
{
    if (isNew) {
        fRowValues.clear();
    } else {
        emit doubleClickOnRow(fRowValues);
    }
    if (fRowEditorDialog) {
        fRowEditorDialog->setValues();
        if (fRowEditorDialog->exec() == QDialog::Accepted) {
            if (isNew) {
                fModel->appendRow(fRowValues);
            } else {
                fModel->updateRowValues(row, fRowValues);
            }
        }
    } else {
        if (isNew) {
            emit newBtn();
        }
    }
}

void WReportGrid::setBtnNewVisible(bool value)
{
    ui->btnNew->setVisible(value);
}

QWidget *WReportGrid::gridOptionWidget()
{
    if (fFilter) {
        return fFilter->gridOptionWidget();
    }
    return 0;
}

void WReportGrid::totalGridHScroll(int value)
{
    ui->tblMain->horizontalScrollBar()->setValue(value);
}

void WReportGrid::tblMainGridHScroll(int value)
{
    ui->tblTotals->horizontalScrollBar()->setValue(value);
}

QToolButton *WReportGrid::addToolBarButton(const QString &image, const QString &text, const char *slot, QObject *receiver)
{
    QToolButton *b = new QToolButton(this);
    b->setIcon(QIcon(image));
    b->setText(text);
    b->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    if (!receiver) {
        receiver = this;
    }
    connect(b, SIGNAL(clicked()), receiver, slot);
    ui->hlToolbar->addWidget(b);
    return b;
}

int WReportGrid::fillRowValuesOut(QList<QVariant> &out)
{
    out.clear();
    QModelIndexList sel = ui->tblMain->selectionModel()->selectedIndexes();
    if (sel.count() == 0) {
        return -1;
    }
    for (int i = 0; i < fModel->columnCount(); i++) {
        out << fModel->data(sel.at(0).row(), i, Qt::EditRole);
    }
    return sel.at(0).row();
}

void WReportGrid::setTblTotalData(const QList<int> &columns, const QList<double> &values)
{
    ui->tblTotals->setVisible(true);
    ui->tblMain->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QStringList l;
    l << QString::number(fModel->rowCount());
    ui->tblTotals->clear();
    for (int i = 0, count = columns.count(); i < count; i++) {
        ui->tblTotals->setItemWithValue(0, columns.at(i), values.at(i));
    }
    ui->tblTotals->setVerticalHeaderLabels(l);
}

void WReportGrid::setTblNoTotalData()
{
    ui->tblTotals->setVisible(false);
    ui->tblMain->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tblTotals->clear();
}

void WReportGrid::countToTotal(int col)
{
    ui->tblTotals->setItem(0, col, new QTableWidgetItem(QString::number(fModel->rowCount())));
}

void WReportGrid::setupTabTextAndIcon(const QString &name, const QString &image)
{
    fGridClassName = name;
    BaseWidget::setupTabTextAndIcon(name, image);
}

void WReportGrid::on_leQuickSearch_textChanged(const QString &arg1)
{
    fModel->searchInTable(arg1.trimmed());
}

void WReportGrid::on_btnExcel_clicked()
{
    int colCount = fModel->columnCount();
    int rowCount = fModel->rowCount();
    if (colCount == 0 || rowCount == 0) {
        message_error_tr("Empty report!");
        return;
    }
    Excel e;
    for (int i = 0; i < colCount; i++) {
        e.setValue(fModel->columnTitle(i), 1, i + 1);
        e.setColumnWidth(i + 1, fModel->columnWidth(i) / 7);
    }
    QColor color = QColor::fromRgb(200, 200, 250);
    e.setBackground(e.address(0, 0), e.address(0, colCount - 1),
                     color.red(), color.green(), color.blue());
    e.setFontBold(e.address(0, 0), e.address(0, colCount - 1));
    e.setHorizontalAlignment(e.address(0, 0), e.address(0, colCount - 1), Excel::hCenter);

    for (int j = 0; j < rowCount; j++) {
        for (int i = 0; i < colCount; i++) {
            e.setValue(fModel->stringData(j, i), j + 2, i + 1);
        }
        color = fModel->data(j, 0, Qt::BackgroundColorRole).value<QColor>();
        e.setBackground(e.address(j + 1, 0), e.address(j + 1, colCount - 1),
                         color.red(), color.green(), color.blue());
    }

    e.setFontSize(e.address(0, 0), e.address(rowCount , colCount ), 10);
    e.show();
}

void WReportGrid::on_tblMain_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    fillRowValues(index.row());
    processValues(index.row(), false);
    emit doubleClickOnRow2(index.row(), index.column(), fRowValues);

}

void WReportGrid::on_btnNew_clicked()
{
    processValues(-1, true);
}

void WReportGrid::on_btnRefresh_clicked()
{
    if (fFilter) {
        fFilter->apply(this);
    } else {
        fModel->apply(this);
    }
    if (!ui->leQuickSearch->text().isEmpty()) {
        on_leQuickSearch_textChanged(ui->leQuickSearch->text());
    }
}

void WReportGrid::on_btnFilter_clicked()
{
    if (fFilter) {
        ui->wFilter->setVisible(true);
        if (fFilter->firstElement()) {
            fFilter->firstElement()->setFocus();
        } else {
            ui->wFilter->hide();
        }
    }
}

void WReportGrid::fillRowValues(int row)
{
    fRowValues.clear();
    for (int i = 0; i < fModel->columnCount(); i++) {
        fRowValues << fModel->data(row, i, Qt::EditRole);
    }
}

void WReportGrid::addFilterWidget(WFilterBase *f)
{
    fFilter = f;
    ui->filterLayout->addWidget(f);
    ui->btnFilter->setVisible(true);
    ui->wFilter->setVisible(true);
    if (fFilter->firstElement()) {
        fFilter->firstElement()->setFocus();
    } else {
        ui->wFilter->hide();
    }
}

void WReportGrid::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_E:
        if (event->modifiers() & Qt::ControlModifier) {
            on_btnExcel_clicked();
        }
        break;
    case Qt::Key_P:
        if (event->modifiers() & Qt::ControlModifier) {
            on_btnPrint_clicked();
        }
        break;
    case Qt::Key_S:
        if (event->modifiers() & Qt::ControlModifier) {
            ui->leQuickSearch->setFocus();
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (focusWidget() == ui->btnApplyFilter) {
            ui->btnApplyFilter->click();
        }
        break;
    }
    QWidget::keyPressEvent(event);
}

bool WReportGrid::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        switch (ke->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (ke->modifiers() & Qt::ControlModifier || ui->btnApplyFilter->hasFocus()) {
                on_btnApplyFilter_clicked();
            }
            return true;
        }
    }
    return BaseWidget::event(event);
}

void WReportGrid::newSum(const QList<int> &columns, const QList<double> &values)
{
    setTblTotalData(columns, values);
}

void WReportGrid::tblMainHeaderClicked(int index)
{
    fModel->sort(index);
}

void WReportGrid::tblMainHeaderResized(int index, int oldSize, int newSize)
{
    Q_UNUSED(oldSize)
    ui->tblTotals->setColumnWidth(index, newSize);
    if (fSaveResizedColumn) {
        QSettings s(_ORGANIZATION_, _APPLICATION_ + QString("\\Grid\\") + fGridClassName);
        QStringList cols;
        for (int i = 0; i < fModel->columnCount(); i++) {
            cols << QString::number(ui->tblMain->columnWidth(i));
        }
        s.setValue("ColumnsWidths", cols.join(","));
    }
}

void WReportGrid::tblMainCustomeMenu(const QPoint &point)
{
    QModelIndexList sel = fTableView->selectionModel()->selectedColumns();
    if (sel.count() == 0) {
        int col = ui->tblMain->horizontalHeader()->logicalIndexAt(point);
        ui->tblMain->selectColumn(col);
    }
    fTableMenu->popup(ui->tblMain->mapToGlobal(point));
}

void WReportGrid::tblMaingridCustomMenu(const QPoint &point)
{
    QModelIndexList sel = fTableView->selectionModel()->selectedIndexes();
    if (sel.count() == 0) {
        return;
    }
    fGridMenu->popup(ui->tblMain->mapToGlobal(point));
}

void WReportGrid::actionCopyGrid(bool v)
{
    Q_UNUSED(v);
    QModelIndexList sel = fTableView->selectionModel()->selectedIndexes();
    if (sel.count() == 0) {
        return;
    }
    QString data;
    int currCol = -1;
    bool first = true;
    foreach (QModelIndex m, sel) {
        if (currCol < 0) {
            currCol = m.row();
        }
        if (currCol != m.row()) {
            currCol = m.row();
            first = true;
            data += "\r\n";
        }
        if (first) {
            first = false;
        } else {
            data += "\t";
        }
        data += m.data(Qt::DisplayRole).toString();
    }
    QClipboard *c = qApp->clipboard();
    c->setText(data);
}

void WReportGrid::actionFilterColumn(bool v)
{
    Q_UNUSED(v)
    QSet<QString> filterValues;
    QModelIndexList sel = fTableView->selectionModel()->selectedColumns();
    if (sel.count() == 0) {
        return;
    }
    fModel->uniqueValuesForColumn(sel.at(0).column(), filterValues);
    QStringList sortedValues = filterValues.toList();
    std::sort(sortedValues.begin(), sortedValues.end());
    DlgFilterValues *d = new DlgFilterValues(sortedValues, this);
    if (d->exec() == QDialog::Accepted) {
        QString searchText;
        bool first = true;
        for (QStringList::const_iterator it = sortedValues.begin(); it != sortedValues.end(); it ++) {
            if (first) {
                first = false;
            } else {
                searchText += "|";
            }
            searchText += *it;
        }
        fModel->searchInTable(searchText);
    }
    delete d;
}

void WReportGrid::rowCount(int count)
{
    QStringList h;
    h << QString::number(count);
    ui->tblTotals->setVerticalHeaderLabels(h);
    ui->tblTotals->setColumnCount(fModel->columnCount());
    for (int i = 0; i < fModel->columnCount(); i++) {
        ui->tblTotals->setColumnWidth(i, ui->tblMain->columnWidth(i));
    }
}

void WReportGrid::on_tblMain_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        emit clickOnRow(-1);
        return;
    }
    fillRowValues(index.row());
    emit clickOnRow(index.row());
}

void WReportGrid::on_btnHideFilter_clicked()
{
    ui->wFilter->hide();
}

void WReportGrid::on_btnApplyFilter_clicked()
{
    if (fFilter) {
        fFilter->apply(this);
    }
    ui->tblMain->horizontalScrollBar()->setValue(0);
    ui->tblTotals->horizontalScrollBar()->setValue(0);
}

void WReportGrid::on_btnClearQuickSearch_clicked()
{
    ui->leQuickSearch->clear();
    ui->tblMain->clearSelection();
}


void WReportGrid::on_btnPrint_clicked()
{
    if (fFilter) {
        if (fFilter->handlePrint()) {
            return;
        }
    }
    int page =  1;
    qreal top = 10;
    int totalWidth = 0;
    QList<qreal> distanceCol;
    distanceCol << 10.000 ;
    for (int i = 0, colCount = fModel->columnCount(); i < colCount; i++) {
        int colw = ui->tblMain->columnWidth(i);
        totalWidth += (colw * resize_factor);
        distanceCol << (distanceCol.at(i) + colw);
    }

    QBrush b(Qt::white, Qt::SolidPattern);
    PTextRect trFooter;
    trFooter.setBrush(b);
    QFont f(qApp->font().family(), 20);
    trFooter.setFont(f);
    trFooter.setBorders(false, false, false, false);
    trFooter.setTextAlignment(Qt::AlignLeft);

    QSettings s("SmartHotel", "SmartHotel\\Grid\\" + fGridClassName);
    PrintOrientation po = (totalWidth > sizePortrait.width() - 50 ? Landscape : Portrait);
    QSize paperSize = (po == Portrait ? sizePortrait : sizeLandscape);
    int footerTop = paperSize.height() - 50;
    PTextRect prTempl;
    prTempl.setWrapMode(QTextOption::NoWrap);
    prTempl.setFont(QFont(s.value("FontName").toString(), 17 + s.value("FontSize").toInt()));
    prTempl.setFontBold(s.value("FontBold", false).toBool());
    prTempl.setBorders(true, true, true, true);
    QPen pen;
    pen.setWidth(2);
    prTempl.setRectPen(pen);
    PTextRect prHead(prTempl, "");
    prHead.setFont(QFont(qApp->font().family(), 20));
    prHead.setBrush(QBrush(QColor::fromRgb(215, 215, 215), Qt::SolidPattern));
    prHead.setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    int rowHeight = ui->tblMain->verticalHeader()->defaultSectionSize();
    int headerHeight = ui->tblMain->horizontalHeader()->height() * resize_factor * 2;
    PPrintPreview *pp = new PPrintPreview(this);
    PPrintScene *ps = pp->addScene(0, po);
    QString title = "NO TITLE";
    bool official = false;
    if (fFilter) {
        title = fFilter->reportTitle();
        official = fFilter->officialTitle();
    } else if (fTabWidget) {
        title = fTabWidget->tabText(fTabIndex);
    }
    if (fTitleWidget) {
        official = false;
        title = fTitleWidget->title();
    }
    int topOffcet = 0;
    if (official) {
        PTextRect *trInfo = new PTextRect(1500, 20, 600, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                          0, QFont(qApp->font().family(), 25));
        trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
        trInfo->setBorders(false, false, false, false);
        ps->addItem(trInfo);
        PImage *logo = new PImage("logo_print.png");
        ps->addItem(logo);
        logo->setRect(QRectF(20, 10, 500, 300));
        PTextRect th(prTempl, title);
        QFont f1(qApp->font().family(), 30);
        f1.setBold(true);
        th.setFont(f1);
        th.setTextAlignment(Qt::AlignCenter);
        th.setWrapMode(QTextOption::WordWrap);
        th.setBorders(false, false, false, false);
        ps->addTextRect(20, 50, 2100, rowHeight, title, &th);
        topOffcet = 310;
    } else {
        PTextRect *caption = new PTextRect(prTempl, QString("%1")
                                           .arg(title));
        QFont captionFont(qApp->font().family(), 20);
        captionFont.setItalic(true);
        caption->setWrapMode(QTextOption::NoWrap);
        captionFont.setBold(true);
        caption->setFont(captionFont);
        caption->setRect(20, 0, totalWidth, (rowHeight * resize_factor) + 20);
        caption->setBorders(false, false, false, false);
        caption->setTextAlignment(Qt::AlignHCenter);
        ps->addTextRect(caption);
        caption->setBorders(false, false, false, false);
        //caption->adjustRect();
        topOffcet = caption->height();
    }

    prTempl.setWrapMode(QTextOption::NoWrap);
    top += topOffcet;
    for (int i = 0; i < fModel->columnCount(); i++) {
        if (ui->tblMain->columnWidth(i) == 0){
            continue;
        }
        PTextRect *tr = new PTextRect(distanceCol.at(i) * resize_factor, top,
                                      ui->tblMain->columnWidth(i) * resize_factor, headerHeight ,
                                      fModel->columnTitle(i), &prHead);
        ps->addTextRect(tr);

    }
    top += headerHeight;
    for (int i = 0, rowCount = fModel->rowCount(); i < rowCount; i++) {
        rowHeight = ui->tblMain->verticalHeader()->defaultSectionSize();
        for (int j = 0, colCount = fModel->columnCount(); j < colCount; j++) {
            if (ui->tblMain->columnWidth(j) == 0) {
                continue;
            }
            QBrush br(fModel->data(i, j, Qt::BackgroundColorRole).value<QColor>());
            if (br.color() == Qt::white) {
                br.setStyle(Qt::NoBrush);
            }
            prTempl.setBrush(br);
            if (top > paperSize.height()  - 100) {
                /*footer*/
                ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                                .arg(tr("Printed"))
                                .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                                .arg(WORKING_USERNAME), &trFooter);
                trFooter.setTextAlignment(Qt::AlignRight);
                ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                                .arg(tr("Page"))
                                .arg(page), &trFooter);
                trFooter.setTextAlignment(Qt::AlignLeft);
                /*header*/
                rowHeight = ui->tblMain->verticalHeader()->defaultSectionSize();
                top = 5;
                page++;
                ps = pp->addScene(0, po);
                if (official) {
                    PTextRect *trInfo = new PTextRect(1500, 20, 600, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                                      0, QFont(qApp->font().family(), 25));
                    trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
                    trInfo->setBorders(false, false, false, false);
                    ps->addItem(trInfo);
                    PImage *logo = new PImage("logo_print.png");
                    ps->addItem(logo);
                    logo->setRect(QRectF(20, 10, 500, 300));
                    PTextRect th(prTempl, title);
                    QFont f1(qApp->font().family(), 30);
                    f1.setBold(true);
                    th.setFont(f1);
                    th.setWrapMode(QTextOption::WordWrap);
                    th.setTextAlignment(Qt::AlignCenter);
                    th.setBorders(false, false, false, false);
                    ps->addTextRect(20, 50, 2100, rowHeight, title, &th);
                    topOffcet = 310;
                } else {
                    PTextRect *caption = new PTextRect(prTempl, QString("%1")
                                                       .arg(title));
                    QFont captionFont(qApp->font().family(), 20);
                    captionFont.setItalic(true);
                    caption->setWrapMode(QTextOption::NoWrap);
                    captionFont.setBold(true);
                    caption->setFont(captionFont);
                    caption->setRect(10, 0,  totalWidth, ui->tblMain->verticalHeader()->defaultSectionSize() * 3.5 * resize_factor);
                    caption->setBorders(false, false, false, false);
                    caption->setTextAlignment(Qt::AlignHCenter);
                    ps->addTextRect(caption);
                    caption->setBorders(false, false, false, false);
                    topOffcet = caption->height();
                }
                top += topOffcet;
                for (int n = 0; n < fModel->columnCount(); n++) {
                    PTextRect *tr = new PTextRect(distanceCol.at(n) * resize_factor, top,
                                                  ui->tblMain->columnWidth(n) * resize_factor,
                                                  headerHeight  ,
                                                  fModel->columnTitle(n), &prHead);
                    ps->addTextRect(tr);

                }
                top += headerHeight;
            }
            int colwidth = ui->tblMain->columnWidth(j);
            int span = ui->tblMain->columnSpan(i, j);
            if (span > 1) {
                colwidth = 0;
                for (int k = j, count = j + span; k < count; k++) {
                    colwidth += ui->tblMain->columnWidth(k);
                }
            }
            rowHeight = ui->tblMain->rowHeight(i);
            PTextRect *tr = new PTextRect(distanceCol.at(j) * resize_factor, top,
                                          colwidth * resize_factor, (rowHeight * resize_factor) + 20,
                                          fModel->data(i, j).toString(), &prTempl);

            j += span - 1;
            ps->addTextRect(tr);
        }
        top += (rowHeight * resize_factor) + 20;
    }
    top += rowHeight;
    if (ui->tblTotals->isVisible()) {
        QFont font(qApp->font().family(), 18);
        font.setBold(true);
        font.setPointSize(font.pointSize() - 1);
        prTempl.setFont(font);
        for (int j = 0, colCount = fModel->columnCount(); j < colCount; j++) {
            if (ui->tblMain->columnWidth(j) == 0) {
                continue;
            }
            if (top > paperSize.height()  - 100) {
                /*footer*/
                ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                                .arg(tr("Printed"))
                                .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                                .arg(WORKING_USERNAME), &trFooter);
                trFooter.setTextAlignment(Qt::AlignRight);
                ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                                .arg(tr("Page"))
                                .arg(page), &trFooter);
                trFooter.setTextAlignment(Qt::AlignLeft);
                /*header*/
                top = 5;
                page++;
                ps = pp->addScene(0, po);
                if (official) {
                    PTextRect *trInfo = new PTextRect(1500, 20, 600, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                                      0, QFont(qApp->font().family(), 25));
                    trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
                    trInfo->setBorders(false, false, false, false);
                    ps->addItem(trInfo);
                    PImage *logo = new PImage("logo_print.png");
                    ps->addItem(logo);
                    logo->setRect(QRectF(20, 10, 500, 300));
                    PTextRect th(prTempl, title);
                    QFont f1(qApp->font().family(), 30);
                    f1.setBold(true);
                    th.setFont(f1);
                    th.setTextAlignment(Qt::AlignCenter);
                    th.setBorders(false, false, false, false);
                    ps->addTextRect(20, 50, 2100, rowHeight, title, &th);
                    topOffcet = 310;
                } else {
                    PTextRect *caption = new PTextRect(prTempl, QString("%1")
                                                       .arg(title));
                    QFont captionFont(qApp->font().family(), 20);
                    captionFont.setItalic(true);
                    caption->setWrapMode(QTextOption::NoWrap);
                    captionFont.setBold(true);
                    caption->setFont(captionFont);
                    caption->setRect(10, 0, 1200, ui->tblMain->verticalHeader()->defaultSectionSize() * 1.5 * resize_factor);
                    caption->setBorders(false, false, false, false);
                    ps->addTextRect(caption);
                    caption->setTextAlignment(Qt::AlignRight);
                    caption->setBorders(false, false, false, false);
                    topOffcet = caption->height();
                }
                top += topOffcet;
                for (int n = 0; n < fModel->columnCount(); n++) {
                    PTextRect *tr = new PTextRect(distanceCol.at(n) * resize_factor, top,
                                                  ui->tblMain->columnWidth(n) * resize_factor,
                                                  headerHeight  ,
                                                  fModel->columnTitle(n), &prHead);
                    ps->addTextRect(tr);

                }
                top += headerHeight ;
            }
            int colwidth = ui->tblMain->columnWidth(j);
            int span = ui->tblMain->columnSpan(0, j);
            if (span > 1) {
                colwidth = 0;
                for (int k = j; k < j + span; k++) {
                    colwidth += ui->tblMain->columnWidth(k);
                }
            }
            if (!ui->tblTotals->item(0, j)) {
                ui->tblTotals->setItem(0, j, new QTableWidgetItem());
            }
            PTextRect *tr = new PTextRect(distanceCol.at(j) * resize_factor, top,
                                          colwidth * resize_factor, (rowHeight * resize_factor) + 20,
                                          ui->tblTotals->toString(0, j), &prTempl);

            j += span - 1;
            ps->addTextRect(tr);
        }
        top += (rowHeight * resize_factor) + 20;
    }

    if (fFilter) {
        fFilter->finalPrint(ps, top);
    }

    ps->addTextRect(20, footerTop, 1800, rowHeight * 3, QString("%1: %2 %3")
                    .arg(tr("Printed"))
                    .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                    .arg(WORKING_USERNAME), &trFooter);
    trFooter.setTextAlignment(Qt::AlignRight);
    ps->addTextRect(1800, footerTop, 200, rowHeight * 3, QString("%1 %2")
                    .arg(tr("Page"))
                    .arg(page), &trFooter);
    trFooter.setTextAlignment(Qt::AlignLeft);

    pp->exec();
    delete pp;

    fTrackControl->insert("Print report", title, "");
}

void WReportGrid::on_btnHelp_clicked()
{
    DlgHelp *d = new DlgHelp(fHelp, this);
    d->exec();
    delete d;
}

void WReportGrid::on_btnConfigGrid_clicked()
{
    QSettings s("SmartHotel", "SmartHotel\\Grid\\" + fGridClassName);
    QString fName = s.value("FontName").toString();
    int fSize = s.value("FontSize").toInt();
    bool fBold = s.value("FontBold").toBool();
    if (!DlgConfigGrid::config(fName, fSize, fBold, this)) {
        return;
    }
    s.setValue("FontName", fName);
    s.setValue("FontSize", fSize);
    s.setValue("FontBold", fBold);

}

void WReportGrid::endApply()
{
    QSettings s(_ORGANIZATION_, _APPLICATION_ + QString("\\Grid\\") + fGridClassName);
    QStringList cols = s.value("ColumnsWidths").toString().split(",", QString::SkipEmptyParts);
    if (cols.count() != fModel->columnCount()) {
        return;
    }
    for (int i = 0; i < cols.count(); i++) {
        ui->tblMain->setColumnWidth(i, cols[i].toInt());
    }
}
