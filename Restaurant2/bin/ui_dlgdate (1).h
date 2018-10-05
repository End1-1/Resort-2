/********************************************************************************
** Form generated from reading UI file 'dlgdate.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGDATE_H
#define UI_DLGDATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <edateedit.h>

QT_BEGIN_NAMESPACE

class Ui_DlgDate
{
public:
    QGridLayout *gridLayout;
    EDateEdit *leDate;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *DlgDate)
    {
        if (DlgDate->objectName().isEmpty())
            DlgDate->setObjectName(QStringLiteral("DlgDate"));
        DlgDate->resize(400, 120);
        gridLayout = new QGridLayout(DlgDate);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        leDate = new EDateEdit(DlgDate);
        leDate->setObjectName(QStringLiteral("leDate"));
        QFont font;
        font.setPointSize(20);
        leDate->setFont(font);

        gridLayout->addWidget(leDate, 0, 0, 1, 2);

        btnOk = new QPushButton(DlgDate);
        btnOk->setObjectName(QStringLiteral("btnOk"));

        gridLayout->addWidget(btnOk, 1, 0, 1, 1);

        btnCancel = new QPushButton(DlgDate);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

        gridLayout->addWidget(btnCancel, 1, 1, 1, 1);


        retranslateUi(DlgDate);

        QMetaObject::connectSlotsByName(DlgDate);
    } // setupUi

    void retranslateUi(QDialog *DlgDate)
    {
        DlgDate->setWindowTitle(QApplication::translate("DlgDate", "Date", nullptr));
        btnOk->setText(QApplication::translate("DlgDate", "OK", nullptr));
        btnCancel->setText(QApplication::translate("DlgDate", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgDate: public Ui_DlgDate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGDATE_H
