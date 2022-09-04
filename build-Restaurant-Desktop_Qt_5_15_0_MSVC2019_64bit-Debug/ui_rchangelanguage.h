/********************************************************************************
** Form generated from reading UI file 'rchangelanguage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCHANGELANGUAGE_H
#define UI_RCHANGELANGUAGE_H

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
            RChangeLanguage->setObjectName(QString::fromUtf8("RChangeLanguage"));
        RChangeLanguage->resize(400, 300);
        RChangeLanguage->setMinimumSize(QSize(400, 300));
        RChangeLanguage->setMaximumSize(QSize(400, 300));
        verticalLayout = new QVBoxLayout(RChangeLanguage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(RChangeLanguage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lstCountry = new QListWidget(frame);
        lstCountry->setObjectName(QString::fromUtf8("lstCountry"));

        verticalLayout_2->addWidget(lstCountry);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

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
        RChangeLanguage->setWindowTitle(QCoreApplication::translate("RChangeLanguage", "Dialog", nullptr));
        btnOk->setText(QCoreApplication::translate("RChangeLanguage", "OK", nullptr));
        btnCancel->setText(QCoreApplication::translate("RChangeLanguage", "No", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RChangeLanguage: public Ui_RChangeLanguage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCHANGELANGUAGE_H
