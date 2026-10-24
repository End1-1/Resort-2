#include "customerdisplay.h"

#include <QFrame>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QWindow>

CustomerDisplay *CustomerDisplay::fInstance = nullptr;

QPixmap customerLogo(const QSize &target)
{
    const QImage source(QStringLiteral(":/images/logo.elite.png"));
    if(source.isNull() || target.width() < 2 || target.height() < 2) {
        return {};
    }
    return QPixmap::fromImage(source.convertToFormat(QImage::Format_RGB32)
                                  .scaled(target, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

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

    fLogoLabel = new QLabel(this);
    fLogoLabel->setObjectName(QStringLiteral("cdLogo"));
    fLogoLabel->setAlignment(Qt::AlignCenter);
    fLogoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outer->addWidget(fLogoLabel, 1);

    fOrderHeader = new QFrame(this);
    fOrderHeader->setObjectName(QStringLiteral("cdOrderHeader"));
    auto *headerBar = new QHBoxLayout(fOrderHeader);
    headerBar->setContentsMargins(16, 6, 24, 6);
    headerBar->setSpacing(16);

    fHeaderLogo = new QLabel(fOrderHeader);
    fHeaderLogo->setObjectName(QStringLiteral("cdHeaderLogo"));
    fHeaderLogo->setFixedHeight(50);
    fHeaderLogo->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    fHeaderLogo->setPixmap(customerLogo(QSize(400, 50)));

    fBranchLabel = new QLabel(fOrderHeader);
    fBranchLabel->setObjectName(QStringLiteral("cdBranch"));
    fBranchLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerBar->addWidget(fHeaderLogo, 0, Qt::AlignVCenter);
    headerBar->addWidget(fBranchLabel, 0, Qt::AlignVCenter);
    headerBar->addStretch(1);
    outer->addWidget(fOrderHeader);
    fOrderHeader->hide();

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
    footerOuter->setContentsMargins(0, 0, 0, 0);

    auto *footerInner = new QFrame(fFooter);
    footerInner->setObjectName(QStringLiteral("cdFooterInner"));
    auto *footerLayout = new QHBoxLayout(footerInner);
    footerLayout->setContentsMargins(40, 8, 40, 8);
    footerLayout->setSpacing(16);

    fServiceCaption = new QLabel(tr("Service"), footerInner);
    fServiceCaption->setObjectName(QStringLiteral("cdServiceCaption"));
    fServiceCaption->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    fServiceAmount = new QLabel(footerInner);
    fServiceAmount->setObjectName(QStringLiteral("cdServiceAmount"));
    fServiceAmount->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    auto *serviceCol = new QVBoxLayout();
    serviceCol->setSpacing(4);
    serviceCol->setContentsMargins(0, 0, 0, 0);
    serviceCol->addWidget(fServiceCaption);
    serviceCol->addWidget(fServiceAmount);

    fTotalCaption = new QLabel(tr("Total"), footerInner);
    fTotalCaption->setObjectName(QStringLiteral("cdTotalCaption"));
    fTotalCaption->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    fTotalAmount = new QLabel(QStringLiteral("0"), footerInner);
    fTotalAmount->setObjectName(QStringLiteral("cdTotalAmount"));
    fTotalAmount->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    fTotalCurrency = new QLabel(QStringLiteral("AMD"), footerInner);
    fTotalCurrency->setObjectName(QStringLiteral("cdTotalCurrency"));
    fTotalCurrency->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    auto *totalValue = new QHBoxLayout();
    totalValue->setSpacing(8);
    totalValue->setContentsMargins(0, 0, 0, 0);
    totalValue->addStretch(1);
    totalValue->addWidget(fTotalAmount, 0, Qt::AlignBottom);
    totalValue->addWidget(fTotalCurrency, 0, Qt::AlignBottom);

    auto *totalCol = new QVBoxLayout();
    totalCol->setSpacing(4);
    totalCol->setContentsMargins(0, 0, 0, 0);
    totalCol->addWidget(fTotalCaption);
    totalCol->addLayout(totalValue);

    footerLayout->addLayout(serviceCol, 0);
    footerLayout->addStretch(1);
    footerLayout->addLayout(totalCol, 0);

    footerOuter->addWidget(footerInner, 1);

    outer->addWidget(fFooter);
    fFooter->hide();

    rebuildStyles();
}

void CustomerDisplay::rebuildStyles()
{
    const int w = qMax(width(), 640);
    const int centerW = qMin(920, qMax(520, int(w * 0.52)));
    fCenterColumn->setFixedWidth(centerW);

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
        "  background: #111111;"
        "  font-family: %10;"
        "}"
        "QLabel#cdLogo { background: #111111; }"
        "QFrame#cdOrderHeader {"
        "  background: #111111;"
        "  border: none;"
        "  border-bottom: 1px solid #2a2a2a;"
        "}"
        "QLabel#cdHeaderLogo { background: transparent; }"
        "QLabel#cdBranch {"
        "  color: #e6c98a;"
        "  font-size: %8px;"
        "  font-weight: 600;"
        "  background: transparent;"
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
        "}"
        "QFrame#cdFooterInner {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #1a2436, stop:1 #1e2d42);"
        "  border-radius: 0;"
        "  border: none;"
        "  border-top: 1px solid #3d4f68;"
        "}"
        "QLabel#cdServiceCaption {"
        "  color: #c6a15b;"
        "  font-size: %8px;"
        "  font-weight: 600;"
        "  letter-spacing: 1px;"
        "  background: transparent;"
        "}"
        "QLabel#cdServiceAmount {"
        "  color: #e6c98a;"
        "  font-size: %7px;"
        "  font-weight: 700;"
        "  background: transparent;"
        "}"
        "QLabel#cdTotalCaption {"
        "  color: #8fa0b5;"
        "  font-size: %8px;"
        "  font-weight: 600;"
        "  text-transform: uppercase;"
        "  letter-spacing: 2px;"
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
    updateLogo();
}

void CustomerDisplay::updateLogo()
{
    if(!fLogoLabel || !fLogoLabel->isVisible()) {
        return;
    }

    fLogoLabel->setPixmap(customerLogo(fLogoLabel->size()));
}

void CustomerDisplay::setIdleLayout(bool idle)
{
    auto *outer = qobject_cast<QVBoxLayout *>(layout());
    if(!outer) {
        return;
    }

    for(int i = 0; i < outer->count(); ++i) {
        QLayout *sub = outer->itemAt(i)->layout();
        if(!sub) {
            continue;
        }
        outer->setStretch(i, idle ? 0 : 1);
        sub->setContentsMargins(idle ? 0 : 32, idle ? 0 : 36, idle ? 0 : 32, idle ? 0 : 24);
    }
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

void CustomerDisplay::setBranchName(const QString &name)
{
    if(fBranchLabel) {
        fBranchLabel->setText(name);
    }
}

void CustomerDisplay::showWelcome()
{
    clearItems();
    fOrderHeader->hide();
    fCenterColumn->hide();
    fFooter->hide();
    setIdleLayout(true);
    fLogoLabel->show();
    if(QLayout *lay = layout()) {
        lay->activate();
    }
    updateLogo();
}

void CustomerDisplay::showOrder(const CustomerDisplayOrder &order)
{
    if(order.lines.isEmpty()) {
        showWelcome();
        return;
    }

    fLogoLabel->hide();
    fLogoLabel->clear();
    setIdleLayout(false);
    fOrderHeader->show();
    fCenterColumn->show();
    fHeaderPanel->hide();
    fContextLabel->hide();

    clearItems();
    for(const CustomerDisplayLine &line : order.lines) {
        addItemRow(line);
    }

    fEmptyHint->hide();
    fScroll->show();
    fFooter->show();

    const bool hasService = !order.serviceAmount.isEmpty();
    fServiceCaption->setVisible(hasService);
    fServiceAmount->setVisible(hasService);
    fServiceAmount->setText(order.serviceAmount);
    fTotalAmount->setText(order.grandTotal);

    QTimer::singleShot(0, this, [this]() {
        if(fScroll && fScroll->verticalScrollBar()) {
            fScroll->verticalScrollBar()->setValue(fScroll->verticalScrollBar()->maximum());
        }
    });
}
