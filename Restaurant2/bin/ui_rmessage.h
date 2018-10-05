/********************************************************************************
** Form generated from reading UI file 'rmessage.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RMESSAGE_H
#define UI_RMESSAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RMessage
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QTextEdit *teMessage;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnYES;
    QPushButton *btnOK;
    QPushButton *btnCancel;
    QPushButton *btnCancel_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *RMessage)
    {
        if (RMessage->objectName().isEmpty())
            RMessage->setObjectName(QStringLiteral("RMessage"));
        RMessage->resize(456, 465);
        verticalLayout_2 = new QVBoxLayout(RMessage);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame = new QFrame(RMessage);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        teMessage = new QTextEdit(frame);
        teMessage->setObjectName(QStringLiteral("teMessage"));
        teMessage->setReadOnly(true);

        verticalLayout->addWidget(teMessage);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnYES = new QPushButton(widget);
        btnYES->setObjectName(QStringLiteral("btnYES"));
        btnYES->setMinimumSize(QSize(0, 60));
        btnYES->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnYES);

        btnOK = new QPushButton(widget);
        btnOK->setObjectName(QStringLiteral("btnOK"));
        btnOK->setMinimumSize(QSize(0, 60));
        btnOK->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnOK);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 60));
        btnCancel->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCancel);

        btnCancel_2 = new QPushButton(widget);
        btnCancel_2->setObjectName(QStringLiteral("btnCancel_2"));
        btnCancel_2->setMinimumSize(QSize(0, 60));
        btnCancel_2->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCancel_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);


        verticalLayout_2->addWidget(frame);


        retranslateUi(RMessage);

        QMetaObject::connectSlotsByName(RMessage);
    } // setupUi

    void retranslateUi(QDialog *RMessage)
    {
        RMessage->setWindowTitle(QApplication::translate("RMessage", "Dialog", nullptr));
        btnYES->setText(QApplication::translate("RMessage", "Yes", nullptr));
        btnOK->setText(QApplication::translate("RMessage", "OK", nullptr));
        btnCancel->setText(QApplication::translate("RMessage", "No", nullptr));
        btnCancel_2->setText(QApplication::translate("RMessage", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RMessage: public Ui_RMessage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RMESSAGE_H
