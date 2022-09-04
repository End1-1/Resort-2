/********************************************************************************
** Form generated from reading UI file 'dlgcarselection.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCARSELECTION_H
#define UI_DLGCARSELECTION_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <eqlineedit.h>

QT_BEGIN_NAMESPACE

class Ui_DlgCarSelection
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *leCar;
    QPushButton *btnModelSelection;
    QLabel *label_2;
    EQLineEdit *leGovNum;
    QPushButton *btnNumberSelection;
    QLabel *label_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;
    EQLineEdit *leCostumer;
    QPushButton *btnCostumer;

    void setupUi(QDialog *DlgCarSelection)
    {
        if (DlgCarSelection->objectName().isEmpty())
            DlgCarSelection->setObjectName(QString::fromUtf8("DlgCarSelection"));
        DlgCarSelection->resize(642, 265);
        verticalLayout = new QVBoxLayout(DlgCarSelection);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(DlgCarSelection);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(30);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        leCar = new QLineEdit(frame);
        leCar->setObjectName(QString::fromUtf8("leCar"));

        gridLayout->addWidget(leCar, 0, 1, 1, 1);

        btnModelSelection = new QPushButton(frame);
        btnModelSelection->setObjectName(QString::fromUtf8("btnModelSelection"));

        gridLayout->addWidget(btnModelSelection, 0, 2, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        leGovNum = new EQLineEdit(frame);
        leGovNum->setObjectName(QString::fromUtf8("leGovNum"));

        gridLayout->addWidget(leGovNum, 1, 1, 1, 1);

        btnNumberSelection = new QPushButton(frame);
        btnNumberSelection->setObjectName(QString::fromUtf8("btnNumberSelection"));

        gridLayout->addWidget(btnNumberSelection, 1, 2, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnSave = new QPushButton(widget);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMinimumSize(QSize(0, 50));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon);

        horizontalLayout->addWidget(btnSave);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 50));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCancel->setIcon(icon1);
        btnCancel->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addWidget(widget, 3, 0, 1, 3);

        leCostumer = new EQLineEdit(frame);
        leCostumer->setObjectName(QString::fromUtf8("leCostumer"));
        leCostumer->setReadOnly(true);

        gridLayout->addWidget(leCostumer, 2, 1, 1, 1);

        btnCostumer = new QPushButton(frame);
        btnCostumer->setObjectName(QString::fromUtf8("btnCostumer"));

        gridLayout->addWidget(btnCostumer, 2, 2, 1, 1);


        verticalLayout->addWidget(frame);


        retranslateUi(DlgCarSelection);

        QMetaObject::connectSlotsByName(DlgCarSelection);
    } // setupUi

    void retranslateUi(QDialog *DlgCarSelection)
    {
        DlgCarSelection->setWindowTitle(QCoreApplication::translate("DlgCarSelection", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DlgCarSelection", "Model of car", nullptr));
        btnModelSelection->setText(QCoreApplication::translate("DlgCarSelection", "...", nullptr));
        label_2->setText(QCoreApplication::translate("DlgCarSelection", "Gov. number", nullptr));
        btnNumberSelection->setText(QCoreApplication::translate("DlgCarSelection", "...", nullptr));
        label_3->setText(QCoreApplication::translate("DlgCarSelection", "Costumer", nullptr));
        btnSave->setText(QCoreApplication::translate("DlgCarSelection", "Save", nullptr));
        btnCancel->setText(QCoreApplication::translate("DlgCarSelection", "Cancel", nullptr));
        btnCostumer->setText(QCoreApplication::translate("DlgCarSelection", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgCarSelection: public Ui_DlgCarSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCARSELECTION_H
