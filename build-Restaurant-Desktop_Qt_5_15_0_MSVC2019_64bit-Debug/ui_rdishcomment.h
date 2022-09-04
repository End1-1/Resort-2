/********************************************************************************
** Form generated from reading UI file 'rdishcomment.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RDISHCOMMENT_H
#define UI_RDISHCOMMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <rkeyboard.h>

QT_BEGIN_NAMESPACE

class Ui_RDishComment
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *leText;
    QTableWidget *tblVariants;
    RKeyboard *wKbd;

    void setupUi(QDialog *RDishComment)
    {
        if (RDishComment->objectName().isEmpty())
            RDishComment->setObjectName(QString::fromUtf8("RDishComment"));
        RDishComment->resize(687, 448);
        verticalLayout = new QVBoxLayout(RDishComment);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(RDishComment);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        leText = new QLineEdit(frame);
        leText->setObjectName(QString::fromUtf8("leText"));

        verticalLayout_2->addWidget(leText);

        tblVariants = new QTableWidget(frame);
        tblVariants->setObjectName(QString::fromUtf8("tblVariants"));

        verticalLayout_2->addWidget(tblVariants);

        wKbd = new RKeyboard(frame);
        wKbd->setObjectName(QString::fromUtf8("wKbd"));

        verticalLayout_2->addWidget(wKbd);


        verticalLayout->addWidget(frame);


        retranslateUi(RDishComment);

        QMetaObject::connectSlotsByName(RDishComment);
    } // setupUi

    void retranslateUi(QDialog *RDishComment)
    {
        RDishComment->setWindowTitle(QCoreApplication::translate("RDishComment", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RDishComment: public Ui_RDishComment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RDISHCOMMENT_H
