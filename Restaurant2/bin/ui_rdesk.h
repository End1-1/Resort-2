/********************************************************************************
** Form generated from reading UI file 'rdesk.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RDESK_H
#define UI_RDESK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RDesk
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *wInfo;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_6;
    QToolButton *btnHallWash;
    QToolButton *btnHallCafe;
    QLabel *lbStaff;
    QLabel *lbCar;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnExit_2;
    QPushButton *btnDiss50;
    QPushButton *btnExit;
    QFrame *frame;
    QGridLayout *gridLayout;
    QWidget *wLeft;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tblTables;
    QTableWidget *tblType;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnTypeUp;
    QPushButton *btnTypeDown;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnPackage;
    QPushButton *btnSetCar;
    QPushButton *btnTools;
    QWidget *wCenter;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tblDish;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnDishUp;
    QPushButton *btnDishDown;
    QSpacerItem *horizontalSpacer_3;
    QWidget *wRight;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QPushButton *btnTransfer;
    QPushButton *btnPrint;
    QPushButton *btn1;
    QPushButton *btn3;
    QPushButton *btnHalfPlush;
    QPushButton *btn7;
    QPushButton *btn6;
    QPushButton *btnPlusTen;
    QPushButton *btn9;
    QPushButton *btn4;
    QPushButton *btnHalfMinus;
    QPushButton *btnTrash;
    QPushButton *btn2;
    QPushButton *btn5;
    QPushButton *btn8;
    QTableWidget *tblOrder;
    QTableWidget *tblComplex;
    QTableWidget *tblTotal;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnOrdUp;
    QPushButton *btnOrdDown;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnPayment_2;
    QPushButton *btnPayment;

    void setupUi(QDialog *RDesk)
    {
        if (RDesk->objectName().isEmpty())
            RDesk->setObjectName(QStringLiteral("RDesk"));
        RDesk->resize(809, 567);
        verticalLayout = new QVBoxLayout(RDesk);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        wInfo = new QWidget(RDesk);
        wInfo->setObjectName(QStringLiteral("wInfo"));
        horizontalLayout_5 = new QHBoxLayout(wInfo);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(wInfo);

        widget_4 = new QWidget(RDesk);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_6 = new QHBoxLayout(widget_4);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        btnHallWash = new QToolButton(widget_4);
        btnHallWash->setObjectName(QStringLiteral("btnHallWash"));
        btnHallWash->setMinimumSize(QSize(200, 40));
        btnHallWash->setMaximumSize(QSize(200, 40));

        horizontalLayout_6->addWidget(btnHallWash);

        btnHallCafe = new QToolButton(widget_4);
        btnHallCafe->setObjectName(QStringLiteral("btnHallCafe"));
        btnHallCafe->setMinimumSize(QSize(200, 40));
        btnHallCafe->setMaximumSize(QSize(200, 40));

        horizontalLayout_6->addWidget(btnHallCafe);

        lbStaff = new QLabel(widget_4);
        lbStaff->setObjectName(QStringLiteral("lbStaff"));

        horizontalLayout_6->addWidget(lbStaff);

        lbCar = new QLabel(widget_4);
        lbCar->setObjectName(QStringLiteral("lbCar"));

        horizontalLayout_6->addWidget(lbCar);

        horizontalSpacer_5 = new QSpacerItem(688, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        btnExit_2 = new QPushButton(widget_4);
        btnExit_2->setObjectName(QStringLiteral("btnExit_2"));
        btnExit_2->setMinimumSize(QSize(80, 50));
        btnExit_2->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/discount.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit_2->setIcon(icon);
        btnExit_2->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(btnExit_2);

        btnDiss50 = new QPushButton(widget_4);
        btnDiss50->setObjectName(QStringLiteral("btnDiss50"));
        btnDiss50->setMinimumSize(QSize(80, 50));
        btnDiss50->setFocusPolicy(Qt::NoFocus);
        btnDiss50->setIcon(icon);
        btnDiss50->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(btnDiss50);

        btnExit = new QPushButton(widget_4);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setMinimumSize(QSize(80, 50));
        btnExit->setFocusPolicy(Qt::NoFocus);
        btnExit->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(btnExit);


        verticalLayout->addWidget(widget_4);

        frame = new QFrame(RDesk);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        wLeft = new QWidget(frame);
        wLeft->setObjectName(QStringLiteral("wLeft"));
        wLeft->setMinimumSize(QSize(330, 0));
        wLeft->setMaximumSize(QSize(330, 16777215));
        verticalLayout_2 = new QVBoxLayout(wLeft);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tblTables = new QTableWidget(wLeft);
        if (tblTables->columnCount() < 3)
            tblTables->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblTables->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblTables->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblTables->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tblTables->rowCount() < 3)
            tblTables->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblTables->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblTables->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblTables->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblTables->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblTables->setItem(0, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tblTables->setItem(0, 2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tblTables->setItem(1, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tblTables->setItem(1, 1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tblTables->setItem(1, 2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tblTables->setItem(2, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tblTables->setItem(2, 1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tblTables->setItem(2, 2, __qtablewidgetitem14);
        tblTables->setObjectName(QStringLiteral("tblTables"));
        tblTables->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTables->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTables->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblTables->setSelectionMode(QAbstractItemView::SingleSelection);
        tblTables->horizontalHeader()->setVisible(false);
        tblTables->verticalHeader()->setVisible(false);
        tblTables->verticalHeader()->setDefaultSectionSize(55);

        verticalLayout_2->addWidget(tblTables);

        tblType = new QTableWidget(wLeft);
        if (tblType->columnCount() < 2)
            tblType->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tblType->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tblType->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        tblType->setObjectName(QStringLiteral("tblType"));
        tblType->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblType->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblType->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblType->setSelectionMode(QAbstractItemView::NoSelection);
        tblType->horizontalHeader()->setVisible(false);
        tblType->verticalHeader()->setVisible(false);
        tblType->verticalHeader()->setDefaultSectionSize(50);

        verticalLayout_2->addWidget(tblType);

        widget_3 = new QWidget(wLeft);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMaximumSize(QSize(16777215, 55));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        btnTypeUp = new QPushButton(widget_3);
        btnTypeUp->setObjectName(QStringLiteral("btnTypeUp"));
        btnTypeUp->setMinimumSize(QSize(50, 50));
        btnTypeUp->setMaximumSize(QSize(50, 50));
        btnTypeUp->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTypeUp->setIcon(icon1);
        btnTypeUp->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnTypeUp);

        btnTypeDown = new QPushButton(widget_3);
        btnTypeDown->setObjectName(QStringLiteral("btnTypeDown"));
        btnTypeDown->setMinimumSize(QSize(50, 50));
        btnTypeDown->setMaximumSize(QSize(50, 50));
        btnTypeDown->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTypeDown->setIcon(icon2);
        btnTypeDown->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnTypeDown);

        horizontalSpacer_2 = new QSpacerItem(147, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnPackage = new QPushButton(widget_3);
        btnPackage->setObjectName(QStringLiteral("btnPackage"));
        btnPackage->setMinimumSize(QSize(50, 50));
        btnPackage->setMaximumSize(QSize(50, 50));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/box.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPackage->setIcon(icon3);
        btnPackage->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnPackage);

        btnSetCar = new QPushButton(widget_3);
        btnSetCar->setObjectName(QStringLiteral("btnSetCar"));
        btnSetCar->setMinimumSize(QSize(50, 50));
        btnSetCar->setMaximumSize(QSize(50, 50));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Resort/images/car.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSetCar->setIcon(icon4);
        btnSetCar->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnSetCar);

        btnTools = new QPushButton(widget_3);
        btnTools->setObjectName(QStringLiteral("btnTools"));
        btnTools->setMinimumSize(QSize(50, 50));
        btnTools->setMaximumSize(QSize(50, 50));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTools->setIcon(icon5);
        btnTools->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnTools);


        verticalLayout_2->addWidget(widget_3);


        gridLayout->addWidget(wLeft, 0, 0, 1, 1);

        wCenter = new QWidget(frame);
        wCenter->setObjectName(QStringLiteral("wCenter"));
        verticalLayout_3 = new QVBoxLayout(wCenter);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        tblDish = new QTableWidget(wCenter);
        tblDish->setObjectName(QStringLiteral("tblDish"));
        tblDish->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblDish->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblDish->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblDish->setSelectionMode(QAbstractItemView::SingleSelection);
        tblDish->horizontalHeader()->setVisible(false);
        tblDish->horizontalHeader()->setDefaultSectionSize(150);
        tblDish->verticalHeader()->setVisible(false);
        tblDish->verticalHeader()->setDefaultSectionSize(120);

        verticalLayout_3->addWidget(tblDish);

        widget_5 = new QWidget(wCenter);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        widget_5->setMaximumSize(QSize(16777215, 55));
        horizontalLayout_3 = new QHBoxLayout(widget_5);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        btnDishUp = new QPushButton(widget_5);
        btnDishUp->setObjectName(QStringLiteral("btnDishUp"));
        btnDishUp->setMinimumSize(QSize(50, 50));
        btnDishUp->setMaximumSize(QSize(50, 50));
        btnDishUp->setFocusPolicy(Qt::NoFocus);
        btnDishUp->setIcon(icon1);
        btnDishUp->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(btnDishUp);

        btnDishDown = new QPushButton(widget_5);
        btnDishDown->setObjectName(QStringLiteral("btnDishDown"));
        btnDishDown->setMinimumSize(QSize(50, 50));
        btnDishDown->setMaximumSize(QSize(50, 50));
        btnDishDown->setFocusPolicy(Qt::NoFocus);
        btnDishDown->setIcon(icon2);
        btnDishDown->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(btnDishDown);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addWidget(widget_5);


        gridLayout->addWidget(wCenter, 0, 1, 1, 1);

        wRight = new QWidget(frame);
        wRight->setObjectName(QStringLiteral("wRight"));
        wRight->setMinimumSize(QSize(0, 0));
        wRight->setMaximumSize(QSize(280, 16777215));
        verticalLayout_4 = new QVBoxLayout(wRight);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 2, 0);
        widget = new QWidget(wRight);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        btnTransfer = new QPushButton(widget);
        btnTransfer->setObjectName(QStringLiteral("btnTransfer"));
        btnTransfer->setMinimumSize(QSize(50, 50));
        btnTransfer->setMaximumSize(QSize(50, 50));
        btnTransfer->setFocusPolicy(Qt::NoFocus);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/transfer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTransfer->setIcon(icon6);
        btnTransfer->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(btnTransfer, 1, 4, 1, 1);

        btnPrint = new QPushButton(widget);
        btnPrint->setObjectName(QStringLiteral("btnPrint"));
        btnPrint->setMinimumSize(QSize(50, 50));
        btnPrint->setMaximumSize(QSize(50, 50));
        btnPrint->setFocusPolicy(Qt::NoFocus);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrint->setIcon(icon7);
        btnPrint->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(btnPrint, 2, 4, 1, 1);

        btn1 = new QPushButton(widget);
        btn1->setObjectName(QStringLiteral("btn1"));
        btn1->setMinimumSize(QSize(50, 50));
        btn1->setMaximumSize(QSize(50, 50));
        btn1->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn1, 0, 0, 1, 1);

        btn3 = new QPushButton(widget);
        btn3->setObjectName(QStringLiteral("btn3"));
        btn3->setMinimumSize(QSize(50, 50));
        btn3->setMaximumSize(QSize(50, 50));
        btn3->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn3, 0, 2, 1, 1);

        btnHalfPlush = new QPushButton(widget);
        btnHalfPlush->setObjectName(QStringLiteral("btnHalfPlush"));
        btnHalfPlush->setMinimumSize(QSize(50, 50));
        btnHalfPlush->setMaximumSize(QSize(50, 50));
        btnHalfPlush->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btnHalfPlush, 0, 3, 1, 1);

        btn7 = new QPushButton(widget);
        btn7->setObjectName(QStringLiteral("btn7"));
        btn7->setMinimumSize(QSize(50, 50));
        btn7->setMaximumSize(QSize(50, 50));
        btn7->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn7, 2, 0, 1, 1);

        btn6 = new QPushButton(widget);
        btn6->setObjectName(QStringLiteral("btn6"));
        btn6->setMinimumSize(QSize(50, 50));
        btn6->setMaximumSize(QSize(50, 50));
        btn6->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn6, 1, 2, 1, 1);

        btnPlusTen = new QPushButton(widget);
        btnPlusTen->setObjectName(QStringLiteral("btnPlusTen"));
        btnPlusTen->setMinimumSize(QSize(50, 50));
        btnPlusTen->setMaximumSize(QSize(50, 50));
        btnPlusTen->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btnPlusTen, 1, 3, 1, 1);

        btn9 = new QPushButton(widget);
        btn9->setObjectName(QStringLiteral("btn9"));
        btn9->setMinimumSize(QSize(50, 50));
        btn9->setMaximumSize(QSize(50, 50));
        btn9->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn9, 2, 2, 1, 1);

        btn4 = new QPushButton(widget);
        btn4->setObjectName(QStringLiteral("btn4"));
        btn4->setMinimumSize(QSize(50, 50));
        btn4->setMaximumSize(QSize(50, 50));
        btn4->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn4, 1, 0, 1, 1);

        btnHalfMinus = new QPushButton(widget);
        btnHalfMinus->setObjectName(QStringLiteral("btnHalfMinus"));
        btnHalfMinus->setMinimumSize(QSize(50, 50));
        btnHalfMinus->setMaximumSize(QSize(50, 50));
        btnHalfMinus->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btnHalfMinus, 0, 4, 1, 1);

        btnTrash = new QPushButton(widget);
        btnTrash->setObjectName(QStringLiteral("btnTrash"));
        btnTrash->setMinimumSize(QSize(50, 50));
        btnTrash->setMaximumSize(QSize(50, 50));
        btnTrash->setFocusPolicy(Qt::NoFocus);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/garbage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTrash->setIcon(icon8);
        btnTrash->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(btnTrash, 2, 3, 1, 1);

        btn2 = new QPushButton(widget);
        btn2->setObjectName(QStringLiteral("btn2"));
        btn2->setMinimumSize(QSize(50, 50));
        btn2->setMaximumSize(QSize(50, 50));
        btn2->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn2, 0, 1, 1, 1);

        btn5 = new QPushButton(widget);
        btn5->setObjectName(QStringLiteral("btn5"));
        btn5->setMinimumSize(QSize(50, 50));
        btn5->setMaximumSize(QSize(50, 50));
        btn5->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn5, 1, 1, 1, 1);

        btn8 = new QPushButton(widget);
        btn8->setObjectName(QStringLiteral("btn8"));
        btn8->setMinimumSize(QSize(50, 50));
        btn8->setMaximumSize(QSize(50, 50));
        btn8->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn8, 2, 1, 1, 1);


        verticalLayout_4->addWidget(widget);

        tblOrder = new QTableWidget(wRight);
        if (tblOrder->columnCount() < 3)
            tblOrder->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tblOrder->setHorizontalHeaderItem(0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tblOrder->setHorizontalHeaderItem(1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tblOrder->setHorizontalHeaderItem(2, __qtablewidgetitem19);
        tblOrder->setObjectName(QStringLiteral("tblOrder"));
        tblOrder->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblOrder->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblOrder->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblOrder->setSelectionMode(QAbstractItemView::SingleSelection);
        tblOrder->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblOrder->verticalHeader()->setVisible(false);
        tblOrder->verticalHeader()->setDefaultSectionSize(50);

        verticalLayout_4->addWidget(tblOrder);

        tblComplex = new QTableWidget(wRight);
        if (tblComplex->columnCount() < 4)
            tblComplex->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tblComplex->setHorizontalHeaderItem(0, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tblComplex->setHorizontalHeaderItem(1, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tblComplex->setHorizontalHeaderItem(2, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tblComplex->setHorizontalHeaderItem(3, __qtablewidgetitem23);
        tblComplex->setObjectName(QStringLiteral("tblComplex"));

        verticalLayout_4->addWidget(tblComplex);

        tblTotal = new QTableWidget(wRight);
        if (tblTotal->columnCount() < 2)
            tblTotal->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tblTotal->setHorizontalHeaderItem(0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tblTotal->setHorizontalHeaderItem(1, __qtablewidgetitem25);
        if (tblTotal->rowCount() < 2)
            tblTotal->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tblTotal->setVerticalHeaderItem(0, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tblTotal->setVerticalHeaderItem(1, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tblTotal->setItem(0, 0, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        tblTotal->setItem(0, 1, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        tblTotal->setItem(1, 0, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        tblTotal->setItem(1, 1, __qtablewidgetitem31);
        tblTotal->setObjectName(QStringLiteral("tblTotal"));
        tblTotal->setMaximumSize(QSize(16777215, 55));
        tblTotal->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTotal->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTotal->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblTotal->setSelectionMode(QAbstractItemView::NoSelection);
        tblTotal->horizontalHeader()->setVisible(false);
        tblTotal->verticalHeader()->setVisible(false);
        tblTotal->verticalHeader()->setDefaultSectionSize(25);

        verticalLayout_4->addWidget(tblTotal);

        widget_2 = new QWidget(wRight);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMaximumSize(QSize(16777215, 55));
        horizontalLayout_4 = new QHBoxLayout(widget_2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        btnOrdUp = new QPushButton(widget_2);
        btnOrdUp->setObjectName(QStringLiteral("btnOrdUp"));
        btnOrdUp->setMinimumSize(QSize(50, 50));
        btnOrdUp->setMaximumSize(QSize(50, 50));
        btnOrdUp->setFocusPolicy(Qt::NoFocus);
        btnOrdUp->setIcon(icon1);
        btnOrdUp->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(btnOrdUp);

        btnOrdDown = new QPushButton(widget_2);
        btnOrdDown->setObjectName(QStringLiteral("btnOrdDown"));
        btnOrdDown->setMinimumSize(QSize(50, 50));
        btnOrdDown->setMaximumSize(QSize(50, 50));
        btnOrdDown->setFocusPolicy(Qt::NoFocus);
        btnOrdDown->setIcon(icon2);
        btnOrdDown->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(btnOrdDown);

        horizontalSpacer_4 = new QSpacerItem(195, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        btnPayment_2 = new QPushButton(widget_2);
        btnPayment_2->setObjectName(QStringLiteral("btnPayment_2"));
        btnPayment_2->setMinimumSize(QSize(50, 50));
        btnPayment_2->setMaximumSize(QSize(50, 50));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/invoice.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPayment_2->setIcon(icon9);
        btnPayment_2->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(btnPayment_2);

        btnPayment = new QPushButton(widget_2);
        btnPayment->setObjectName(QStringLiteral("btnPayment"));
        btnPayment->setMinimumSize(QSize(50, 50));
        btnPayment->setMaximumSize(QSize(50, 50));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/wallet-with-money.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPayment->setIcon(icon10);
        btnPayment->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(btnPayment);


        verticalLayout_4->addWidget(widget_2);


        gridLayout->addWidget(wRight, 0, 2, 1, 1);


        verticalLayout->addWidget(frame);


        retranslateUi(RDesk);
        QObject::connect(btn1, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn2, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn3, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn4, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn5, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn6, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn7, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn8, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btn9, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btnHalfMinus, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btnHalfPlush, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btnTransfer, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));
        QObject::connect(btnPlusTen, SIGNAL(clicked()), RDesk, SLOT(onBtnQtyClicked()));

        QMetaObject::connectSlotsByName(RDesk);
    } // setupUi

    void retranslateUi(QDialog *RDesk)
    {
        RDesk->setWindowTitle(QApplication::translate("RDesk", "Desk", nullptr));
        btnHallWash->setText(QApplication::translate("RDesk", "WASH", nullptr));
        btnHallCafe->setText(QApplication::translate("RDesk", "CAFE", nullptr));
        lbStaff->setText(QApplication::translate("RDesk", "Staff", nullptr));
        lbCar->setText(QApplication::translate("RDesk", "Car", nullptr));
        btnExit_2->setText(QApplication::translate("RDesk", "-20%", nullptr));
        btnDiss50->setText(QApplication::translate("RDesk", "-50%", nullptr));
        btnExit->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tblTables->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("RDesk", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblTables->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("RDesk", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblTables->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("RDesk", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblTables->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("RDesk", "New Row", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblTables->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("RDesk", "New Row", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblTables->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("RDesk", "New Row", nullptr));

        const bool __sortingEnabled = tblTables->isSortingEnabled();
        tblTables->setSortingEnabled(false);
        tblTables->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem6 = tblType->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("RDesk", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblType->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("RDesk", "New Column", nullptr));
        btnTypeUp->setText(QString());
        btnTypeDown->setText(QString());
        btnPackage->setText(QString());
        btnSetCar->setText(QString());
        btnTools->setText(QString());
        btnDishUp->setText(QString());
        btnDishDown->setText(QString());
        btnTransfer->setText(QString());
        btnPrint->setText(QString());
        btn1->setText(QApplication::translate("RDesk", "1", nullptr));
        btn3->setText(QApplication::translate("RDesk", "3", nullptr));
        btnHalfPlush->setText(QApplication::translate("RDesk", "+0.5", nullptr));
        btn7->setText(QApplication::translate("RDesk", "7", nullptr));
        btn6->setText(QApplication::translate("RDesk", "6", nullptr));
        btnPlusTen->setText(QApplication::translate("RDesk", "+10", nullptr));
        btn9->setText(QApplication::translate("RDesk", "9", nullptr));
        btn4->setText(QApplication::translate("RDesk", "4", nullptr));
        btnHalfMinus->setText(QApplication::translate("RDesk", "-0.5", nullptr));
        btnTrash->setText(QString());
        btn2->setText(QApplication::translate("RDesk", "2", nullptr));
        btn5->setText(QApplication::translate("RDesk", "5", nullptr));
        btn8->setText(QApplication::translate("RDesk", "8", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblOrder->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QApplication::translate("RDesk", "Item name", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tblOrder->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QApplication::translate("RDesk", "Qty", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tblOrder->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QApplication::translate("RDesk", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tblComplex->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QApplication::translate("RDesk", "ItemCode", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tblComplex->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QApplication::translate("RDesk", "ItemName", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tblComplex->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QApplication::translate("RDesk", "Qty", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tblComplex->horizontalHeaderItem(3);
        ___qtablewidgetitem14->setText(QApplication::translate("RDesk", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tblTotal->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QApplication::translate("RDesk", "Key", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tblTotal->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QApplication::translate("RDesk", "Value", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tblTotal->verticalHeaderItem(0);
        ___qtablewidgetitem17->setText(QApplication::translate("RDesk", "New Row", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tblTotal->verticalHeaderItem(1);
        ___qtablewidgetitem18->setText(QApplication::translate("RDesk", "New Row", nullptr));

        const bool __sortingEnabled1 = tblTotal->isSortingEnabled();
        tblTotal->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem19 = tblTotal->item(0, 0);
        ___qtablewidgetitem19->setText(QApplication::translate("RDesk", "Table", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tblTotal->item(0, 1);
        ___qtablewidgetitem20->setText(QApplication::translate("RDesk", "Seghan1", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tblTotal->item(1, 0);
        ___qtablewidgetitem21->setText(QApplication::translate("RDesk", "Total", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tblTotal->item(1, 1);
        ___qtablewidgetitem22->setText(QApplication::translate("RDesk", "0", nullptr));
        tblTotal->setSortingEnabled(__sortingEnabled1);

        btnOrdUp->setText(QString());
        btnOrdDown->setText(QString());
        btnPayment_2->setText(QString());
        btnPayment->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RDesk: public Ui_RDesk {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RDESK_H
