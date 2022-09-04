/********************************************************************************
** Form generated from reading UI file 'message.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGE_H
#define UI_MESSAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_message
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lbMessage;
    QGridLayout *gridLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCopy;
    QPushButton *btnYes;
    QPushButton *btnNo;
    QPushButton *btnNoAll;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *message)
    {
        if (message->objectName().isEmpty())
            message->setObjectName(QString::fromUtf8("message"));
        message->resize(530, 225);
        verticalLayout = new QVBoxLayout(message);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lbMessage = new QLabel(message);
        lbMessage->setObjectName(QString::fromUtf8("lbMessage"));
        QFont font;
        font.setPointSize(14);
        lbMessage->setFont(font);
        lbMessage->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbMessage);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget = new QWidget(message);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout_2->addWidget(btnOk);

        btnCopy = new QPushButton(widget);
        btnCopy->setObjectName(QString::fromUtf8("btnCopy"));

        horizontalLayout_2->addWidget(btnCopy);

        btnYes = new QPushButton(widget);
        btnYes->setObjectName(QString::fromUtf8("btnYes"));

        horizontalLayout_2->addWidget(btnYes);

        btnNo = new QPushButton(widget);
        btnNo->setObjectName(QString::fromUtf8("btnNo"));

        horizontalLayout_2->addWidget(btnNo);

        btnNoAll = new QPushButton(widget);
        btnNoAll->setObjectName(QString::fromUtf8("btnNoAll"));

        horizontalLayout_2->addWidget(btnNoAll);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout_2->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addWidget(widget, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(message);

        QMetaObject::connectSlotsByName(message);
    } // setupUi

    void retranslateUi(QDialog *message)
    {
        message->setWindowTitle(QString());
        lbMessage->setText(QCoreApplication::translate("message", "<html><body style=\"background-color:green\"><p align=\"center\"><img src=\":/images/app.png\"></p>\n"
"    <p align=\"center\" style=\"font-size:12px; color:black;\">This is a river</p></body></html>", nullptr));
        btnOk->setText(QCoreApplication::translate("message", "Ok", nullptr));
        btnCopy->setText(QCoreApplication::translate("message", "Copy", nullptr));
        btnYes->setText(QCoreApplication::translate("message", "Yes", nullptr));
        btnNo->setText(QCoreApplication::translate("message", "No", nullptr));
        btnNoAll->setText(QCoreApplication::translate("message", "No all", nullptr));
        btnCancel->setText(QCoreApplication::translate("message", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class message: public Ui_message {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGE_H
