/********************************************************************************
** Form generated from reading UI file 'dlglist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGLIST_H
#define UI_DLGLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <rkeyboard.h>

QT_BEGIN_NAMESPACE

class Ui_DlgList
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *lbCaption;
    QListWidget *lst;
    RKeyboard *wKbd;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *DlgList)
    {
        if (DlgList->objectName().isEmpty())
            DlgList->setObjectName(QString::fromUtf8("DlgList"));
        DlgList->resize(841, 688);
        horizontalLayout = new QHBoxLayout(DlgList);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(DlgList);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lbCaption = new QLabel(frame);
        lbCaption->setObjectName(QString::fromUtf8("lbCaption"));
        lbCaption->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbCaption);

        lst = new QListWidget(frame);
        lst->setObjectName(QString::fromUtf8("lst"));
        lst->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lst->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(lst);

        wKbd = new RKeyboard(frame);
        wKbd->setObjectName(QString::fromUtf8("wKbd"));

        verticalLayout->addWidget(wKbd);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(99, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setMinimumSize(QSize(0, 50));

        horizontalLayout_2->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 50));

        horizontalLayout_2->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgList);

        QMetaObject::connectSlotsByName(DlgList);
    } // setupUi

    void retranslateUi(QDialog *DlgList)
    {
        DlgList->setWindowTitle(QCoreApplication::translate("DlgList", "Dialog", nullptr));
        lbCaption->setText(QCoreApplication::translate("DlgList", "Caption", nullptr));
        btnOk->setText(QCoreApplication::translate("DlgList", "OK", nullptr));
        btnCancel->setText(QCoreApplication::translate("DlgList", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgList: public Ui_DlgList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGLIST_H
