/********************************************************************************
** Form generated from reading UI file 'rtools.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTOOLS_H
#define UI_RTOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RTools
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QGridLayout *gridLayout;
    QToolButton *btnMoveTable;
    QToolButton *btnChangePassword;
    QToolButton *btnOrderComment;
    QToolButton *btnCompimentary;
    QToolButton *btnTax;
    QToolButton *btnRemoveOrder;
    QToolButton *btnPrintAnyReport;
    QToolButton *btnComplex;
    QToolButton *btnShowHideRemovedItems;
    QToolButton *btnCloseDay;
    QToolButton *btnVoidBack;
    QToolButton *btnSalary;
    QToolButton *btnPrintVoid;
    QToolButton *btnCardStat;
    QToolButton *btnVisitStat;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QToolButton *btnExit;
    QToolButton *btnTotalReport;
    QToolButton *btnTotalYesterday;
    QToolButton *btnShowNyTotal;
    QToolButton *btnTotalReportAnyDay;
    QToolButton *btnTotalShort;

    void setupUi(QDialog *RTools)
    {
        if (RTools->objectName().isEmpty())
            RTools->setObjectName(QStringLiteral("RTools"));
        RTools->resize(520, 478);
        verticalLayout = new QVBoxLayout(RTools);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(RTools);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnMoveTable = new QToolButton(frame);
        btnMoveTable->setObjectName(QStringLiteral("btnMoveTable"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/transfer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMoveTable->setIcon(icon);
        btnMoveTable->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnMoveTable, 1, 0, 1, 1);

        btnChangePassword = new QToolButton(frame);
        btnChangePassword->setObjectName(QStringLiteral("btnChangePassword"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/lock.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnChangePassword->setIcon(icon1);
        btnChangePassword->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnChangePassword, 2, 1, 1, 1);

        btnOrderComment = new QToolButton(frame);
        btnOrderComment->setObjectName(QStringLiteral("btnOrderComment"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/mark.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOrderComment->setIcon(icon2);
        btnOrderComment->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnOrderComment, 0, 1, 1, 1);

        btnCompimentary = new QToolButton(frame);
        btnCompimentary->setObjectName(QStringLiteral("btnCompimentary"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/gift.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCompimentary->setIcon(icon3);
        btnCompimentary->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCompimentary, 1, 2, 1, 1);

        btnTax = new QToolButton(frame);
        btnTax->setObjectName(QStringLiteral("btnTax"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/tax.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTax->setIcon(icon4);
        btnTax->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnTax, 2, 0, 1, 1);

        btnRemoveOrder = new QToolButton(frame);
        btnRemoveOrder->setObjectName(QStringLiteral("btnRemoveOrder"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/garbage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRemoveOrder->setIcon(icon5);
        btnRemoveOrder->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnRemoveOrder, 1, 1, 1, 1);

        btnPrintAnyReport = new QToolButton(frame);
        btnPrintAnyReport->setObjectName(QStringLiteral("btnPrintAnyReport"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/invoice.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintAnyReport->setIcon(icon6);
        btnPrintAnyReport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnPrintAnyReport, 3, 0, 1, 1);

        btnComplex = new QToolButton(frame);
        btnComplex->setObjectName(QStringLiteral("btnComplex"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/dinner.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnComplex->setIcon(icon7);
        btnComplex->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnComplex, 0, 2, 1, 1);

        btnShowHideRemovedItems = new QToolButton(frame);
        btnShowHideRemovedItems->setObjectName(QStringLiteral("btnShowHideRemovedItems"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/basket.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnShowHideRemovedItems->setIcon(icon8);
        btnShowHideRemovedItems->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnShowHideRemovedItems, 0, 0, 1, 1);

        btnCloseDay = new QToolButton(frame);
        btnCloseDay->setObjectName(QStringLiteral("btnCloseDay"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/money_calculator.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCloseDay->setIcon(icon9);
        btnCloseDay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCloseDay, 2, 2, 1, 1);

        btnVoidBack = new QToolButton(frame);
        btnVoidBack->setObjectName(QStringLiteral("btnVoidBack"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/recycle.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnVoidBack->setIcon(icon10);
        btnVoidBack->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnVoidBack, 3, 1, 1, 1);

        btnSalary = new QToolButton(frame);
        btnSalary->setObjectName(QStringLiteral("btnSalary"));
        btnSalary->setIcon(icon6);
        btnSalary->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnSalary, 4, 0, 1, 1);

        btnPrintVoid = new QToolButton(frame);
        btnPrintVoid->setObjectName(QStringLiteral("btnPrintVoid"));
        btnPrintVoid->setIcon(icon10);
        btnPrintVoid->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnPrintVoid, 3, 2, 1, 1);

        btnCardStat = new QToolButton(frame);
        btnCardStat->setObjectName(QStringLiteral("btnCardStat"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/guests.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCardStat->setIcon(icon11);
        btnCardStat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCardStat, 4, 1, 1, 1);

        btnVisitStat = new QToolButton(frame);
        btnVisitStat->setObjectName(QStringLiteral("btnVisitStat"));
        btnVisitStat->setIcon(icon11);
        btnVisitStat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnVisitStat, 4, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(RTools);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Sunken);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label = new QLabel(frame_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        btnExit = new QToolButton(frame_2);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon12);
        btnExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnExit, 1, 2, 1, 1);

        btnTotalReport = new QToolButton(frame_2);
        btnTotalReport->setObjectName(QStringLiteral("btnTotalReport"));
        btnTotalReport->setIcon(icon9);
        btnTotalReport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalReport, 0, 0, 1, 1);

        btnTotalYesterday = new QToolButton(frame_2);
        btnTotalYesterday->setObjectName(QStringLiteral("btnTotalYesterday"));
        btnTotalYesterday->setIcon(icon9);
        btnTotalYesterday->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalYesterday, 0, 1, 1, 1);

        btnShowNyTotal = new QToolButton(frame_2);
        btnShowNyTotal->setObjectName(QStringLiteral("btnShowNyTotal"));
        btnShowNyTotal->setIcon(icon9);
        btnShowNyTotal->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnShowNyTotal, 1, 1, 1, 1);

        btnTotalReportAnyDay = new QToolButton(frame_2);
        btnTotalReportAnyDay->setObjectName(QStringLiteral("btnTotalReportAnyDay"));
        btnTotalReportAnyDay->setIcon(icon9);
        btnTotalReportAnyDay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalReportAnyDay, 1, 0, 1, 1);

        btnTotalShort = new QToolButton(frame_2);
        btnTotalShort->setObjectName(QStringLiteral("btnTotalShort"));
        btnTotalShort->setIcon(icon9);
        btnTotalShort->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalShort, 0, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);


        verticalLayout->addWidget(frame_2);


        retranslateUi(RTools);

        QMetaObject::connectSlotsByName(RTools);
    } // setupUi

    void retranslateUi(QDialog *RTools)
    {
        RTools->setWindowTitle(QApplication::translate("RTools", "Tools", nullptr));
        label_2->setText(QApplication::translate("RTools", "Actions", nullptr));
        btnMoveTable->setText(QApplication::translate("RTools", "Move\n"
"table", nullptr));
        btnChangePassword->setText(QApplication::translate("RTools", "Change my\n"
"password", nullptr));
        btnOrderComment->setText(QApplication::translate("RTools", "Order\n"
"comment", nullptr));
        btnCompimentary->setText(QApplication::translate("RTools", "Complimentary", nullptr));
        btnTax->setText(QApplication::translate("RTools", "Print tax", nullptr));
        btnRemoveOrder->setText(QApplication::translate("RTools", "Remove\n"
"order", nullptr));
        btnPrintAnyReport->setText(QApplication::translate("RTools", "Print receipt\n"
"by number", nullptr));
        btnComplex->setText(QApplication::translate("RTools", "Add complex", nullptr));
        btnShowHideRemovedItems->setText(QApplication::translate("RTools", "Show / hide\n"
"removed items", nullptr));
        btnCloseDay->setText(QApplication::translate("RTools", "End of day", nullptr));
        btnVoidBack->setText(QApplication::translate("RTools", "Void Back", nullptr));
        btnSalary->setText(QApplication::translate("RTools", "Salary", nullptr));
        btnPrintVoid->setText(QApplication::translate("RTools", "Print void\n"
"report", nullptr));
        btnCardStat->setText(QApplication::translate("RTools", "Cards\n"
"statistics", nullptr));
        btnVisitStat->setText(QApplication::translate("RTools", "Visits\n"
"statistics", nullptr));
        label->setText(QApplication::translate("RTools", "Reports", nullptr));
        btnExit->setText(QApplication::translate("RTools", "Exit", nullptr));
        btnTotalReport->setText(QApplication::translate("RTools", "Total, Today", nullptr));
        btnTotalYesterday->setText(QApplication::translate("RTools", "Total, Yesterday", nullptr));
        btnShowNyTotal->setText(QApplication::translate("RTools", "Show my total", nullptr));
        btnTotalReportAnyDay->setText(QApplication::translate("RTools", "Total, Any day", nullptr));
        btnTotalShort->setText(QApplication::translate("RTools", "Total, Short", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RTools: public Ui_RTools {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTOOLS_H
