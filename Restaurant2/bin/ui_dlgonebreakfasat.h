/********************************************************************************
** Form generated from reading UI file 'dlgonebreakfasat.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGONEBREAKFASAT_H
#define UI_DLGONEBREAKFASAT_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <eqlineedit.h>

QT_BEGIN_NAMESPACE

class Ui_DlgOneBreakfasat
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *lbGuest;
    QPushButton *btnPayment;
    QLabel *label_5;
    QLabel *lbRoom;
    QPushButton *btnRoom;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *lbCardType;
    QLabel *label;
    EQLineEdit *lePayment;
    EQLineEdit *lePax;
    EQLineEdit *leInvoice;
    QSpacerItem *verticalSpacer;
    EQLineEdit *leTotal;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *btnOpenDraft;
    QToolButton *btnPrePrint;
    QToolButton *btnPrint;
    QToolButton *btnPrintTax;
    QToolButton *btnExit;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnPax;
    QPushButton *btnPrice;
    EQLineEdit *lePrice;
    QLabel *label_3;
    QLineEdit *leGuest;
    EQLineEdit *leRoom;
    QLabel *lbInvoice;
    QLabel *lbCityLedger;
    EQLineEdit *leCityLedger;
    EQLineEdit *leCardType;

    void setupUi(QDialog *DlgOneBreakfasat)
    {
        if (DlgOneBreakfasat->objectName().isEmpty())
            DlgOneBreakfasat->setObjectName(QStringLiteral("DlgOneBreakfasat"));
        DlgOneBreakfasat->resize(549, 508);
        verticalLayout = new QVBoxLayout(DlgOneBreakfasat);
        verticalLayout->setSpacing(24);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(DlgOneBreakfasat);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(24);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lbGuest = new QLabel(frame);
        lbGuest->setObjectName(QStringLiteral("lbGuest"));

        gridLayout->addWidget(lbGuest, 9, 0, 1, 1);

        btnPayment = new QPushButton(frame);
        btnPayment->setObjectName(QStringLiteral("btnPayment"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/wallet-with-money.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPayment->setIcon(icon);

        gridLayout->addWidget(btnPayment, 4, 6, 1, 2);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        lbRoom = new QLabel(frame);
        lbRoom->setObjectName(QStringLiteral("lbRoom"));

        gridLayout->addWidget(lbRoom, 8, 0, 1, 1);

        btnRoom = new QPushButton(frame);
        btnRoom->setObjectName(QStringLiteral("btnRoom"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/dinner.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRoom->setIcon(icon1);

        gridLayout->addWidget(btnRoom, 8, 6, 1, 2);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lbCardType = new QLabel(frame);
        lbCardType->setObjectName(QStringLiteral("lbCardType"));

        gridLayout->addWidget(lbCardType, 5, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 8);

        lePayment = new EQLineEdit(frame);
        lePayment->setObjectName(QStringLiteral("lePayment"));

        gridLayout->addWidget(lePayment, 4, 1, 1, 5);

        lePax = new EQLineEdit(frame);
        lePax->setObjectName(QStringLiteral("lePax"));

        gridLayout->addWidget(lePax, 1, 1, 1, 1);

        leInvoice = new EQLineEdit(frame);
        leInvoice->setObjectName(QStringLiteral("leInvoice"));

        gridLayout->addWidget(leInvoice, 8, 5, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 10, 4, 1, 1);

        leTotal = new EQLineEdit(frame);
        leTotal->setObjectName(QStringLiteral("leTotal"));

        gridLayout->addWidget(leTotal, 3, 1, 1, 6);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(75, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOpenDraft = new QToolButton(widget);
        btnOpenDraft->setObjectName(QStringLiteral("btnOpenDraft"));
        btnOpenDraft->setMinimumSize(QSize(60, 60));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOpenDraft->setIcon(icon2);
        btnOpenDraft->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnOpenDraft);

        btnPrePrint = new QToolButton(widget);
        btnPrePrint->setObjectName(QStringLiteral("btnPrePrint"));
        btnPrePrint->setMinimumSize(QSize(60, 60));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrePrint->setIcon(icon3);
        btnPrePrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnPrePrint);

        btnPrint = new QToolButton(widget);
        btnPrint->setObjectName(QStringLiteral("btnPrint"));
        btnPrint->setEnabled(false);
        btnPrint->setMinimumSize(QSize(60, 60));
        btnPrint->setIcon(icon3);
        btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnPrint);

        btnPrintTax = new QToolButton(widget);
        btnPrintTax->setObjectName(QStringLiteral("btnPrintTax"));
        btnPrintTax->setEnabled(false);
        btnPrintTax->setMinimumSize(QSize(60, 60));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/tax.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintTax->setIcon(icon4);
        btnPrintTax->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnPrintTax);

        btnExit = new QToolButton(widget);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setMinimumSize(QSize(60, 60));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon5);
        btnExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnExit);

        horizontalSpacer_2 = new QSpacerItem(74, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addWidget(widget, 11, 0, 1, 8);

        btnPax = new QPushButton(frame);
        btnPax->setObjectName(QStringLiteral("btnPax"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/guests.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPax->setIcon(icon6);

        gridLayout->addWidget(btnPax, 1, 2, 1, 1);

        btnPrice = new QPushButton(frame);
        btnPrice->setObjectName(QStringLiteral("btnPrice"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrice->setIcon(icon7);

        gridLayout->addWidget(btnPrice, 1, 6, 1, 1);

        lePrice = new EQLineEdit(frame);
        lePrice->setObjectName(QStringLiteral("lePrice"));

        gridLayout->addWidget(lePrice, 1, 5, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 4, 1, 1);

        leGuest = new QLineEdit(frame);
        leGuest->setObjectName(QStringLiteral("leGuest"));

        gridLayout->addWidget(leGuest, 9, 1, 1, 5);

        leRoom = new EQLineEdit(frame);
        leRoom->setObjectName(QStringLiteral("leRoom"));

        gridLayout->addWidget(leRoom, 8, 1, 1, 2);

        lbInvoice = new QLabel(frame);
        lbInvoice->setObjectName(QStringLiteral("lbInvoice"));

        gridLayout->addWidget(lbInvoice, 8, 3, 1, 2);

        lbCityLedger = new QLabel(frame);
        lbCityLedger->setObjectName(QStringLiteral("lbCityLedger"));

        gridLayout->addWidget(lbCityLedger, 6, 0, 1, 1);

        leCityLedger = new EQLineEdit(frame);
        leCityLedger->setObjectName(QStringLiteral("leCityLedger"));

        gridLayout->addWidget(leCityLedger, 6, 1, 1, 7);

        leCardType = new EQLineEdit(frame);
        leCardType->setObjectName(QStringLiteral("leCardType"));

        gridLayout->addWidget(leCardType, 5, 1, 1, 6);


        verticalLayout->addWidget(frame);


        retranslateUi(DlgOneBreakfasat);

        QMetaObject::connectSlotsByName(DlgOneBreakfasat);
    } // setupUi

    void retranslateUi(QDialog *DlgOneBreakfasat)
    {
        DlgOneBreakfasat->setWindowTitle(QApplication::translate("DlgOneBreakfasat", "Dialog", nullptr));
        lbGuest->setText(QApplication::translate("DlgOneBreakfasat", "GUEST", nullptr));
        btnPayment->setText(QString());
        label_5->setText(QApplication::translate("DlgOneBreakfasat", "PAYMENT MODE", nullptr));
        lbRoom->setText(QApplication::translate("DlgOneBreakfasat", "ROOM", nullptr));
        btnRoom->setText(QString());
        label_4->setText(QApplication::translate("DlgOneBreakfasat", "TOTAL", nullptr));
        label_2->setText(QApplication::translate("DlgOneBreakfasat", "PAX", nullptr));
        lbCardType->setText(QApplication::translate("DlgOneBreakfasat", "CARD TYPE", nullptr));
        label->setText(QApplication::translate("DlgOneBreakfasat", "BREAKFAST", nullptr));
        btnOpenDraft->setText(QApplication::translate("DlgOneBreakfasat", "Open\n"
"Draft", nullptr));
        btnPrePrint->setText(QApplication::translate("DlgOneBreakfasat", "Print\n"
"And Draft", nullptr));
        btnPrint->setText(QApplication::translate("DlgOneBreakfasat", "Print\n"
"And Save", nullptr));
        btnPrintTax->setText(QApplication::translate("DlgOneBreakfasat", "Print Tax", nullptr));
        btnExit->setText(QApplication::translate("DlgOneBreakfasat", "Exit", nullptr));
        btnPax->setText(QString());
        btnPrice->setText(QString());
        lePrice->setText(QApplication::translate("DlgOneBreakfasat", "6500", nullptr));
        label_3->setText(QApplication::translate("DlgOneBreakfasat", "PRICE", nullptr));
        lbInvoice->setText(QApplication::translate("DlgOneBreakfasat", "INVOICE", nullptr));
        lbCityLedger->setText(QApplication::translate("DlgOneBreakfasat", "CITY LEDGER", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgOneBreakfasat: public Ui_DlgOneBreakfasat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGONEBREAKFASAT_H
