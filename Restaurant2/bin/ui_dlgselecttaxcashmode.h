/********************************************************************************
** Form generated from reading UI file 'dlgselecttaxcashmode.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSELECTTAXCASHMODE_H
#define UI_DLGSELECTTAXCASHMODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            DlgSelectTaxCashMode->setObjectName(QStringLiteral("DlgSelectTaxCashMode"));
        DlgSelectTaxCashMode->resize(267, 160);
        horizontalLayout = new QHBoxLayout(DlgSelectTaxCashMode);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(DlgSelectTaxCashMode);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        btnCashLess = new QPushButton(frame);
        btnCashLess->setObjectName(QStringLiteral("btnCashLess"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/maestro.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCashLess->setIcon(icon);

        verticalLayout->addWidget(btnCashLess);

        btnCash = new QPushButton(frame);
        btnCash->setObjectName(QStringLiteral("btnCash"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCash->setIcon(icon1);

        verticalLayout->addWidget(btnCash);

        btnCancel = new QPushButton(frame);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

        verticalLayout->addWidget(btnCancel);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgSelectTaxCashMode);

        QMetaObject::connectSlotsByName(DlgSelectTaxCashMode);
    } // setupUi

    void retranslateUi(QDialog *DlgSelectTaxCashMode)
    {
        DlgSelectTaxCashMode->setWindowTitle(QApplication::translate("DlgSelectTaxCashMode", "Dialog", nullptr));
        btnCashLess->setText(QApplication::translate("DlgSelectTaxCashMode", "Card", nullptr));
        btnCash->setText(QApplication::translate("DlgSelectTaxCashMode", "Cash", nullptr));
        btnCancel->setText(QApplication::translate("DlgSelectTaxCashMode", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgSelectTaxCashMode: public Ui_DlgSelectTaxCashMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSELECTTAXCASHMODE_H
