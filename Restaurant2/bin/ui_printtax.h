/********************************************************************************
** Form generated from reading UI file 'printtax.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTTAX_H
#define UI_PRINTTAX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <eqcheckbox.h>
#include <eqlineedit.h>
#include <eqtablewidget.h>

QT_BEGIN_NAMESPACE

class Ui_PrintTax
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    EQLineEdit *leAvailablePrepaid;
    QSpacerItem *horizontalSpacer_4;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tblFilter;
    QPushButton *btnClearFilter;
    QSpacerItem *horizontalSpacer_3;
    EQCheckBox *chCheckUncheckAll;
    EQTableWidget *tblData;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    EQLineEdit *leCard;
    EQLineEdit *leTotal;
    EQLineEdit *leCash;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    EQLineEdit *lePre;
    QLabel *label_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnPrint;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *PrintTax)
    {
        if (PrintTax->objectName().isEmpty())
            PrintTax->setObjectName(QStringLiteral("PrintTax"));
        PrintTax->resize(524, 540);
        verticalLayout = new QVBoxLayout(PrintTax);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_3 = new QWidget(PrintTax);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_5 = new QLabel(widget_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_2->addWidget(label_5);

        leAvailablePrepaid = new EQLineEdit(widget_3);
        leAvailablePrepaid->setObjectName(QStringLiteral("leAvailablePrepaid"));
        leAvailablePrepaid->setStyleSheet(QLatin1String("font-size:12pt;\n"
"font-weight:bold;\n"
"color:red;"));
        leAvailablePrepaid->setReadOnly(true);

        horizontalLayout_2->addWidget(leAvailablePrepaid);

        horizontalSpacer_4 = new QSpacerItem(174, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(widget_3);

        widget_4 = new QWidget(PrintTax);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        tblFilter = new QTableWidget(widget_4);
        if (tblFilter->columnCount() < 7)
            tblFilter->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblFilter->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (tblFilter->rowCount() < 1)
            tblFilter->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblFilter->setVerticalHeaderItem(0, __qtablewidgetitem7);
        tblFilter->setObjectName(QStringLiteral("tblFilter"));
        tblFilter->setMaximumSize(QSize(16777215, 30));
        tblFilter->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblFilter->setSelectionMode(QAbstractItemView::SingleSelection);
        tblFilter->horizontalHeader()->setVisible(false);
        tblFilter->horizontalHeader()->setDefaultSectionSize(30);
        tblFilter->verticalHeader()->setVisible(false);

        horizontalLayout_3->addWidget(tblFilter);

        btnClearFilter = new QPushButton(widget_4);
        btnClearFilter->setObjectName(QStringLiteral("btnClearFilter"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/remove_filter.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClearFilter->setIcon(icon);

        horizontalLayout_3->addWidget(btnClearFilter);

        horizontalSpacer_3 = new QSpacerItem(92, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        chCheckUncheckAll = new EQCheckBox(widget_4);
        chCheckUncheckAll->setObjectName(QStringLiteral("chCheckUncheckAll"));

        horizontalLayout_3->addWidget(chCheckUncheckAll);


        verticalLayout->addWidget(widget_4);

        tblData = new EQTableWidget(PrintTax);
        if (tblData->columnCount() < 8)
            tblData->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(4, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(5, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(6, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tblData->setHorizontalHeaderItem(7, __qtablewidgetitem15);
        tblData->setObjectName(QStringLiteral("tblData"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(4);
        sizePolicy1.setHeightForWidth(tblData->sizePolicy().hasHeightForWidth());
        tblData->setSizePolicy(sizePolicy1);
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(tblData);

        widget_2 = new QWidget(PrintTax);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        leCard = new EQLineEdit(widget_2);
        leCard->setObjectName(QStringLiteral("leCard"));

        gridLayout->addWidget(leCard, 1, 1, 1, 1);

        leTotal = new EQLineEdit(widget_2);
        leTotal->setObjectName(QStringLiteral("leTotal"));
        leTotal->setReadOnly(true);

        gridLayout->addWidget(leTotal, 3, 1, 1, 1);

        leCash = new EQLineEdit(widget_2);
        leCash->setObjectName(QStringLiteral("leCash"));

        gridLayout->addWidget(leCash, 1, 0, 1, 1);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 1, 1, 1);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 2, 1, 1, 1);

        lePre = new EQLineEdit(widget_2);
        lePre->setObjectName(QStringLiteral("lePre"));
        lePre->setStyleSheet(QLatin1String("font-size:12pt;\n"
"font-weight:bold;\n"
"color:red;"));
        lePre->setReadOnly(false);

        gridLayout->addWidget(lePre, 3, 0, 1, 1);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(PrintTax);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnPrint = new QPushButton(widget);
        btnPrint->setObjectName(QStringLiteral("btnPrint"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(10);
        sizePolicy2.setHeightForWidth(btnPrint->sizePolicy().hasHeightForWidth());
        btnPrint->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(btnPrint);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        sizePolicy2.setHeightForWidth(btnCancel->sizePolicy().hasHeightForWidth());
        btnCancel->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);


        retranslateUi(PrintTax);

        QMetaObject::connectSlotsByName(PrintTax);
    } // setupUi

    void retranslateUi(QDialog *PrintTax)
    {
        PrintTax->setWindowTitle(QApplication::translate("PrintTax", "Print tax", nullptr));
        label_5->setText(QApplication::translate("PrintTax", "Available prepaid amount", nullptr));
        leAvailablePrepaid->setText(QApplication::translate("PrintTax", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblFilter->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblFilter->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblFilter->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblFilter->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblFilter->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblFilter->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblFilter->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("PrintTax", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblFilter->verticalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("PrintTax", "New Row", nullptr));
        btnClearFilter->setText(QString());
        chCheckUncheckAll->setText(QApplication::translate("PrintTax", "Check/Uncheck all", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblData->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QApplication::translate("PrintTax", "RecId", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tblData->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QApplication::translate("PrintTax", "Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tblData->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QApplication::translate("PrintTax", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tblData->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QApplication::translate("PrintTax", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tblData->horizontalHeaderItem(4);
        ___qtablewidgetitem12->setText(QApplication::translate("PrintTax", "Print", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tblData->horizontalHeaderItem(5);
        ___qtablewidgetitem13->setText(QApplication::translate("PrintTax", "Adgt", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tblData->horizontalHeaderItem(6);
        ___qtablewidgetitem14->setText(QApplication::translate("PrintTax", "Source", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tblData->horizontalHeaderItem(7);
        ___qtablewidgetitem15->setText(QApplication::translate("PrintTax", "qty", nullptr));
        leCard->setInputMask(QString());
        leCash->setInputMask(QString());
        label_3->setText(QApplication::translate("PrintTax", "Card", nullptr));
        label_2->setText(QApplication::translate("PrintTax", "Cash", nullptr));
        label->setText(QApplication::translate("PrintTax", "Total", nullptr));
        lePre->setInputMask(QString());
        lePre->setText(QApplication::translate("PrintTax", "0", nullptr));
        label_4->setText(QApplication::translate("PrintTax", "Prepaid", nullptr));
        btnPrint->setText(QApplication::translate("PrintTax", "Print", nullptr));
        btnCancel->setText(QApplication::translate("PrintTax", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintTax: public Ui_PrintTax {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTTAX_H
