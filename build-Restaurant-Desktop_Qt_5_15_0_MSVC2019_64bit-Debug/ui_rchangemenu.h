/********************************************************************************
** Form generated from reading UI file 'rchangemenu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCHANGEMENU_H
#define UI_RCHANGEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RChangeMenu
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QListWidget *lstMenu;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *RChangeMenu)
    {
        if (RChangeMenu->objectName().isEmpty())
            RChangeMenu->setObjectName(QString::fromUtf8("RChangeMenu"));
        RChangeMenu->resize(419, 504);
        verticalLayout = new QVBoxLayout(RChangeMenu);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(RChangeMenu);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lstMenu = new QListWidget(frame);
        lstMenu->setObjectName(QString::fromUtf8("lstMenu"));

        verticalLayout_2->addWidget(lstMenu);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(95, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(94, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(widget);


        verticalLayout->addWidget(frame);


        retranslateUi(RChangeMenu);

        QMetaObject::connectSlotsByName(RChangeMenu);
    } // setupUi

    void retranslateUi(QDialog *RChangeMenu)
    {
        RChangeMenu->setWindowTitle(QCoreApplication::translate("RChangeMenu", "Dialog", nullptr));
        btnOk->setText(QCoreApplication::translate("RChangeMenu", "Ok", nullptr));
        btnCancel->setText(QCoreApplication::translate("RChangeMenu", "No", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RChangeMenu: public Ui_RChangeMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCHANGEMENU_H
