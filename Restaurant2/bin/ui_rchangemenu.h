/********************************************************************************
** Form generated from reading UI file 'rchangemenu.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCHANGEMENU_H
#define UI_RCHANGEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            RChangeMenu->setObjectName(QStringLiteral("RChangeMenu"));
        RChangeMenu->resize(419, 504);
        verticalLayout = new QVBoxLayout(RChangeMenu);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(RChangeMenu);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lstMenu = new QListWidget(frame);
        lstMenu->setObjectName(QStringLiteral("lstMenu"));

        verticalLayout_2->addWidget(lstMenu);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(95, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

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
        RChangeMenu->setWindowTitle(QApplication::translate("RChangeMenu", "Dialog", nullptr));
        btnOk->setText(QApplication::translate("RChangeMenu", "Ok", nullptr));
        btnCancel->setText(QApplication::translate("RChangeMenu", "No", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RChangeMenu: public Ui_RChangeMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCHANGEMENU_H
