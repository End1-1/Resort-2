/********************************************************************************
** Form generated from reading UI file 'rchangehall.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCHANGEHALL_H
#define UI_RCHANGEHALL_H

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

class Ui_RChangeHall
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QTableWidget *tblHall;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnCancel;

    void setupUi(QDialog *RChangeHall)
    {
        if (RChangeHall->objectName().isEmpty())
            RChangeHall->setObjectName(QStringLiteral("RChangeHall"));
        RChangeHall->resize(535, 509);
        horizontalLayout = new QHBoxLayout(RChangeHall);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(RChangeHall);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tblHall = new QTableWidget(frame);
        tblHall->setObjectName(QStringLiteral("tblHall"));
        QFont font;
        font.setPointSize(16);
        tblHall->setFont(font);
        tblHall->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblHall->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblHall->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblHall->setSelectionMode(QAbstractItemView::NoSelection);
        tblHall->setTextElideMode(Qt::ElideMiddle);
        tblHall->setCornerButtonEnabled(false);
        tblHall->horizontalHeader()->setVisible(false);
        tblHall->verticalHeader()->setVisible(false);
        tblHall->verticalHeader()->setDefaultSectionSize(60);

        verticalLayout->addWidget(tblHall);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, 0, -1);
        horizontalSpacer = new QSpacerItem(454, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

        horizontalLayout_2->addWidget(btnCancel);


        verticalLayout->addWidget(widget);


        horizontalLayout->addWidget(frame);


        retranslateUi(RChangeHall);

        QMetaObject::connectSlotsByName(RChangeHall);
    } // setupUi

    void retranslateUi(QDialog *RChangeHall)
    {
        RChangeHall->setWindowTitle(QApplication::translate("RChangeHall", "Dialog", nullptr));
        btnCancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RChangeHall: public Ui_RChangeHall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCHANGEHALL_H
