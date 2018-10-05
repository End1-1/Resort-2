/********************************************************************************
** Form generated from reading UI file 'dlgbanketitems.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGBANKETITEMS_H
#define UI_DLGBANKETITEMS_H

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
            DlgBanketItems->setObjectName(QStringLiteral("DlgBanketItems"));
        DlgBanketItems->resize(522, 544);
        horizontalLayout = new QHBoxLayout(DlgBanketItems);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(DlgBanketItems);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lst = new QListWidget(frame);
        lst->setObjectName(QStringLiteral("lst"));
        lst->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(lst);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));

        horizontalLayout_2->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

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
        DlgBanketItems->setWindowTitle(QApplication::translate("DlgBanketItems", "Dialog", nullptr));
        btnOk->setText(QString());
        btnCancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgBanketItems: public Ui_DlgBanketItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGBANKETITEMS_H
