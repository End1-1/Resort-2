/********************************************************************************
** Form generated from reading UI file 'dlgpayment.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGPAYMENT_H
#define UI_DLGPAYMENT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
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
    QLabel *label_11;
    EQLineEdit *leCouponSerial;
    QLabel *label_4;
    QToolButton *btnDept;
    QLabel *label;
    QToolButton *btnCouponNumber;
    QLabel *label_6;
    EQLineEdit *leCouponNumber;
    QToolButton *btnCard;
    QToolButton *btnCash;
    QLabel *label_10;
    QLabel *label_3;
    QLabel *label_5;
    EQLineEdit *leDiscountAmount;
    QLabel *lbVisit;
    EQLineEdit *leCardHolder;
    EQLineEdit *leCash;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    EQLineEdit *leDeptHolder;
    QToolButton *btnDeptHolder;
    QLabel *label_2;
    QToolButton *btnCouponSerail;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnPrintTax;
    EQLineEdit *leCard;
    EQLineEdit *leFinalAmount;
    QLabel *label_7;
    EQLineEdit *leDiscount;
    EQLineEdit *leCouponAmount;
    QLabel *label_12;
    EQLineEdit *leServiceValue;
    EQLineEdit *leDept;
    QLabel *label_8;

    void setupUi(QDialog *DlgPayment)
    {
        if (DlgPayment->objectName().isEmpty())
            DlgPayment->setObjectName(QString::fromUtf8("DlgPayment"));
        DlgPayment->resize(800, 489);
        horizontalLayout = new QHBoxLayout(DlgPayment);
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(DlgPayment);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(30);
        label_11 = new QLabel(frame);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 3, 4, 1, 1);

        leCouponSerial = new EQLineEdit(frame);
        leCouponSerial->setObjectName(QString::fromUtf8("leCouponSerial"));
        leCouponSerial->setMaximumSize(QSize(150, 16777215));
        leCouponSerial->setFocusPolicy(Qt::NoFocus);
        leCouponSerial->setReadOnly(true);

        gridLayout->addWidget(leCouponSerial, 1, 1, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 4, 1, 1);

        btnDept = new QToolButton(frame);
        btnDept->setObjectName(QString::fromUtf8("btnDept"));
        btnDept->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnDept, 5, 6, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        btnCouponNumber = new QToolButton(frame);
        btnCouponNumber->setObjectName(QString::fromUtf8("btnCouponNumber"));
        btnCouponNumber->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCouponNumber, 1, 6, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 2, 4, 1, 1);

        leCouponNumber = new EQLineEdit(frame);
        leCouponNumber->setObjectName(QString::fromUtf8("leCouponNumber"));
        leCouponNumber->setFocusPolicy(Qt::NoFocus);
        leCouponNumber->setReadOnly(true);

        gridLayout->addWidget(leCouponNumber, 1, 5, 1, 1);

        btnCard = new QToolButton(frame);
        btnCard->setObjectName(QString::fromUtf8("btnCard"));
        btnCard->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCard, 0, 6, 1, 1);

        btnCash = new QToolButton(frame);
        btnCash->setObjectName(QString::fromUtf8("btnCash"));
        btnCash->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCash, 0, 2, 1, 2);

        label_10 = new QLabel(frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 3, 0, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        leDiscountAmount = new EQLineEdit(frame);
        leDiscountAmount->setObjectName(QString::fromUtf8("leDiscountAmount"));
        leDiscountAmount->setMaximumSize(QSize(150, 16777215));
        leDiscountAmount->setFocusPolicy(Qt::ClickFocus);
        leDiscountAmount->setReadOnly(true);

        gridLayout->addWidget(leDiscountAmount, 3, 5, 1, 1);

        lbVisit = new QLabel(frame);
        lbVisit->setObjectName(QString::fromUtf8("lbVisit"));

        gridLayout->addWidget(lbVisit, 2, 6, 1, 1);

        leCardHolder = new EQLineEdit(frame);
        leCardHolder->setObjectName(QString::fromUtf8("leCardHolder"));
        leCardHolder->setEnabled(false);
        leCardHolder->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(leCardHolder, 2, 5, 1, 1);

        leCash = new EQLineEdit(frame);
        leCash->setObjectName(QString::fromUtf8("leCash"));
        leCash->setMaximumSize(QSize(150, 16777215));
        leCash->setFocusPolicy(Qt::NoFocus);
        leCash->setReadOnly(true);

        gridLayout->addWidget(leCash, 0, 1, 1, 1);

        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, 0, -1);
        label_9 = new QLabel(widget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_3->addWidget(label_9);

        leDeptHolder = new EQLineEdit(widget_2);
        leDeptHolder->setObjectName(QString::fromUtf8("leDeptHolder"));
        leDeptHolder->setFocusPolicy(Qt::NoFocus);
        leDeptHolder->setReadOnly(true);

        horizontalLayout_3->addWidget(leDeptHolder);

        btnDeptHolder = new QToolButton(widget_2);
        btnDeptHolder->setObjectName(QString::fromUtf8("btnDeptHolder"));
        btnDeptHolder->setEnabled(false);
        btnDeptHolder->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(btnDeptHolder);


        gridLayout->addWidget(widget_2, 7, 0, 1, 7);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 4, 1, 1);

        btnCouponSerail = new QToolButton(frame);
        btnCouponSerail->setObjectName(QString::fromUtf8("btnCouponSerail"));
        btnCouponSerail->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btnCouponSerail, 1, 2, 1, 1);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setMinimumSize(QSize(100, 50));
        btnOk->setFocusPolicy(Qt::ClickFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOk->setIcon(icon);
        btnOk->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setMinimumSize(QSize(100, 50));
        btnCancel->setFocusPolicy(Qt::ClickFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCancel->setIcon(icon1);
        btnCancel->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnPrintTax = new QPushButton(widget);
        btnPrintTax->setObjectName(QString::fromUtf8("btnPrintTax"));
        btnPrintTax->setMinimumSize(QSize(100, 50));
        btnPrintTax->setFocusPolicy(Qt::ClickFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintTax->setIcon(icon2);
        btnPrintTax->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnPrintTax);


        gridLayout->addWidget(widget, 8, 0, 1, 7);

        leCard = new EQLineEdit(frame);
        leCard->setObjectName(QString::fromUtf8("leCard"));
        leCard->setFocusPolicy(Qt::NoFocus);
        leCard->setReadOnly(true);

        gridLayout->addWidget(leCard, 0, 5, 1, 1);

        leFinalAmount = new EQLineEdit(frame);
        leFinalAmount->setObjectName(QString::fromUtf8("leFinalAmount"));
        leFinalAmount->setMaximumSize(QSize(150, 16777215));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        leFinalAmount->setFont(font);
        leFinalAmount->setFocusPolicy(Qt::NoFocus);
        leFinalAmount->setReadOnly(true);

        gridLayout->addWidget(leFinalAmount, 5, 1, 1, 1);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        leDiscount = new EQLineEdit(frame);
        leDiscount->setObjectName(QString::fromUtf8("leDiscount"));
        leDiscount->setMaximumSize(QSize(150, 16777215));
        leDiscount->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(leDiscount, 2, 1, 1, 1);

        leCouponAmount = new EQLineEdit(frame);
        leCouponAmount->setObjectName(QString::fromUtf8("leCouponAmount"));
        leCouponAmount->setMaximumSize(QSize(150, 16777215));
        leCouponAmount->setFocusPolicy(Qt::ClickFocus);
        leCouponAmount->setReadOnly(true);

        gridLayout->addWidget(leCouponAmount, 3, 1, 1, 1);

        label_12 = new QLabel(frame);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 4, 0, 1, 1);

        leServiceValue = new EQLineEdit(frame);
        leServiceValue->setObjectName(QString::fromUtf8("leServiceValue"));
        leServiceValue->setMaximumSize(QSize(150, 16777215));
        leServiceValue->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(leServiceValue, 4, 1, 1, 1);

        leDept = new EQLineEdit(frame);
        leDept->setObjectName(QString::fromUtf8("leDept"));
        leDept->setFocusPolicy(Qt::NoFocus);
        leDept->setReadOnly(true);

        gridLayout->addWidget(leDept, 4, 5, 1, 1);

        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 4, 4, 1, 1);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgPayment);

        QMetaObject::connectSlotsByName(DlgPayment);
    } // setupUi

    void retranslateUi(QDialog *DlgPayment)
    {
        DlgPayment->setWindowTitle(QCoreApplication::translate("DlgPayment", "Dialog", nullptr));
        label_11->setText(QCoreApplication::translate("DlgPayment", "Discount amount", nullptr));
        label_4->setText(QCoreApplication::translate("DlgPayment", "Coupon number", nullptr));
        btnDept->setText(QCoreApplication::translate("DlgPayment", "...", nullptr));
        label->setText(QCoreApplication::translate("DlgPayment", "Cash", nullptr));
        btnCouponNumber->setText(QCoreApplication::translate("DlgPayment", "...", nullptr));
        label_6->setText(QCoreApplication::translate("DlgPayment", "Cardholder", nullptr));
        btnCard->setText(QCoreApplication::translate("DlgPayment", "...", nullptr));
        btnCash->setText(QCoreApplication::translate("DlgPayment", "...", nullptr));
        label_10->setText(QCoreApplication::translate("DlgPayment", "Coupon amount", nullptr));
        label_3->setText(QCoreApplication::translate("DlgPayment", "Coupon seria", nullptr));
        label_5->setText(QCoreApplication::translate("DlgPayment", "Discount card", nullptr));
        lbVisit->setText(QString());
        label_9->setText(QCoreApplication::translate("DlgPayment", "Debt holder", nullptr));
        btnDeptHolder->setText(QCoreApplication::translate("DlgPayment", "...", nullptr));
        label_2->setText(QCoreApplication::translate("DlgPayment", "Card", nullptr));
        btnCouponSerail->setText(QCoreApplication::translate("DlgPayment", "...", nullptr));
        btnOk->setText(QCoreApplication::translate("DlgPayment", "Save", nullptr));
        btnCancel->setText(QCoreApplication::translate("DlgPayment", "Cancel", nullptr));
        btnPrintTax->setText(QCoreApplication::translate("DlgPayment", "Print tax", nullptr));
        label_7->setText(QCoreApplication::translate("DlgPayment", "Final amount", nullptr));
        label_12->setText(QCoreApplication::translate("DlgPayment", "Service", nullptr));
        label_8->setText(QCoreApplication::translate("DlgPayment", "Debt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgPayment: public Ui_DlgPayment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGPAYMENT_H
