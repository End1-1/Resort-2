/********************************************************************************
** Form generated from reading UI file 'dlgselecttaxcashmode.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSELECTTAXCASHMODE_H
#define UI_DLGSELECTTAXCASHMODE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgSelectTaxCashMode
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *btnCashLess;
    QPushButton *btnCash;
    QPushButton *btnCancel;

    void setupUi(QDialog *DlgSelectTaxCashMode)
    {
        if (DlgSelectTaxCashMode->objectName().isEmpty())
            DlgSelectTaxCashMode->setObjectName(QString::fromUtf8("DlgSelectTaxCashMode"));
        DlgSelectTaxCashMode->resize(267, 160);
        horizontalLayout = new QHBoxLayout(DlgSelectTaxCashMode);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(DlgSelectTaxCashMode);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btnCashLess = new QPushButton(frame);
        btnCashLess->setObjectName(QString::fromUtf8("btnCashLess"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/maestro.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCashLess->setIcon(icon);

        verticalLayout->addWidget(btnCashLess);

        btnCash = new QPushButton(frame);
        btnCash->setObjectName(QString::fromUtf8("btnCash"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCash->setIcon(icon1);

        verticalLayout->addWidget(btnCash);

        btnCancel = new QPushButton(frame);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        verticalLayout->addWidget(btnCancel);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgSelectTaxCashMode);

        QMetaObject::connectSlotsByName(DlgSelectTaxCashMode);
    } // setupUi

    void retranslateUi(QDialog *DlgSelectTaxCashMode)
    {
        DlgSelectTaxCashMode->setWindowTitle(QCoreApplication::translate("DlgSelectTaxCashMode", "Dialog", nullptr));
        btnCashLess->setText(QCoreApplication::translate("DlgSelectTaxCashMode", "Card", nullptr));
        btnCash->setText(QCoreApplication::translate("DlgSelectTaxCashMode", "Cash", nullptr));
        btnCancel->setText(QCoreApplication::translate("DlgSelectTaxCashMode", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgSelectTaxCashMode: public Ui_DlgSelectTaxCashMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSELECTTAXCASHMODE_H
