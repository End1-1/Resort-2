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
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DlgDate
{
public:
    QGridLayout *gridLayout;
    QPushButton *btnCancel;
    QPushButton *btnOk;
    QDateEdit *deDate;

    void setupUi(QDialog *DlgDate)
    {
        if (DlgDate->objectName().isEmpty())
            DlgDate->setObjectName(QStringLiteral("DlgDate"));
        DlgDate->resize(400, 120);
        gridLayout = new QGridLayout(DlgDate);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnCancel = new QPushButton(DlgDate);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(btnCancel, 1, 1, 1, 1);

        btnOk = new QPushButton(DlgDate);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(btnOk, 1, 0, 1, 1);

        deDate = new QDateEdit(DlgDate);
        deDate->setObjectName(QStringLiteral("deDate"));
        QFont font;
        font.setPointSize(25);
        deDate->setFont(font);
        deDate->setCalendarPopup(true);

        gridLayout->addWidget(deDate, 0, 0, 1, 2);


        retranslateUi(DlgDate);

        QMetaObject::connectSlotsByName(DlgDate);
    } // setupUi

    void retranslateUi(QDialog *DlgDate)
    {
        DlgDate->setWindowTitle(QApplication::translate("DlgDate", "Date", nullptr));
        btnCancel->setText(QApplication::translate("DlgDate", "Cancel", nullptr));
        btnOk->setText(QApplication::translate("DlgDate", "OK", nullptr));
        deDate->setDisplayFormat(QApplication::translate("DlgDate", "dd.MM.yyyy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgDate: public Ui_DlgDate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGDATE_H
