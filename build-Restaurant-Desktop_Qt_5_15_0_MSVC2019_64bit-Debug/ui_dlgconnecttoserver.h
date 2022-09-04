/********************************************************************************
** Form generated from reading UI file 'dlgconnecttoserver.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNECTTOSERVER_H
#define UI_DLGCONNECTTOSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgConnectToServer
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;

    void setupUi(QDialog *DlgConnectToServer)
    {
        if (DlgConnectToServer->objectName().isEmpty())
            DlgConnectToServer->setObjectName(QString::fromUtf8("DlgConnectToServer"));
        DlgConnectToServer->resize(442, 144);
        verticalLayout = new QVBoxLayout(DlgConnectToServer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(DlgConnectToServer);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(14);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);


        verticalLayout->addWidget(frame);


        retranslateUi(DlgConnectToServer);

        QMetaObject::connectSlotsByName(DlgConnectToServer);
    } // setupUi

    void retranslateUi(QDialog *DlgConnectToServer)
    {
        DlgConnectToServer->setWindowTitle(QCoreApplication::translate("DlgConnectToServer", "Connection", nullptr));
        label->setText(QCoreApplication::translate("DlgConnectToServer", "Connecting to server...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgConnectToServer: public Ui_DlgConnectToServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNECTTOSERVER_H
