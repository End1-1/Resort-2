/********************************************************************************
** Form generated from reading UI file 'rtools.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTOOLS_H
#define UI_RTOOLS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
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
    QToolButton *btnSalary;
    QToolButton *btnOrderComment;
    QToolButton *btnCardStat;
    QToolButton *btnPrintDailySales;
    QToolButton *btnCloseDay;
    QToolButton *btnTax;
    QToolButton *btnVisitStat;
    QToolButton *btnRemoveOrder;
    QToolButton *btnComplex;
    QToolButton *btnCompimentary;
    QToolButton *btnVoidBack;
    QToolButton *btnCheckCardReg;
    QToolButton *btnShowHideRemovedItems;
    QToolButton *btnPrintVoid;
    QToolButton *btnCheckCard;
    QToolButton *btnChangePassword;
    QToolButton *btnPrintAnyReport;
    QToolButton *btnMoveTable;
    QToolButton *btnSalary_2;
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
            RTools->setObjectName(QString::fromUtf8("RTools"));
        RTools->resize(520, 536);
        verticalLayout = new QVBoxLayout(RTools);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(RTools);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btnSalary = new QToolButton(frame);
        btnSalary->setObjectName(QString::fromUtf8("btnSalary"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/invoice.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSalary->setIcon(icon);
        btnSalary->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnSalary, 4, 0, 1, 1);

        btnOrderComment = new QToolButton(frame);
        btnOrderComment->setObjectName(QString::fromUtf8("btnOrderComment"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/mark.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOrderComment->setIcon(icon1);
        btnOrderComment->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnOrderComment, 0, 1, 1, 1);

        btnCardStat = new QToolButton(frame);
        btnCardStat->setObjectName(QString::fromUtf8("btnCardStat"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/guests.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCardStat->setIcon(icon2);
        btnCardStat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCardStat, 4, 1, 1, 1);

        btnPrintDailySales = new QToolButton(frame);
        btnPrintDailySales->setObjectName(QString::fromUtf8("btnPrintDailySales"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/list.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintDailySales->setIcon(icon3);
        btnPrintDailySales->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnPrintDailySales, 6, 1, 1, 1);

        btnCloseDay = new QToolButton(frame);
        btnCloseDay->setObjectName(QString::fromUtf8("btnCloseDay"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/money_calculator.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCloseDay->setIcon(icon4);
        btnCloseDay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCloseDay, 2, 2, 1, 1);

        btnTax = new QToolButton(frame);
        btnTax->setObjectName(QString::fromUtf8("btnTax"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/tax.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTax->setIcon(icon5);
        btnTax->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnTax, 2, 0, 1, 1);

        btnVisitStat = new QToolButton(frame);
        btnVisitStat->setObjectName(QString::fromUtf8("btnVisitStat"));
        btnVisitStat->setIcon(icon2);
        btnVisitStat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnVisitStat, 4, 2, 1, 1);

        btnRemoveOrder = new QToolButton(frame);
        btnRemoveOrder->setObjectName(QString::fromUtf8("btnRemoveOrder"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/garbage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRemoveOrder->setIcon(icon6);
        btnRemoveOrder->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnRemoveOrder, 1, 1, 1, 1);

        btnComplex = new QToolButton(frame);
        btnComplex->setObjectName(QString::fromUtf8("btnComplex"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/dinner.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnComplex->setIcon(icon7);
        btnComplex->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnComplex, 0, 2, 1, 1);

        btnCompimentary = new QToolButton(frame);
        btnCompimentary->setObjectName(QString::fromUtf8("btnCompimentary"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/gift.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCompimentary->setIcon(icon8);
        btnCompimentary->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCompimentary, 1, 2, 1, 1);

        btnVoidBack = new QToolButton(frame);
        btnVoidBack->setObjectName(QString::fromUtf8("btnVoidBack"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/recycle.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnVoidBack->setIcon(icon9);
        btnVoidBack->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnVoidBack, 3, 1, 1, 1);

        btnCheckCardReg = new QToolButton(frame);
        btnCheckCardReg->setObjectName(QString::fromUtf8("btnCheckCardReg"));
        btnCheckCardReg->setIcon(icon4);
        btnCheckCardReg->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCheckCardReg, 6, 0, 1, 1);

        btnShowHideRemovedItems = new QToolButton(frame);
        btnShowHideRemovedItems->setObjectName(QString::fromUtf8("btnShowHideRemovedItems"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/basket.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnShowHideRemovedItems->setIcon(icon10);
        btnShowHideRemovedItems->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnShowHideRemovedItems, 0, 0, 1, 1);

        btnPrintVoid = new QToolButton(frame);
        btnPrintVoid->setObjectName(QString::fromUtf8("btnPrintVoid"));
        btnPrintVoid->setIcon(icon9);
        btnPrintVoid->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnPrintVoid, 3, 2, 1, 1);

        btnCheckCard = new QToolButton(frame);
        btnCheckCard->setObjectName(QString::fromUtf8("btnCheckCard"));
        btnCheckCard->setIcon(icon4);
        btnCheckCard->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnCheckCard, 6, 2, 1, 1);

        btnChangePassword = new QToolButton(frame);
        btnChangePassword->setObjectName(QString::fromUtf8("btnChangePassword"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/lock.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnChangePassword->setIcon(icon11);
        btnChangePassword->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnChangePassword, 2, 1, 1, 1);

        btnPrintAnyReport = new QToolButton(frame);
        btnPrintAnyReport->setObjectName(QString::fromUtf8("btnPrintAnyReport"));
        btnPrintAnyReport->setIcon(icon);
        btnPrintAnyReport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnPrintAnyReport, 3, 0, 1, 1);

        btnMoveTable = new QToolButton(frame);
        btnMoveTable->setObjectName(QString::fromUtf8("btnMoveTable"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/transfer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMoveTable->setIcon(icon12);
        btnMoveTable->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnMoveTable, 1, 0, 1, 1);

        btnSalary_2 = new QToolButton(frame);
        btnSalary_2->setObjectName(QString::fromUtf8("btnSalary_2"));
        btnSalary_2->setIcon(icon);
        btnSalary_2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(btnSalary_2, 5, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(RTools);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Sunken);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        btnExit = new QToolButton(frame_2);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon13);
        btnExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnExit, 1, 2, 1, 1);

        btnTotalReport = new QToolButton(frame_2);
        btnTotalReport->setObjectName(QString::fromUtf8("btnTotalReport"));
        btnTotalReport->setIcon(icon4);
        btnTotalReport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalReport, 0, 0, 1, 1);

        btnTotalYesterday = new QToolButton(frame_2);
        btnTotalYesterday->setObjectName(QString::fromUtf8("btnTotalYesterday"));
        btnTotalYesterday->setIcon(icon4);
        btnTotalYesterday->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalYesterday, 0, 1, 1, 1);

        btnShowNyTotal = new QToolButton(frame_2);
        btnShowNyTotal->setObjectName(QString::fromUtf8("btnShowNyTotal"));
        btnShowNyTotal->setIcon(icon4);
        btnShowNyTotal->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnShowNyTotal, 1, 1, 1, 1);

        btnTotalReportAnyDay = new QToolButton(frame_2);
        btnTotalReportAnyDay->setObjectName(QString::fromUtf8("btnTotalReportAnyDay"));
        btnTotalReportAnyDay->setIcon(icon4);
        btnTotalReportAnyDay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalReportAnyDay, 1, 0, 1, 1);

        btnTotalShort = new QToolButton(frame_2);
        btnTotalShort->setObjectName(QString::fromUtf8("btnTotalShort"));
        btnTotalShort->setIcon(icon4);
        btnTotalShort->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(btnTotalShort, 0, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);


        verticalLayout->addWidget(frame_2);


        retranslateUi(RTools);

        QMetaObject::connectSlotsByName(RTools);
    } // setupUi

    void retranslateUi(QDialog *RTools)
    {
        RTools->setWindowTitle(QCoreApplication::translate("RTools", "Tools", nullptr));
        label_2->setText(QCoreApplication::translate("RTools", "Actions", nullptr));
        btnSalary->setText(QCoreApplication::translate("RTools", "Salary", nullptr));
        btnOrderComment->setText(QCoreApplication::translate("RTools", "Order\n"
"comment", nullptr));
        btnCardStat->setText(QCoreApplication::translate("RTools", "Cards\n"
"statistics", nullptr));
        btnPrintDailySales->setText(QCoreApplication::translate("RTools", "Saled item", nullptr));
        btnCloseDay->setText(QCoreApplication::translate("RTools", "End of day", nullptr));
        btnTax->setText(QCoreApplication::translate("RTools", "Print tax", nullptr));
        btnVisitStat->setText(QCoreApplication::translate("RTools", "Visits\n"
"statistics", nullptr));
        btnRemoveOrder->setText(QCoreApplication::translate("RTools", "Remove\n"
"order", nullptr));
        btnComplex->setText(QCoreApplication::translate("RTools", "Add complex", nullptr));
        btnCompimentary->setText(QCoreApplication::translate("RTools", "Complimentary", nullptr));
        btnVoidBack->setText(QCoreApplication::translate("RTools", "Void Back", nullptr));
        btnCheckCardReg->setText(QCoreApplication::translate("RTools", "Check cards\n"
"registration", nullptr));
        btnShowHideRemovedItems->setText(QCoreApplication::translate("RTools", "Show / hide\n"
"removed items", nullptr));
        btnPrintVoid->setText(QCoreApplication::translate("RTools", "Print void\n"
"report", nullptr));
        btnCheckCard->setText(QCoreApplication::translate("RTools", "Card\n"
"amount", nullptr));
        btnChangePassword->setText(QCoreApplication::translate("RTools", "Change my\n"
"password", nullptr));
        btnPrintAnyReport->setText(QCoreApplication::translate("RTools", "Print receipt\n"
"by number", nullptr));
        btnMoveTable->setText(QCoreApplication::translate("RTools", "Move\n"
"table", nullptr));
        btnSalary_2->setText(QCoreApplication::translate("RTools", "Employes of day", nullptr));
        label->setText(QCoreApplication::translate("RTools", "Reports", nullptr));
        btnExit->setText(QCoreApplication::translate("RTools", "Exit", nullptr));
        btnTotalReport->setText(QCoreApplication::translate("RTools", "Total, Today", nullptr));
        btnTotalYesterday->setText(QCoreApplication::translate("RTools", "Total, Yesterday", nullptr));
        btnShowNyTotal->setText(QCoreApplication::translate("RTools", "Show my total", nullptr));
        btnTotalReportAnyDay->setText(QCoreApplication::translate("RTools", "Total, Any day", nullptr));
        btnTotalShort->setText(QCoreApplication::translate("RTools", "Total, Short", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RTools: public Ui_RTools {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTOOLS_H
