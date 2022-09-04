/********************************************************************************
** Form generated from reading UI file 'dlgreservation.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGRESERVATION_H
#define UI_DLGRESERVATION_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <rkeyboard.h>

QT_BEGIN_NAMESPACE

class Ui_DlgReservation
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *leHint;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *leSearch;
    QPushButton *btnClear;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tblItem;
    QTableWidget *tblData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    RKeyboard *kbd;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *DlgReservation)
    {
        if (DlgReservation->objectName().isEmpty())
            DlgReservation->setObjectName(QString::fromUtf8("DlgReservation"));
        DlgReservation->resize(1134, 614);
        verticalLayout = new QVBoxLayout(DlgReservation);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(DlgReservation);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        leHint = new QLineEdit(frame);
        leHint->setObjectName(QString::fromUtf8("leHint"));
        QFont font;
        font.setPointSize(16);
        leHint->setFont(font);
        leHint->setFocusPolicy(Qt::NoFocus);
        leHint->setStyleSheet(QString::fromUtf8("background:rgb(0, 0, 127);\n"
"color:rgb(255, 255, 255)\n"
""));
        leHint->setAlignment(Qt::AlignCenter);
        leHint->setReadOnly(true);

        verticalLayout_2->addWidget(leHint);

        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        leSearch = new QLineEdit(widget_2);
        leSearch->setObjectName(QString::fromUtf8("leSearch"));
        leSearch->setMinimumSize(QSize(200, 0));
        leSearch->setFont(font);

        horizontalLayout->addWidget(leSearch);

        btnClear = new QPushButton(widget_2);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/eraser.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClear->setIcon(icon);

        horizontalLayout->addWidget(btnClear);

        horizontalSpacer = new QSpacerItem(134, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(widget_2);

        tblItem = new QTableWidget(frame);
        tblItem->setObjectName(QString::fromUtf8("tblItem"));
        tblItem->setEnabled(false);
        tblItem->setMaximumSize(QSize(16777215, 45));
        QFont font1;
        font1.setPointSize(12);
        tblItem->setFont(font1);
        tblItem->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblItem->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblItem->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblItem->setSelectionMode(QAbstractItemView::SingleSelection);
        tblItem->horizontalHeader()->setVisible(false);
        tblItem->verticalHeader()->setVisible(false);
        tblItem->verticalHeader()->setDefaultSectionSize(40);

        verticalLayout_2->addWidget(tblItem);

        tblData = new QTableWidget(frame);
        tblData->setObjectName(QString::fromUtf8("tblData"));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        tblData->setFont(font2);
        tblData->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblData->setSelectionMode(QAbstractItemView::SingleSelection);
        tblData->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblData->verticalHeader()->setVisible(false);
        tblData->verticalHeader()->setDefaultSectionSize(50);

        verticalLayout_2->addWidget(tblData);


        verticalLayout->addWidget(frame);

        widget = new QWidget(DlgReservation);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 50));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(535, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        kbd = new RKeyboard(widget);
        kbd->setObjectName(QString::fromUtf8("kbd"));

        horizontalLayout_2->addWidget(kbd);

        horizontalSpacer_3 = new QSpacerItem(535, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(widget);


        retranslateUi(DlgReservation);

        QMetaObject::connectSlotsByName(DlgReservation);
    } // setupUi

    void retranslateUi(QDialog *DlgReservation)
    {
        DlgReservation->setWindowTitle(QCoreApplication::translate("DlgReservation", "Dialog", nullptr));
        leHint->setText(QCoreApplication::translate("DlgReservation", "!", nullptr));
        btnClear->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgReservation: public Ui_DlgReservation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGRESERVATION_H
