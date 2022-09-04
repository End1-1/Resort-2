/********************************************************************************
** Form generated from reading UI file 'dlgbanketitems.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGBANKETITEMS_H
#define UI_DLGBANKETITEMS_H

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

class Ui_DlgBanketItems
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QListWidget *lst;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *DlgBanketItems)
    {
        if (DlgBanketItems->objectName().isEmpty())
            DlgBanketItems->setObjectName(QString::fromUtf8("DlgBanketItems"));
        DlgBanketItems->resize(522, 544);
        horizontalLayout = new QHBoxLayout(DlgBanketItems);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(DlgBanketItems);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lst = new QListWidget(frame);
        lst->setObjectName(QString::fromUtf8("lst"));
        lst->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(lst);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout_2->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout_2->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);


        horizontalLayout->addWidget(frame);


        retranslateUi(DlgBanketItems);

        QMetaObject::connectSlotsByName(DlgBanketItems);
    } // setupUi

    void retranslateUi(QDialog *DlgBanketItems)
    {
        DlgBanketItems->setWindowTitle(QCoreApplication::translate("DlgBanketItems", "Dialog", nullptr));
        btnOk->setText(QString());
        btnCancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgBanketItems: public Ui_DlgBanketItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGBANKETITEMS_H
