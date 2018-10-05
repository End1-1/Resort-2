/********************************************************************************
** Form generated from reading UI file 'dlgpayment.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGPAYMENT_H
#define UI_DLGPAYMENT_H

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
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <eqlineedit.h>

QT_BEGIN_NAMESPACE

class Ui_DlgPayment
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    EQLineEdit *leDeptHolder;
    QToolButton *btnDeptHolder;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnPrintTax;
    EQLineEdit *leCouponNumber;
    EQLineEdit *leCard;
    QLabel *label_7;
    EQLineEdit *leFinalAmount;
    QLabel *label_4;
    QToolButton *btnCouponNumber;
    EQLineEdit *leCouponSerial;
    QToolButton *btnCard;
    EQLineEdit *leCash;
    QLabel *label_3;
    QLabel *label;
    EQLineEdit *leDiscount;
    QToolButton *btnDept;
    QLabel *label_2;
    QLabel *label_5;
    QToolButton *btnCash;
    QToolButton *btnCouponSerail;
    EQLineEdit *leDept;
    QLabel *label_6;
    QLabel *label_10;
    QLabel *label_8;
    EQLineEdit *leCouponAmount;
    EQLineEdit *leDiscountAmount;
    QLabel *label_11;
    EQLineEdit *leCardHolder;
    QLabel *lbVisit;

    void setupUi(QDialog *DlgPayment)
    {
        if (DlgPayment->objectName().isEmpty())
            DlgPayment->setObjectName(QStringLiteral("DlgPayment"));
        DlgPayment->resize(800, 439);
        horizontalLayout = new QHBoxLayout(DlgPayment);
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(DlgPayment);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(30);
        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, 0, -1);
        label_9 = new QLabel(widget_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        leDeptHolder = new EQLineEdit(widget_2);
        leDeptHolder->setObjectName(QStringLiteral("leDeptHolder"));
        leDeptHolder->setFocusPolicy(Qt::NoFocus);
        leDeptHolder->setReadOnly(true);

        horizontalLayout_3->addWidget(leDeptHolder);

        btnDeptHolder = new QToolButton(widget_2);
        btnDeptHolder->setObjectName(QStringLiteral("btnDeptHolder"));
        btnDeptHolder->setEnabled(false);
        btnDeptHolder->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(btnDeptHolder);


        gridLayout->addWidget(widget_2, 5, 0, 1, 7);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setMinimumSize(QSize(100, 50));
        btnOk->setFocusPolicy(Qt::ClickFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOk->setIcon(icon);
        btnOk->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setMinimumSize(QSize(100, 50));
        btnCancel->setFocusPolicy(Qt::ClickFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCancel->setIcon(icon1);
        btnCancel->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnPrintTax = new QPushButton(widget);
        btnPrintTax->setObjectName(QStringLiteral("btnPrintTax"));
        btnPrintTax->setMinimumSize(QSize(100, 50));
        btnPrintTax->setFocusPolicy(Qt::ClickFocus);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintTax->setIcon(icon2);
        btnPrintTax->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnPrintTax);


        gridLayout->addWidget(widget, 6, 0, 1, 7);

        leCouponNumber = new EQLineEdit(frame);
        leCouponNumber->setObjectName(QStringLiteral("leCouponNumber"));
        leCouponNumber->setFocusPolicy(Qt::NoFocus);
        leCouponNumber->setReadOnly(true);

        gridLayout->addWidget(leCouponNumber, 1, 5, 1, 1);

        leCard = new EQLineEdit(frame);
        leCard->setObjectName(QStringLiteral("leCard"));
        leCard->setFocusPolicy(Qt::NoFocus);
        leCard->setReadOnly(true);

        gridLayout->addWidget(leCard, 0, 5, 1, 1);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 4, 0, 1, 1);

        leFinalAmount = new EQLineEdit(frame);
        leFinalAmount->setObjectName(QStringLiteral("leFinalAmount"));
        leFinalAmount->setMaximumSize(QSize(150, 16777215));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        leFinalAmount->setFont(font);
        leFinalAmount->setFocusPolicy(Qt::NoFocus);
        leFinalAmount->setReadOnly(true);

        gridLayout->addWidget(leFinalAmount, 4, 1, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 4, 1, 1);

        btnCouponNumber = new QToolButton(frame);
        btnCouponNumber->setObjectName(QStringLiteral("btnCouponNumber"));
        btnCouponNumber->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCouponNumber, 1, 6, 1, 1);

        leCouponSerial = new EQLineEdit(frame);
        leCouponSerial->setObjectName(QStringLiteral("leCouponSerial"));
        leCouponSerial->setMaximumSize(QSize(150, 16777215));
        leCouponSerial->setFocusPolicy(Qt::NoFocus);
        leCouponSerial->setReadOnly(true);

        gridLayout->addWidget(leCouponSerial, 1, 1, 1, 1);

        btnCard = new QToolButton(frame);
        btnCard->setObjectName(QStringLiteral("btnCard"));
        btnCard->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCard, 0, 6, 1, 1);

        leCash = new EQLineEdit(frame);
        leCash->setObjectName(QStringLiteral("leCash"));
        leCash->setMaximumSize(QSize(150, 16777215));
        leCash->setFocusPolicy(Qt::NoFocus);
        leCash->setReadOnly(true);

        gridLayout->addWidget(leCash, 0, 1, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        leDiscount = new EQLineEdit(frame);
        leDiscount->setObjectName(QStringLiteral("leDiscount"));
        leDiscount->setMaximumSize(QSize(150, 16777215));
        leDiscount->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(leDiscount, 2, 1, 1, 1);

        btnDept = new QToolButton(frame);
        btnDept->setObjectName(QStringLiteral("btnDept"));
        btnDept->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnDept, 4, 6, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 4, 1, 1);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        btnCash = new QToolButton(frame);
        btnCash->setObjectName(QStringLiteral("btnCash"));
        btnCash->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCash, 0, 2, 1, 2);

        btnCouponSerail = new QToolButton(frame);
        btnCouponSerail->setObjectName(QStringLiteral("btnCouponSerail"));
        btnCouponSerail->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCouponSerail, 1, 2, 1, 1);

        leDept = new EQLineEdit(frame);
        leDept->setObjectName(QStringLiteral("leDept"));
        leDept->setFocusPolicy(Qt::NoFocus);
        leDept->setReadOnly(true);

        gridLayout->addWidget(leDept, 4, 5, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 4, 1, 1);

        label_10 = new QLabel(frame);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 3, 0, 1, 1);

        label_8 = new QLabel(frame);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 4, 4, 1, 1);

        leCouponAmount = new EQLineEdit(frame);
        leCouponAmount->setObjectName(QStringLiteral("leCouponAmount"));
        leCouponAmount->setMaximumSize(QSize(150, 16777215));
        leCouponAmount->setReadOnly(true);

        gridLayout->addWidget(leCouponAmount, 3, 1, 1, 1);

        leDiscountAmount = new EQLineEdit(frame);
        leDiscountAmount->setObjectName(QStringLiteral("leDiscountAmount"));
        leDiscountAmount->setMaximumSize(QSize(150, 16777215));
        leDiscountAmount->setReadOnly(true);

        gridLayout->addWidget(leDiscountAmount, 3, 5, 1, 1);

        label_11 = new QLabel(frame);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 3, 4, 1, 1);

        leCardHolder = new EQLineEdit(frame);
        leCardHolder->setObjectName(QStringLiteral("leCardHolder"));
        leCardHolder->setEnabled(false);
        leCardHolder->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(leCardHolder, 2, 5, 1, 1);

        lbVisit = new QLabel(frame);
        lbVisit->setObjectName(QStringLiteral("lbVisit"));

        gridLayout->addWidget(lbVisit, 2, 6, 1, 1);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgPayment);

        QMetaObject::connectSlotsByName(DlgPayment);
    } // setupUi

    void retranslateUi(QDialog *DlgPayment)
    {
        DlgPayment->setWindowTitle(QApplication::translate("DlgPayment", "Dialog", nullptr));
        label_9->setText(QApplication::translate("DlgPayment", "Debt holder", nullptr));
        btnDeptHolder->setText(QApplication::translate("DlgPayment", "...", nullptr));
        btnOk->setText(QApplication::translate("DlgPayment", "Save", nullptr));
        btnCancel->setText(QApplication::translate("DlgPayment", "Cancel", nullptr));
        btnPrintTax->setText(QApplication::translate("DlgPayment", "Print tax", nullptr));
        label_7->setText(QApplication::translate("DlgPayment", "Final amount", nullptr));
        label_4->setText(QApplication::translate("DlgPayment", "Coupon number", nullptr));
        btnCouponNumber->setText(QApplication::translate("DlgPayment", "...", nullptr));
        btnCard->setText(QApplication::translate("DlgPayment", "...", nullptr));
        label_3->setText(QApplication::translate("DlgPayment", "Coupon seria", nullptr));
        label->setText(QApplication::translate("DlgPayment", "Cash", nullptr));
        btnDept->setText(QApplication::translate("DlgPayment", "...", nullptr));
        label_2->setText(QApplication::translate("DlgPayment", "Card", nullptr));
        label_5->setText(QApplication::translate("DlgPayment", "Discount card", nullptr));
        btnCash->setText(QApplication::translate("DlgPayment", "...", nullptr));
        btnCouponSerail->setText(QApplication::translate("DlgPayment", "...", nullptr));
        label_6->setText(QApplication::translate("DlgPayment", "Cardholder", nullptr));
        label_10->setText(QApplication::translate("DlgPayment", "Coupon amount", nullptr));
        label_8->setText(QApplication::translate("DlgPayment", "Debt", nullptr));
        label_11->setText(QApplication::translate("DlgPayment", "Discount amount", nullptr));
        lbVisit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgPayment: public Ui_DlgPayment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGPAYMENT_H
