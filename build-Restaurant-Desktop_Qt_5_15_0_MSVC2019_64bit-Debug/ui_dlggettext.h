/********************************************************************************
** Form generated from reading UI file 'dlggettext.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGGETTEXT_H
#define UI_DLGGETTEXT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <rkeyboard.h>

QT_BEGIN_NAMESPACE

class Ui_DlgGetText
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *leText;
    RKeyboard *wdtKbd;

    void setupUi(QDialog *DlgGetText)
    {
        if (DlgGetText->objectName().isEmpty())
            DlgGetText->setObjectName(QString::fromUtf8("DlgGetText"));
        DlgGetText->resize(824, 319);
        verticalLayout = new QVBoxLayout(DlgGetText);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(DlgGetText);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        leText = new QLineEdit(frame);
        leText->setObjectName(QString::fromUtf8("leText"));
        QFont font;
        font.setPointSize(30);
        leText->setFont(font);
        leText->setFocusPolicy(Qt::NoFocus);
        leText->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(leText);

        wdtKbd = new RKeyboard(frame);
        wdtKbd->setObjectName(QString::fromUtf8("wdtKbd"));

        verticalLayout_2->addWidget(wdtKbd);


        verticalLayout->addWidget(frame);


        retranslateUi(DlgGetText);

        QMetaObject::connectSlotsByName(DlgGetText);
    } // setupUi

    void retranslateUi(QDialog *DlgGetText)
    {
        DlgGetText->setWindowTitle(QCoreApplication::translate("DlgGetText", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgGetText: public Ui_DlgGetText {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGGETTEXT_H
