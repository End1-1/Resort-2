/********************************************************************************
** Form generated from reading UI file 'dlgorders.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGORDERS_H
#define UI_DLGORDERS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgOrders
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tblData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnDown;
    QPushButton *btnUp;
    QPushButton *btnByNumber;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *btnPrintTax;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnCancel;

    void setupUi(QDialog *DlgOrders)
    {
        if (DlgOrders->objectName().isEmpty())
            DlgOrders->setObjectName(QString::fromUtf8("DlgOrders"));
        DlgOrders->resize(993, 713);
        verticalLayout = new QVBoxLayout(DlgOrders);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tblData = new QTableWidget(DlgOrders);
        if (tblData->columnCount() < 8)
            tblData->setColumnCount(8);
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
        tblData->setObjectName(QString::fromUtf8("tblData"));
        tblData->setSelectionMode(QAbstractItemView::SingleSelection);
        tblData->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(tblData);

        widget = new QWidget(DlgOrders);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, 0, -1);
        btnDown = new QPushButton(widget);
        btnDown->setObjectName(QString::fromUtf8("btnDown"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDown->setIcon(icon);

        horizontalLayout->addWidget(btnDown);

        btnUp = new QPushButton(widget);
        btnUp->setObjectName(QString::fromUtf8("btnUp"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon1);

        horizontalLayout->addWidget(btnUp);

        btnByNumber = new QPushButton(widget);
        btnByNumber->setObjectName(QString::fromUtf8("btnByNumber"));

        horizontalLayout->addWidget(btnByNumber);

        horizontalSpacer_2 = new QSpacerItem(238, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnPrintTax = new QToolButton(widget);
        btnPrintTax->setObjectName(QString::fromUtf8("btnPrintTax"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/tax.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrintTax->setIcon(icon2);
        btnPrintTax->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnPrintTax);

        horizontalSpacer = new QSpacerItem(238, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addWidget(widget);


        retranslateUi(DlgOrders);

        QMetaObject::connectSlotsByName(DlgOrders);
    } // setupUi

    void retranslateUi(QDialog *DlgOrders)
    {
        DlgOrders->setWindowTitle(QCoreApplication::translate("DlgOrders", "Print Tax", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("DlgOrders", "Order", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("DlgOrders", "Hall", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("DlgOrders", "Table", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("DlgOrders", "Staff", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("DlgOrders", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblData->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("DlgOrders", "Tax", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblData->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("DlgOrders", "Payment", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblData->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("DlgOrders", "Item", nullptr));
        btnDown->setText(QString());
        btnUp->setText(QString());
        btnByNumber->setText(QCoreApplication::translate("DlgOrders", "Order id", nullptr));
        btnPrintTax->setText(QCoreApplication::translate("DlgOrders", "Print Tax", nullptr));
        btnCancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgOrders: public Ui_DlgOrders {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGORDERS_H
