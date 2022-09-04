/********************************************************************************
** Form generated from reading UI file 'rface.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RFACE_H
#define UI_RFACE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RFace
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnTools;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QLabel *lbWorkingDate;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnBanket;
    QPushButton *btnBreakFast;
    QPushButton *btnChangeHall;
    QTableWidget *tblTables;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnUp;
    QPushButton *btnDown;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnExit;

    void setupUi(QDialog *RFace)
    {
        if (RFace->objectName().isEmpty())
            RFace->setObjectName(QString::fromUtf8("RFace"));
        RFace->resize(756, 506);
        verticalLayout_2 = new QVBoxLayout(RFace);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(RFace);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnTools = new QPushButton(frame);
        btnTools->setObjectName(QString::fromUtf8("btnTools"));
        btnTools->setMinimumSize(QSize(0, 0));
        btnTools->setMaximumSize(QSize(555, 555));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTools->setIcon(icon);

        horizontalLayout->addWidget(btnTools);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lbWorkingDate = new QLabel(widget);
        lbWorkingDate->setObjectName(QString::fromUtf8("lbWorkingDate"));

        verticalLayout_3->addWidget(lbWorkingDate);


        horizontalLayout->addWidget(widget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnBanket = new QPushButton(frame);
        btnBanket->setObjectName(QString::fromUtf8("btnBanket"));
        btnBanket->setMinimumSize(QSize(0, 0));
        btnBanket->setMaximumSize(QSize(555, 555));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/banket.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBanket->setIcon(icon1);

        horizontalLayout->addWidget(btnBanket);

        btnBreakFast = new QPushButton(frame);
        btnBreakFast->setObjectName(QString::fromUtf8("btnBreakFast"));
        btnBreakFast->setMaximumSize(QSize(555, 555));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/dinner.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBreakFast->setIcon(icon2);

        horizontalLayout->addWidget(btnBreakFast);

        btnChangeHall = new QPushButton(frame);
        btnChangeHall->setObjectName(QString::fromUtf8("btnChangeHall"));
        btnChangeHall->setMaximumSize(QSize(548, 555));
        btnChangeHall->setFocusPolicy(Qt::NoFocus);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/hall.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnChangeHall->setIcon(icon3);

        horizontalLayout->addWidget(btnChangeHall);


        verticalLayout->addLayout(horizontalLayout);

        tblTables = new QTableWidget(frame);
        tblTables->setObjectName(QString::fromUtf8("tblTables"));
        tblTables->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTables->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblTables->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblTables->setSelectionMode(QAbstractItemView::SingleSelection);
        tblTables->setShowGrid(false);
        tblTables->horizontalHeader()->setVisible(false);
        tblTables->verticalHeader()->setVisible(false);
        tblTables->verticalHeader()->setDefaultSectionSize(60);

        verticalLayout->addWidget(tblTables);

        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        btnUp = new QPushButton(widget_2);
        btnUp->setObjectName(QString::fromUtf8("btnUp"));
        btnUp->setMinimumSize(QSize(50, 50));
        btnUp->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon4);
        btnUp->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnUp);

        btnDown = new QPushButton(widget_2);
        btnDown->setObjectName(QString::fromUtf8("btnDown"));
        btnDown->setMinimumSize(QSize(50, 50));
        btnDown->setFocusPolicy(Qt::NoFocus);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDown->setIcon(icon5);
        btnDown->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnDown);

        horizontalSpacer_3 = new QSpacerItem(607, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        btnExit = new QPushButton(widget_2);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        btnExit->setMinimumSize(QSize(50, 50));
        btnExit->setMaximumSize(QSize(100, 16777215));
        btnExit->setFocusPolicy(Qt::NoFocus);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon6);
        btnExit->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btnExit);


        verticalLayout->addWidget(widget_2);


        verticalLayout_2->addWidget(frame);


        retranslateUi(RFace);

        QMetaObject::connectSlotsByName(RFace);
    } // setupUi

    void retranslateUi(QDialog *RFace)
    {
        RFace->setWindowTitle(QCoreApplication::translate("RFace", "RFace", nullptr));
        btnTools->setText(QCoreApplication::translate("RFace", "TOOLS", nullptr));
        lbWorkingDate->setText(QCoreApplication::translate("RFace", "Working date\n"
"01/06/2017", nullptr));
        btnBanket->setText(QCoreApplication::translate("RFace", "BANQUET", nullptr));
        btnBreakFast->setText(QCoreApplication::translate("RFace", "BREAKFAST", nullptr));
        btnChangeHall->setText(QCoreApplication::translate("RFace", "HALL", nullptr));
        btnUp->setText(QString());
        btnDown->setText(QString());
        btnExit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RFace: public Ui_RFace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RFACE_H
