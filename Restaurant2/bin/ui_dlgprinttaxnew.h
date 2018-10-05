/********************************************************************************
** Form generated from reading UI file 'dlgprinttaxnew.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGPRINTTAXNEW_H
#define UI_DLGPRINTTAXNEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DlgPrintTaxNew
{
public:

    void setupUi(QDialog *DlgPrintTaxNew)
    {
        if (DlgPrintTaxNew->objectName().isEmpty())
            DlgPrintTaxNew->setObjectName(QStringLiteral("DlgPrintTaxNew"));
        DlgPrintTaxNew->resize(576, 468);

        retranslateUi(DlgPrintTaxNew);

        QMetaObject::connectSlotsByName(DlgPrintTaxNew);
    } // setupUi

    void retranslateUi(QDialog *DlgPrintTaxNew)
    {
        DlgPrintTaxNew->setWindowTitle(QApplication::translate("DlgPrintTaxNew", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgPrintTaxNew: public Ui_DlgPrintTaxNew {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGPRINTTAXNEW_H
