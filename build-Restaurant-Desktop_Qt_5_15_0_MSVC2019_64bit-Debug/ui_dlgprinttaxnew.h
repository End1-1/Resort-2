/********************************************************************************
** Form generated from reading UI file 'dlgprinttaxnew.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGPRINTTAXNEW_H
#define UI_DLGPRINTTAXNEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_DlgPrintTaxNew
{
public:

    void setupUi(QDialog *DlgPrintTaxNew)
    {
        if (DlgPrintTaxNew->objectName().isEmpty())
            DlgPrintTaxNew->setObjectName(QString::fromUtf8("DlgPrintTaxNew"));
        DlgPrintTaxNew->resize(576, 468);

        retranslateUi(DlgPrintTaxNew);

        QMetaObject::connectSlotsByName(DlgPrintTaxNew);
    } // setupUi

    void retranslateUi(QDialog *DlgPrintTaxNew)
    {
        DlgPrintTaxNew->setWindowTitle(QCoreApplication::translate("DlgPrintTaxNew", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgPrintTaxNew: public Ui_DlgPrintTaxNew {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGPRINTTAXNEW_H
