#include "customerdisplay.h"

#include <QFrame>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QWindow>

CustomerDisplay *CustomerDisplay::fInstance = nullptr;

CustomerDisplay *CustomerDisplay::instance()
{
    return fInstance;
}

void CustomerDisplay::tryAttachSecondScreen()
{
    if(fInstance) {
        return;
    }
    const QList<QScreen *> screens = QGuiApplication::screens();
    if(screens.size() < 2) {
        return;
    }

    fInstance = new CustomerDisplay();
    QScreen *target = screens.at(1);
    fInstance->setGeometry(target->geometry());
    fInstance->showFullScreen();
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if(QWindow *wh = fInstance->windowHandle()) {
        wh->setScreen(target);
        wh->setGeometry(target->geometry());
    }
#endif
    fInstance->showWelcome();
}

void CustomerDisplay::destroyInstance()
{
    delete fInstance;
    fInstance = nullptr;
}

CustomerDisplay::CustomerDisplay(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Customer display"));
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    setObjectName(QStringLiteral("CustomerDisplayRoot"));

    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    auto *bodyRow = new QHBoxLayout();
    bodyRow->setContentsMargins(32, 36, 32, 24);
    bodyRow->setSpacing(0);
    bodyRow->addStretch(1);

    fCenterColumn = new QWidget(this);
    fCenterColumn->setObjectName(QStringLiteral("cdCenter"));
    auto *centerLayout = new QVBoxLayout(fCenterColumn);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(20);

    fHeaderPanel = new QFrame(fCenterColumn);
    fHeaderPanel->setObjectName(QStringLiteral("cdHeaderPanel"));
    auto *headerLayout = new QVBoxLayout(fHeaderPanel);
    headerLayout->setContentsMargins(32, 28, 32, 28);
    headerLayout->setSpacing(10);

    fBrandLabel = new QLabel(tr("Your order"), fHeaderPanel);
    fBrandLabel->setObjectName(QStringLiteral("cdBrand"));
    headerLayout->addWidget(fBrandLabel);

    fContextLabel = new QLabel(fHeaderPanel);
    fContextLabel->setObjectName(QStringLiteral("cdContext"));
    fContextLabel->setWordWrap(true);
    headerLayout->addWidget(fContextLabel);

    centerLayout->addWidget(fHeaderPanel);

    fEmptyHint = new QLabel(tr("Items will appear here as your order is prepared."), fCenterColumn);
    fEmptyHint->setObjectName(QStringLiteral("cdEmpty"));
    fEmptyHint->setWordWrap(true);
    fEmptyHint->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(fEmptyHint, 1);

    fScroll = new QScrollArea(fCenterColumn);
    fScroll->setObjectName(QStringLiteral("cdScroll"));
    fScroll->setWidgetResizable(true);
    fScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fScroll->setFrameShape(QFrame::NoFrame);
    fScroll->hide();

    fItemsHost = new QWidget(fScroll);
    fItemsHost->setObjectName(QStringLiteral("cdItemsHost"));
    fItemsLayout = new QVBoxLayout(fItemsHost);
    fItemsLayout->setContentsMargins(4, 4, 4, 4);
    fItemsLayout->setSpacing(14);
    fItemsLayout->addStretch(1);
    fScroll->setWidget(fItemsHost);
    centerLayout->addWidget(fScroll, 1);

    bodyRow->addWidget(fCenterColumn, 0, Qt::AlignHCenter);
    bodyRow->addStretch(1);
    outer->addLayout(bodyRow, 1);

    fFooter = new QFrame(this);
    fFooter->setObjectName(QStringLiteral("cdFooter"));
    auto *footerOuter = new QHBoxLayout(fFooter);
    footerOuter->setContentsMargins(48, 0, 48, 0);
    footerOuter->addStretch(1);

    auto *footerInner = new QFrame(fFooter);
    footerInner->setObjectName(QStringLiteral("cdFooterInner"));
    auto *footerLayout = new QHBoxLayout(footerInner);
    footerLayout->setContentsMargins(40, 28, 40, 32);
    footerLayout->setSpacing(16);

    fTotalCaption = new QLabel(tr("Total"), footerInner);
    fTotalCaption->setObjectName(QStringLiteral("cdTotalCaption"));
    fTotalCaption->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    fTotalAmount = new QLabel(QStringLiteral("0"), footerInner);
    fTotalAmount->setObjectName(QStringLiteral("cdTotalAmount"));
    fTotalAmount->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    fTotalCurrency = new QLabel(QStringLiteral("AMD"), footerInner);
    fTotalCurrency->setObjectName(QStringLiteral("cdTotalCurrency"));
    fTotalCurrency->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    footerLayout->addStretch(1);
    footerLayout->addWidget(fTotalCaption, 0, Qt::AlignVCenter);
    footerLayout->addWidget(fTotalAmount, 0, Qt::AlignVCenter);
    footerLayout->addWidget(fTotalCurrency, 0, Qt::AlignBottom);

    footerOuter->addWidget(footerInner, 0, Qt::AlignHCenter);
    footerOuter->addStretch(1);

    outer->addWidget(fFooter);
    fFooter->hide();

    rebuildStyles();
}

void CustomerDisplay::rebuildStyles()
{
    const int w = qMax(width(), 640);
    const int centerW = qMin(920, qMax(520, int(w * 0.52)));
    fCenterColumn->setFixedWidth(centerW);

    const int footerW = qMin(920, qMax(520, int(w * 0.52)));
    if(QFrame *inner = fFooter->findChild<QFrame *>(QStringLiteral("cdFooterInner"))) {
        inner->setFixedWidth(footerW);
    }

    const int brand = qMax(28, w / 38);
    const int context = qMax(17, w / 58);
    const int rowName = qMax(20, w / 48);
    const int rowQty = qMax(16, w / 62);
    const int rowPrice = qMax(20, w / 50);
    const int totalNum = qMax(42, w / 28);
    const int totalLabel = qMax(16, w / 70);
    const int empty = qMax(20, w / 52);

    const QString fontStack = QStringLiteral("\"Segoe UI\", \"Helvetica Neue\", Arial, sans-serif");

    setStyleSheet(QString(
        "#CustomerDisplayRoot {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "    stop:0 #0a0e17, stop:0.45 #121827, stop:1 #0f1419);"
        "  font-family: %10;"
        "}"
        "QWidget#cdCenter { background: transparent; }"
        "QFrame#cdHeaderPanel {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #1e2638, stop:1 #171e2c);"
        "  border-radius: 24px;"
        "  border: 1px solid #2d384d;"
        "}"
        "QLabel#cdBrand {"
        "  color: #ffffff;"
        "  font-size: %1px;"
        "  font-weight: 600;"
        "  letter-spacing: 0.5px;"
        "  background: transparent;"
        "}"
        "QLabel#cdContext {"
        "  color: #9dabb9;"
        "  font-size: %2px;"
        "  font-weight: 500;"
        "  line-height: 1.35;"
        "  background: transparent;"
        "}"
        "QLabel#cdEmpty {"
        "  color: #6b7788;"
        "  font-size: %9px;"
        "  font-weight: 500;"
        "  padding: 64px 24px;"
        "  background: transparent;"
        "}"
        "QScrollArea#cdScroll { background: transparent; border: none; }"
        "QWidget#cdItemsHost { background: transparent; }"
        "QFrame#cdFooter {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 transparent, stop:0.35 #0a0e17);"
        "  border: none;"
        "  min-height: 120px;"
        "}"
        "QFrame#cdFooterInner {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #1a2436, stop:1 #1e2d42);"
        "  border-radius: 28px;"
        "  border: 1px solid #3d4f68;"
        "}"
        "QLabel#cdTotalCaption {"
        "  color: #8fa0b5;"
        "  font-size: %8px;"
        "  font-weight: 600;"
        "  text-transform: uppercase;"
        "  letter-spacing: 2px;"
        "  padding-right: 12px;"
        "  background: transparent;"
        "}"
        "QLabel#cdTotalAmount {"
        "  color: #ffffff;"
        "  font-size: %7px;"
        "  font-weight: 700;"
        "  background: transparent;"
        "}"
        "QLabel#cdTotalCurrency {"
        "  color: #5ec4ff;"
        "  font-size: %8px;"
        "  font-weight: 600;"
        "  padding-bottom: 6px;"
        "  padding-left: 4px;"
        "  background: transparent;"
        "}"
        "QFrame#cdRowFrame {"
        "  background: #151c28;"
        "  border-radius: 18px;"
        "  border: 1px solid #252f42;"
        "  border-left: 4px solid #3d9cf5;"
        "}"
        "QLabel#cdRowQtyBadge {"
        "  background: #243044;"
        "  color: #e8eef5;"
        "  font-size: %3px;"
        "  font-weight: 700;"
        "  border-radius: 26px;"
        "  min-width: 52px;"
        "  max-width: 52px;"
        "  min-height: 52px;"
        "  max-height: 52px;"
        "}"
        "QLabel#cdRowName {"
        "  color: #f2f5f9;"
        "  font-size: %4px;"
        "  font-weight: 600;"
        "  background: transparent;"
        "}"
        "QLabel#cdRowTotal {"
        "  color: #7ec8ff;"
        "  font-size: %5px;"
        "  font-weight: 700;"
        "  background: transparent;"
        "}"
        "QLabel#cdRowComment {"
        "  color: #7a8799;"
        "  font-size: %6px;"
        "  font-weight: 500;"
        "  font-style: normal;"
        "  padding-left: 68px;"
        "  background: transparent;"
        "}"
    ).arg(brand).arg(context).arg(rowQty).arg(rowName).arg(rowPrice)
     .arg(qMax(14, rowName - 4)).arg(totalNum).arg(totalLabel).arg(empty).arg(fontStack));

    fScroll->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical { width: 6px; background: transparent; margin: 8px 2px; }"
        "QScrollBar::handle:vertical { background: #3a4a62; border-radius: 3px; min-height: 48px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }");
}

void CustomerDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rebuildStyles();
}

void CustomerDisplay::clearItems()
{
    while(QLayoutItem *item = fItemsLayout->takeAt(0)) {
        if(QWidget *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }
    fItemsLayout->addStretch(1);
}

void CustomerDisplay::addItemRow(const CustomerDisplayLine &line)
{
    if(fItemsLayout->count() > 0) {
        QLayoutItem *stretch = fItemsLayout->takeAt(fItemsLayout->count() - 1);
        delete stretch;
    }

    auto *frame = new QFrame(fItemsHost);
    frame->setObjectName(QStringLiteral("cdRowFrame"));
    frame->setFrameShape(QFrame::NoFrame);

    auto *col = new QVBoxLayout(frame);
    col->setContentsMargins(22, 18, 24, 18);
    col->setSpacing(8);

    auto *row = new QHBoxLayout();
    row->setSpacing(20);
    row->setContentsMargins(0, 0, 0, 0);

    auto *qty = new QLabel(line.qty, frame);
    qty->setObjectName(QStringLiteral("cdRowQtyBadge"));
    qty->setAlignment(Qt::AlignCenter);

    auto *name = new QLabel(line.name, frame);
    name->setObjectName(QStringLiteral("cdRowName"));
    name->setWordWrap(true);
    name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto *total = new QLabel(line.total, frame);
    total->setObjectName(QStringLiteral("cdRowTotal"));
    total->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    total->setMinimumWidth(100);

    row->addWidget(qty, 0, Qt::AlignVCenter);
    row->addWidget(name, 1, Qt::AlignVCenter);
    row->addWidget(total, 0, Qt::AlignVCenter);
    col->addLayout(row);

    if(!line.comment.isEmpty()) {
        auto *comment = new QLabel(line.comment, frame);
        comment->setObjectName(QStringLiteral("cdRowComment"));
        comment->setWordWrap(true);
        col->addWidget(comment);
    }

    fItemsLayout->addWidget(frame);
    fItemsLayout->addStretch(1);
}

void CustomerDisplay::showWelcome()
{
    fBrandLabel->setText(tr("Welcome"));
    fContextLabel->setText(tr("Thank you for visiting us"));
    fContextLabel->show();
    fHeaderPanel->show();
    fEmptyHint->show();
    fScroll->hide();
    fFooter->hide();
    clearItems();
}

void CustomerDisplay::showOrder(const CustomerDisplayOrder &order)
{
    fBrandLabel->setText(tr("Your order"));
    fContextLabel->hide();

    clearItems();
    for(const CustomerDisplayLine &line : order.lines) {
        addItemRow(line);
    }

    if(order.lines.isEmpty()) {
        fEmptyHint->setText(tr("Your order is empty."));
        fEmptyHint->show();
        fScroll->hide();
        fFooter->hide();
        return;
    }

    fEmptyHint->hide();
    fScroll->show();
    fFooter->show();
    fTotalAmount->setText(order.grandTotal);

    QTimer::singleShot(0, this, [this]() {
        if(fScroll && fScroll->verticalScrollBar()) {
            fScroll->verticalScrollBar()->setValue(fScroll->verticalScrollBar()->maximum());
        }
    });
}
