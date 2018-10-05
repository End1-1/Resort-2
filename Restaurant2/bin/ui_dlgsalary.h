/********************************************************************************
** Form generated from reading UI file 'dlgsalary.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSALARY_H
#define UI_DLGSALARY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <eqtablewidget.h>

QT_BEGIN_NAMESPACE

class Ui_DlgSalary
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnAdd;
    QToolButton *btnRemove;
    QToolButton *btnAmount;
    QSpacerItem *horizontalSpacer;
    QDateEdit *deDate;
    QToolButton *btnSave;
    QToolButton *btnClose;
    EQTableWidget *tblData;

    void setupUi(QDialog *DlgSalary)
    {
        if (DlgSalary->objectName().isEmpty())
            DlgSalary->setObjectName(QStringLiteral("DlgSalary"));
        DlgSalary->resize(649, 485);
        verticalLayout = new QVBoxLayout(DlgSalary);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QWidget(DlgSalary);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnAdd = new QToolButton(widget);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));
        btnAdd->setMinimumSize(QSize(50, 50));
        btnAdd->setMaximumSize(QSize(50, 50));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAdd->setIcon(icon);
        btnAdd->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnAdd);

        btnRemove = new QToolButton(widget);
        btnRemove->setObjectName(QStringLiteral("btnRemove"));
        btnRemove->setMinimumSize(QSize(50, 50));
        btnRemove->setMaximumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRemove->setIcon(icon1);
        btnRemove->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnRemove);

        btnAmount = new QToolButton(widget);
        btnAmount->setObjectName(QStringLiteral("btnAmount"));
        btnAmount->setMinimumSize(QSize(50, 50));
        btnAmount->setMaximumSize(QSize(50, 50));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAmount->setIcon(icon2);
        btnAmount->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnAmount);

        horizontalSpacer = new QSpacerItem(386, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        deDate = new QDateEdit(widget);
        deDate->setObjectName(QStringLiteral("deDate"));
        QFont font;
        font.setPointSize(24);
        deDate->setFont(font);
        deDate->setCalendarPopup(true);

        horizontalLayout->addWidget(deDate);

        btnSave = new QToolButton(widget);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setMinimumSize(QSize(50, 50));
        btnSave->setMaximumSize(QSize(50, 50));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon3);
        btnSave->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnSave);

        btnClose = new QToolButton(widget);
        btnClose->setObjectName(QStringLiteral("btnClose"));
        btnClose->setMinimumSize(QSize(50, 50));
        btnClose->setMaximumSize(QSize(50, 50));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon4);
        btnClose->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addWidget(widget);

        tblData = new EQTableWidget(DlgSalary);
        if (tblData->columnCount() < 5)
            tblData->setColumnCount(5);
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
        tblData->setObjectName(QStringLiteral("tblData"));
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblData->setSelectionMode(QAbstractItemView::SingleSelection);
        tblData->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(tblData);


        retranslateUi(DlgSalary);

        QMetaObject::connectSlotsByName(DlgSalary);
    } // setupUi

    void retranslateUi(QDialog *DlgSalary)
    {
        DlgSalary->setWindowTitle(QApplication::translate("DlgSalary", "Dialog", nullptr));
        btnAdd->setText(QApplication::translate("DlgSalary", "...", nullptr));
        btnRemove->setText(QApplication::translate("DlgSalary", "...", nullptr));
        btnAmount->setText(QApplication::translate("DlgSalary", "...", nullptr));
        deDate->setDisplayFormat(QApplication::translate("DlgSalary", "dd.MM.yyyy", nullptr));
        btnSave->setText(QApplication::translate("DlgSalary", "...", nullptr));
        btnClose->setText(QApplication::translate("DlgSalary", "...", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DlgSalary", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DlgSalary", "Employee Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("DlgSalary", "Employee Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("DlgSalary", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("DlgSalary", "Position", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgSalary: public Ui_DlgSalary {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSALARY_H
