/********************************************************************************
** Form generated from reading UI file 'dlgcheckcard.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCHECKCARD_H
#define UI_DLGCHECKCARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgCheckCard
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *leCode;
    QTextEdit *teResult;
    QPushButton *btnClose;

    void setupUi(QDialog *DlgCheckCard)
    {
        if (DlgCheckCard->objectName().isEmpty())
            DlgCheckCard->setObjectName(QString::fromUtf8("DlgCheckCard"));
        DlgCheckCard->resize(611, 657);
        verticalLayout = new QVBoxLayout(DlgCheckCard);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        leCode = new QLineEdit(DlgCheckCard);
        leCode->setObjectName(QString::fromUtf8("leCode"));

        verticalLayout->addWidget(leCode);

        teResult = new QTextEdit(DlgCheckCard);
        teResult->setObjectName(QString::fromUtf8("teResult"));
        teResult->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(teResult);

        btnClose = new QPushButton(DlgCheckCard);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(btnClose);


        retranslateUi(DlgCheckCard);

        QMetaObject::connectSlotsByName(DlgCheckCard);
    } // setupUi

    void retranslateUi(QDialog *DlgCheckCard)
    {
        DlgCheckCard->setWindowTitle(QCoreApplication::translate("DlgCheckCard", "Dialog", nullptr));
        btnClose->setText(QCoreApplication::translate("DlgCheckCard", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgCheckCard: public Ui_DlgCheckCard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCHECKCARD_H
