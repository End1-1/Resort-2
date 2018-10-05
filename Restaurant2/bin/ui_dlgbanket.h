/********************************************************************************
** Form generated from reading UI file 'dlgbanket.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGBANKET_H
#define UI_DLGBANKET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
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

class Ui_DlgBanket
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QWidget *widget_8;
    QVBoxLayout *verticalLayout_2;
    QLabel *lbState;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    EQLineEdit *leService;
    QPushButton *btnSelectService;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *leComment;
    QPushButton *btnComment;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    EQLineEdit *leGuests;
    QPushButton *btnGuestQty;
    QFrame *line;
    QLabel *label_8;
    EQLineEdit *lePrepayment;
    QPushButton *btnPrepayment;
    QFrame *line_2;
    QLabel *label_3;
    EQLineEdit *lePrice;
    QPushButton *btnPrice;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    EQLineEdit *leModeOfPayment;
    QPushButton *btnPayment;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    EQLineEdit *lePaymentComment;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    EQLineEdit *leHall;
    QPushButton *btnHall;
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *btnNew;
    QToolButton *btnDraft;
    QToolButton *btnOpen;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *btnSave;
    QToolButton *btnTaxPrint;
    QToolButton *btnPrintReceipt;
    QToolButton *btnExit;

    void setupUi(QDialog *DlgBanket)
    {
        if (DlgBanket->objectName().isEmpty())
            DlgBanket->setObjectName(QStringLiteral("DlgBanket"));
        DlgBanket->resize(968, 593);
        horizontalLayout = new QHBoxLayout(DlgBanket);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(DlgBanket);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_8 = new QWidget(frame);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        verticalLayout_2 = new QVBoxLayout(widget_8);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lbState = new QLabel(widget_8);
        lbState->setObjectName(QStringLiteral("lbState"));
        lbState->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lbState);


        verticalLayout->addWidget(widget_8);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(200, 0));

        horizontalLayout_2->addWidget(label);

        leService = new EQLineEdit(widget);
        leService->setObjectName(QStringLiteral("leService"));
        leService->setReadOnly(true);

        horizontalLayout_2->addWidget(leService);

        btnSelectService = new QPushButton(widget);
        btnSelectService->setObjectName(QStringLiteral("btnSelectService"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/banket.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSelectService->setIcon(icon);

        horizontalLayout_2->addWidget(btnSelectService);


        verticalLayout->addWidget(widget);

        widget_7 = new QWidget(frame);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        horizontalLayout_8 = new QHBoxLayout(widget_7);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(widget_7);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(200, 0));

        horizontalLayout_8->addWidget(label_7);

        leComment = new QLineEdit(widget_7);
        leComment->setObjectName(QStringLiteral("leComment"));
        leComment->setReadOnly(true);

        horizontalLayout_8->addWidget(leComment);

        btnComment = new QPushButton(widget_7);
        btnComment->setObjectName(QStringLiteral("btnComment"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/mark.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnComment->setIcon(icon1);

        horizontalLayout_8->addWidget(btnComment);


        verticalLayout->addWidget(widget_7);

        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(200, 0));

        horizontalLayout_3->addWidget(label_2);

        leGuests = new EQLineEdit(widget_2);
        leGuests->setObjectName(QStringLiteral("leGuests"));
        leGuests->setReadOnly(true);

        horizontalLayout_3->addWidget(leGuests);

        btnGuestQty = new QPushButton(widget_2);
        btnGuestQty->setObjectName(QStringLiteral("btnGuestQty"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/guests.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnGuestQty->setIcon(icon2);

        horizontalLayout_3->addWidget(btnGuestQty);

        line = new QFrame(widget_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(line);

        label_8 = new QLabel(widget_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMinimumSize(QSize(100, 0));

        horizontalLayout_3->addWidget(label_8);

        lePrepayment = new EQLineEdit(widget_2);
        lePrepayment->setObjectName(QStringLiteral("lePrepayment"));

        horizontalLayout_3->addWidget(lePrepayment);

        btnPrepayment = new QPushButton(widget_2);
        btnPrepayment->setObjectName(QStringLiteral("btnPrepayment"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrepayment->setIcon(icon3);

        horizontalLayout_3->addWidget(btnPrepayment);

        line_2 = new QFrame(widget_2);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(line_2);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(100, 0));

        horizontalLayout_3->addWidget(label_3);

        lePrice = new EQLineEdit(widget_2);
        lePrice->setObjectName(QStringLiteral("lePrice"));
        lePrice->setReadOnly(true);

        horizontalLayout_3->addWidget(lePrice);

        btnPrice = new QPushButton(widget_2);
        btnPrice->setObjectName(QStringLiteral("btnPrice"));
        btnPrice->setIcon(icon3);

        horizontalLayout_3->addWidget(btnPrice);


        verticalLayout->addWidget(widget_2);

        widget_4 = new QWidget(frame);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_6 = new QHBoxLayout(widget_4);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_4 = new QLabel(widget_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(200, 0));

        horizontalLayout_6->addWidget(label_4);

        leModeOfPayment = new EQLineEdit(widget_4);
        leModeOfPayment->setObjectName(QStringLiteral("leModeOfPayment"));
        leModeOfPayment->setReadOnly(true);

        horizontalLayout_6->addWidget(leModeOfPayment);

        btnPayment = new QPushButton(widget_4);
        btnPayment->setObjectName(QStringLiteral("btnPayment"));
        btnPayment->setMinimumSize(QSize(0, 0));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/wallet-with-money.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPayment->setIcon(icon4);

        horizontalLayout_6->addWidget(btnPayment);


        verticalLayout->addWidget(widget_4);

        widget_5 = new QWidget(frame);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        horizontalLayout_5 = new QHBoxLayout(widget_5);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(widget_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(200, 0));

        horizontalLayout_5->addWidget(label_5);

        lePaymentComment = new EQLineEdit(widget_5);
        lePaymentComment->setObjectName(QStringLiteral("lePaymentComment"));

        horizontalLayout_5->addWidget(lePaymentComment);


        verticalLayout->addWidget(widget_5);

        widget_6 = new QWidget(frame);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        horizontalLayout_7 = new QHBoxLayout(widget_6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_6 = new QLabel(widget_6);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(200, 0));
        label_6->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_7->addWidget(label_6);

        leHall = new EQLineEdit(widget_6);
        leHall->setObjectName(QStringLiteral("leHall"));

        horizontalLayout_7->addWidget(leHall);

        btnHall = new QPushButton(widget_6);
        btnHall->setObjectName(QStringLiteral("btnHall"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/hall.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnHall->setIcon(icon5);

        horizontalLayout_7->addWidget(btnHall);


        verticalLayout->addWidget(widget_6);

        verticalSpacer = new QSpacerItem(20, 316, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        widget_3 = new QWidget(frame);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_4 = new QHBoxLayout(widget_3);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        btnNew = new QToolButton(widget_3);
        btnNew->setObjectName(QStringLiteral("btnNew"));
        btnNew->setMinimumSize(QSize(60, 0));
        btnNew->setMaximumSize(QSize(5555, 5555));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/revive.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnNew->setIcon(icon6);
        btnNew->setIconSize(QSize(32, 32));
        btnNew->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnNew);

        btnDraft = new QToolButton(widget_3);
        btnDraft->setObjectName(QStringLiteral("btnDraft"));
        btnDraft->setMinimumSize(QSize(60, 0));
        btnDraft->setMaximumSize(QSize(5555, 5555));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/draft.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDraft->setIcon(icon7);
        btnDraft->setIconSize(QSize(32, 32));
        btnDraft->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnDraft);

        btnOpen = new QToolButton(widget_3);
        btnOpen->setObjectName(QStringLiteral("btnOpen"));
        btnOpen->setMinimumSize(QSize(60, 0));
        btnOpen->setMaximumSize(QSize(5555, 5555));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOpen->setIcon(icon8);
        btnOpen->setIconSize(QSize(32, 32));
        btnOpen->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnOpen);

        horizontalSpacer_3 = new QSpacerItem(681, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        btnSave = new QToolButton(widget_3);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setMinimumSize(QSize(60, 0));
        btnSave->setMaximumSize(QSize(5555, 5555));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon9);
        btnSave->setIconSize(QSize(32, 32));
        btnSave->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnSave);

        btnTaxPrint = new QToolButton(widget_3);
        btnTaxPrint->setObjectName(QStringLiteral("btnTaxPrint"));
        btnTaxPrint->setMinimumSize(QSize(60, 0));
        btnTaxPrint->setMaximumSize(QSize(5555, 5555));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/tax.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTaxPrint->setIcon(icon10);
        btnTaxPrint->setIconSize(QSize(32, 32));
        btnTaxPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnTaxPrint);

        btnPrintReceipt = new QToolButton(widget_3);
        btnPrintReceipt->setObjectName(QStringLiteral("btnPrintReceipt"));
        btnPrintReceipt->setMinimumSize(QSize(60, 0));
        btnPrintReceipt->setMaximumSize(QSize(5555, 5555));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/invoice.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintReceipt->setIcon(icon11);
        btnPrintReceipt->setIconSize(QSize(32, 32));
        btnPrintReceipt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnPrintReceipt);

        btnExit = new QToolButton(widget_3);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setMinimumSize(QSize(60, 0));
        btnExit->setMaximumSize(QSize(5555, 5555));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon12);
        btnExit->setIconSize(QSize(32, 32));
        btnExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_4->addWidget(btnExit);


        verticalLayout->addWidget(widget_3);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgBanket);

        QMetaObject::connectSlotsByName(DlgBanket);
    } // setupUi

    void retranslateUi(QDialog *DlgBanket)
    {
        DlgBanket->setWindowTitle(QApplication::translate("DlgBanket", "Dialog", nullptr));
        lbState->setText(QApplication::translate("DlgBanket", "DRAFT", nullptr));
        label->setText(QApplication::translate("DlgBanket", "THE NAME OF SERVICE", nullptr));
        btnSelectService->setText(QString());
        label_7->setText(QApplication::translate("DlgBanket", "COMMENT", nullptr));
        btnComment->setText(QString());
        label_2->setText(QApplication::translate("DlgBanket", "GUEST COUNT", nullptr));
        btnGuestQty->setText(QString());
        label_8->setText(QApplication::translate("DlgBanket", "PREPAYMENT", nullptr));
        btnPrepayment->setText(QString());
        label_3->setText(QApplication::translate("DlgBanket", "TOTAL AMOUNT", nullptr));
        btnPrice->setText(QString());
        label_4->setText(QApplication::translate("DlgBanket", "MODE OF PAYMENT", nullptr));
        btnPayment->setText(QString());
        label_5->setText(QApplication::translate("DlgBanket", "PAYMENT COMMENT", nullptr));
        label_6->setText(QApplication::translate("DlgBanket", "PLACE OF EVENT", nullptr));
        btnHall->setText(QString());
        btnNew->setText(QApplication::translate("DlgBanket", "CREATE NEW", nullptr));
        btnDraft->setText(QApplication::translate("DlgBanket", "SAVE AS DRAFT", nullptr));
        btnOpen->setText(QApplication::translate("DlgBanket", "OPEN DRAFT", nullptr));
        btnSave->setText(QApplication::translate("DlgBanket", "SAVE", nullptr));
        btnTaxPrint->setText(QApplication::translate("DlgBanket", "PRINT FISCAL", nullptr));
        btnPrintReceipt->setText(QApplication::translate("DlgBanket", "RECEIPT", nullptr));
        btnExit->setText(QApplication::translate("DlgBanket", "EXIT", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgBanket: public Ui_DlgBanket {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGBANKET_H
