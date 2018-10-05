/********************************************************************************
** Form generated from reading UI file 'dlgbreakfast.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGBREAKFAST_H
#define UI_DLGBREAKFAST_H

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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <eqtablewidget.h>

QT_BEGIN_NAMESPACE

class Ui_DlgBreakfast
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    EQTableWidget *tblData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnSave;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnDown;
    QPushButton *btnUp;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnExit;

    void setupUi(QDialog *DlgBreakfast)
    {
        if (DlgBreakfast->objectName().isEmpty())
            DlgBreakfast->setObjectName(QStringLiteral("DlgBreakfast"));
        DlgBreakfast->resize(1030, 650);
        verticalLayout = new QVBoxLayout(DlgBreakfast);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(DlgBreakfast);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tblData = new EQTableWidget(frame);
        if (tblData->columnCount() < 13)
            tblData->setColumnCount(13);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(12, __qtablewidgetitem12);
        tblData->setObjectName(QStringLiteral("tblData"));
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblData->setSelectionMode(QAbstractItemView::NoSelection);
        tblData->horizontalHeader()->setVisible(false);
        tblData->verticalHeader()->setVisible(false);
        tblData->verticalHeader()->setDefaultSectionSize(50);

        verticalLayout_2->addWidget(tblData);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnSave = new QPushButton(widget);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon);

        horizontalLayout->addWidget(btnSave);

        horizontalSpacer_2 = new QSpacerItem(5555, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnDown = new QPushButton(widget);
        btnDown->setObjectName(QStringLiteral("btnDown"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDown->setIcon(icon1);

        horizontalLayout->addWidget(btnDown);

        btnUp = new QPushButton(widget);
        btnUp->setObjectName(QStringLiteral("btnUp"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon2);

        horizontalLayout->addWidget(btnUp);

        horizontalSpacer = new QSpacerItem(5555, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnExit = new QPushButton(widget);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon3);

        horizontalLayout->addWidget(btnExit);


        verticalLayout_2->addWidget(widget);


        verticalLayout->addWidget(frame);


        retranslateUi(DlgBreakfast);

        QMetaObject::connectSlotsByName(DlgBreakfast);
    } // setupUi

    void retranslateUi(QDialog *DlgBreakfast)
    {
        DlgBreakfast->setWindowTitle(QApplication::translate("DlgBreakfast", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DlgBreakfast", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DlgBreakfast", "room", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("DlgBreakfast", "arrangement", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("DlgBreakfast", "guest", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("DlgBreakfast", "qty", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblData->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("DlgBreakfast", "show", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblData->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("DlgBreakfast", "price", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblData->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("DlgBreakfast", "total", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblData->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("DlgBreakfast", "+", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tblData->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("DlgBreakfast", "-", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tblData->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("DlgBreakfast", "print", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tblData->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QApplication::translate("DlgBreakfast", "invoice", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tblData->horizontalHeaderItem(12);
        ___qtablewidgetitem12->setText(QApplication::translate("DlgBreakfast", "fdoc", nullptr));
        btnSave->setText(QApplication::translate("DlgBreakfast", "Save all", nullptr));
        btnDown->setText(QString());
        btnUp->setText(QString());
        btnExit->setText(QApplication::translate("DlgBreakfast", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgBreakfast: public Ui_DlgBreakfast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGBREAKFAST_H
