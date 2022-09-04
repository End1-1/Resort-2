/********************************************************************************
** Form generated from reading UI file 'dlgvoidback.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGVOIDBACK_H
#define UI_DLGVOIDBACK_H

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

class Ui_DlgVoidBack
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tblData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnDown;
    QPushButton *btnUp;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *btnRecover;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnCancel;

    void setupUi(QDialog *DlgVoidBack)
    {
        if (DlgVoidBack->objectName().isEmpty())
            DlgVoidBack->setObjectName(QString::fromUtf8("DlgVoidBack"));
        DlgVoidBack->resize(689, 604);
        verticalLayout = new QVBoxLayout(DlgVoidBack);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tblData = new QTableWidget(DlgVoidBack);
        if (tblData->columnCount() < 9)
            tblData->setColumnCount(9);
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
        tblData->setObjectName(QString::fromUtf8("tblData"));
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblData->setSelectionMode(QAbstractItemView::SingleSelection);
        tblData->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblData->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tblData);

        widget = new QWidget(DlgVoidBack);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
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

        horizontalSpacer_2 = new QSpacerItem(238, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnRecover = new QToolButton(widget);
        btnRecover->setObjectName(QString::fromUtf8("btnRecover"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/recycle.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRecover->setIcon(icon2);
        btnRecover->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(btnRecover);

        horizontalSpacer = new QSpacerItem(238, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addWidget(widget);


        retranslateUi(DlgVoidBack);

        QMetaObject::connectSlotsByName(DlgVoidBack);
    } // setupUi

    void retranslateUi(QDialog *DlgVoidBack)
    {
        DlgVoidBack->setWindowTitle(QCoreApplication::translate("DlgVoidBack", "Void back", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("DlgVoidBack", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("DlgVoidBack", "REC", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("DlgVoidBack", "ORDER", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("DlgVoidBack", "HALL", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("DlgVoidBack", "TABLE", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblData->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("DlgVoidBack", "STAFF", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblData->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("DlgVoidBack", "ITEM", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblData->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("DlgVoidBack", "QTY", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblData->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("DlgVoidBack", "AMOUNT", nullptr));
        btnDown->setText(QString());
        btnUp->setText(QString());
        btnRecover->setText(QCoreApplication::translate("DlgVoidBack", "Recover", nullptr));
        btnCancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgVoidBack: public Ui_DlgVoidBack {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGVOIDBACK_H
