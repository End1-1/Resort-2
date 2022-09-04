/********************************************************************************
** Form generated from reading UI file 'dlgsalary.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSALARY_H
#define UI_DLGSALARY_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
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
    QToolButton *btnCopyFromReal;
    QSpacerItem *horizontalSpacer;
    QDateEdit *deDate;
    QToolButton *btnSave;
    QToolButton *btnClose;
    EQTableWidget *tblData;

    void setupUi(QDialog *DlgSalary)
    {
        if (DlgSalary->objectName().isEmpty())
            DlgSalary->setObjectName(QString::fromUtf8("DlgSalary"));
        DlgSalary->resize(649, 485);
        verticalLayout = new QVBoxLayout(DlgSalary);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(DlgSalary);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAdd = new QToolButton(widget);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setMinimumSize(QSize(50, 50));
        btnAdd->setMaximumSize(QSize(50, 50));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAdd->setIcon(icon);
        btnAdd->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnAdd);

        btnRemove = new QToolButton(widget);
        btnRemove->setObjectName(QString::fromUtf8("btnRemove"));
        btnRemove->setMinimumSize(QSize(50, 50));
        btnRemove->setMaximumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRemove->setIcon(icon1);
        btnRemove->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnRemove);

        btnAmount = new QToolButton(widget);
        btnAmount->setObjectName(QString::fromUtf8("btnAmount"));
        btnAmount->setMinimumSize(QSize(50, 50));
        btnAmount->setMaximumSize(QSize(50, 50));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAmount->setIcon(icon2);
        btnAmount->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnAmount);

        btnCopyFromReal = new QToolButton(widget);
        btnCopyFromReal->setObjectName(QString::fromUtf8("btnCopyFromReal"));
        btnCopyFromReal->setMinimumSize(QSize(50, 50));
        btnCopyFromReal->setMaximumSize(QSize(50, 50));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCopyFromReal->setIcon(icon3);
        btnCopyFromReal->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCopyFromReal);

        horizontalSpacer = new QSpacerItem(386, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        deDate = new QDateEdit(widget);
        deDate->setObjectName(QString::fromUtf8("deDate"));
        QFont font;
        font.setPointSize(24);
        deDate->setFont(font);
        deDate->setCalendarPopup(true);

        horizontalLayout->addWidget(deDate);

        btnSave = new QToolButton(widget);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMinimumSize(QSize(50, 50));
        btnSave->setMaximumSize(QSize(50, 50));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon4);
        btnSave->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnSave);

        btnClose = new QToolButton(widget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setMinimumSize(QSize(50, 50));
        btnClose->setMaximumSize(QSize(50, 50));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon5);
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
        tblData->setObjectName(QString::fromUtf8("tblData"));
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblData->setSelectionMode(QAbstractItemView::SingleSelection);
        tblData->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(tblData);


        retranslateUi(DlgSalary);

        QMetaObject::connectSlotsByName(DlgSalary);
    } // setupUi

    void retranslateUi(QDialog *DlgSalary)
    {
        DlgSalary->setWindowTitle(QCoreApplication::translate("DlgSalary", "Dialog", nullptr));
        btnAdd->setText(QCoreApplication::translate("DlgSalary", "...", nullptr));
        btnRemove->setText(QCoreApplication::translate("DlgSalary", "...", nullptr));
        btnAmount->setText(QCoreApplication::translate("DlgSalary", "...", nullptr));
        btnCopyFromReal->setText(QCoreApplication::translate("DlgSalary", "...", nullptr));
        deDate->setDisplayFormat(QCoreApplication::translate("DlgSalary", "dd.MM.yyyy", nullptr));
        btnSave->setText(QCoreApplication::translate("DlgSalary", "...", nullptr));
        btnClose->setText(QCoreApplication::translate("DlgSalary", "...", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("DlgSalary", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("DlgSalary", "Employee Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("DlgSalary", "Employee Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("DlgSalary", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("DlgSalary", "Position", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgSalary: public Ui_DlgSalary {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSALARY_H
