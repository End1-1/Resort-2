/********************************************************************************
** Form generated from reading UI file 'rchangelanguage.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCHANGELANGUAGE_H
#define UI_RCHANGELANGUAGE_H

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

class Ui_RChangeLanguage
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QListWidget *lstCountry;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *RChangeLanguage)
    {
        if (RChangeLanguage->objectName().isEmpty())
            RChangeLanguage->setObjectName(QStringLiteral("RChangeLanguage"));
        RChangeLanguage->resize(400, 300);
        RChangeLanguage->setMinimumSize(QSize(400, 300));
        RChangeLanguage->setMaximumSize(QSize(400, 300));
        verticalLayout = new QVBoxLayout(RChangeLanguage);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(RChangeLanguage);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lstCountry = new QListWidget(frame);
        lstCountry->setObjectName(QStringLiteral("lstCountry"));

        verticalLayout_2->addWidget(lstCountry);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(95, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(widget);


        verticalLayout->addWidget(frame);


        retranslateUi(RChangeLanguage);

        QMetaObject::connectSlotsByName(RChangeLanguage);
    } // setupUi

    void retranslateUi(QDialog *RChangeLanguage)
    {
        RChangeLanguage->setWindowTitle(QApplication::translate("RChangeLanguage", "Dialog", nullptr));
        btnOk->setText(QApplication::translate("RChangeLanguage", "OK", nullptr));
        btnCancel->setText(QApplication::translate("RChangeLanguage", "No", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RChangeLanguage: public Ui_RChangeLanguage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCHANGELANGUAGE_H
