/********************************************************************************
** Form generated from reading UI file 'dlgcarselection.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCARSELECTION_H
#define UI_DLGCARSELECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            DlgCarSelection->setObjectName(QStringLiteral("DlgCarSelection"));
        DlgCarSelection->resize(642, 265);
        verticalLayout = new QVBoxLayout(DlgCarSelection);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(DlgCarSelection);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(30);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        leCar = new QLineEdit(frame);
        leCar->setObjectName(QStringLiteral("leCar"));

        gridLayout->addWidget(leCar, 0, 1, 1, 1);

        btnModelSelection = new QPushButton(frame);
        btnModelSelection->setObjectName(QStringLiteral("btnModelSelection"));

        gridLayout->addWidget(btnModelSelection, 0, 2, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        leGovNum = new EQLineEdit(frame);
        leGovNum->setObjectName(QStringLiteral("leGovNum"));

        gridLayout->addWidget(leGovNum, 1, 1, 1, 1);

        btnNumberSelection = new QPushButton(frame);
        btnNumberSelection->setObjectName(QStringLiteral("btnNumberSelection"));

        gridLayout->addWidget(btnNumberSelection, 1, 2, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnSave = new QPushButton(widget);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setMinimumSize(QSize(0, 50));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon);

        horizontalLayout->addWidget(btnSave);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 50));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCancel->setIcon(icon1);
        btnCancel->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addWidget(widget, 3, 0, 1, 3);

        leCostumer = new EQLineEdit(frame);
        leCostumer->setObjectName(QStringLiteral("leCostumer"));
        leCostumer->setReadOnly(true);

        gridLayout->addWidget(leCostumer, 2, 1, 1, 1);

        btnCostumer = new QPushButton(frame);
        btnCostumer->setObjectName(QStringLiteral("btnCostumer"));

        gridLayout->addWidget(btnCostumer, 2, 2, 1, 1);


        verticalLayout->addWidget(frame);


        retranslateUi(DlgCarSelection);

        QMetaObject::connectSlotsByName(DlgCarSelection);
    } // setupUi

    void retranslateUi(QDialog *DlgCarSelection)
    {
        DlgCarSelection->setWindowTitle(QApplication::translate("DlgCarSelection", "Dialog", nullptr));
        label->setText(QApplication::translate("DlgCarSelection", "Model of car", nullptr));
        btnModelSelection->setText(QApplication::translate("DlgCarSelection", "...", nullptr));
        label_2->setText(QApplication::translate("DlgCarSelection", "Gov. number", nullptr));
        btnNumberSelection->setText(QApplication::translate("DlgCarSelection", "...", nullptr));
        label_3->setText(QApplication::translate("DlgCarSelection", "Costumer", nullptr));
        btnSave->setText(QApplication::translate("DlgCarSelection", "Save", nullptr));
        btnCancel->setText(QApplication::translate("DlgCarSelection", "Cancel", nullptr));
        btnCostumer->setText(QApplication::translate("DlgCarSelection", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgCarSelection: public Ui_DlgCarSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCARSELECTION_H
