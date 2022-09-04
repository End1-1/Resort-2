/********************************************************************************
** Form generated from reading UI file 'dlgsmile.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSMILE_H
#define UI_DLGSMILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_DlgSmile
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *lbSmile;

    void setupUi(QDialog *DlgSmile)
    {
        if (DlgSmile->objectName().isEmpty())
            DlgSmile->setObjectName(QString::fromUtf8("DlgSmile"));
        DlgSmile->resize(218, 218);
        DlgSmile->setMinimumSize(QSize(218, 218));
        DlgSmile->setMaximumSize(QSize(218, 218));
        horizontalLayout = new QHBoxLayout(DlgSmile);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbSmile = new QLabel(DlgSmile);
        lbSmile->setObjectName(QString::fromUtf8("lbSmile"));
        lbSmile->setMaximumSize(QSize(200, 200));
        lbSmile->setPixmap(QPixmap(QString::fromUtf8(":/images/smile.png")));
        lbSmile->setScaledContents(true);

        horizontalLayout->addWidget(lbSmile);


        retranslateUi(DlgSmile);

        QMetaObject::connectSlotsByName(DlgSmile);
    } // setupUi

    void retranslateUi(QDialog *DlgSmile)
    {
        DlgSmile->setWindowTitle(QCoreApplication::translate("DlgSmile", "Smile", nullptr));
        lbSmile->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgSmile: public Ui_DlgSmile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSMILE_H
