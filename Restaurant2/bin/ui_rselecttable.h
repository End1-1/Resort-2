/********************************************************************************
** Form generated from reading UI file 'rselecttable.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RSELECTTABLE_H
#define UI_RSELECTTABLE_H

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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RSelectTable
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tblTables;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnChangeHall;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnReject;

    void setupUi(QDialog *RSelectTable)
    {
        if (RSelectTable->objectName().isEmpty())
            RSelectTable->setObjectName(QStringLiteral("RSelectTable"));
        RSelectTable->resize(684, 480);
        verticalLayout = new QVBoxLayout(RSelectTable);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(RSelectTable);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tblTables = new QTableWidget(frame);
        tblTables->setObjectName(QStringLiteral("tblTables"));
        tblTables->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTables->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTables->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblTables->horizontalHeader()->setVisible(false);
        tblTables->verticalHeader()->setVisible(false);
        tblTables->verticalHeader()->setDefaultSectionSize(60);

        verticalLayout_2->addWidget(tblTables);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, 0, -1);
        btnChangeHall = new QPushButton(widget);
        btnChangeHall->setObjectName(QStringLiteral("btnChangeHall"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/hall.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnChangeHall->setIcon(icon);

        horizontalLayout->addWidget(btnChangeHall);

        horizontalSpacer = new QSpacerItem(573, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnReject = new QPushButton(widget);
        btnReject->setObjectName(QStringLiteral("btnReject"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnReject->setIcon(icon1);

        horizontalLayout->addWidget(btnReject);


        verticalLayout_2->addWidget(widget);


        verticalLayout->addWidget(frame);


        retranslateUi(RSelectTable);

        QMetaObject::connectSlotsByName(RSelectTable);
    } // setupUi

    void retranslateUi(QDialog *RSelectTable)
    {
        RSelectTable->setWindowTitle(QApplication::translate("RSelectTable", "Dialog", nullptr));
        btnChangeHall->setText(QString());
        btnReject->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RSelectTable: public Ui_RSelectTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RSELECTTABLE_H
