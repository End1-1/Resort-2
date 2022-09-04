/********************************************************************************
** Form generated from reading UI file 'rpaymenttype.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RPAYMENTTYPE_H
#define UI_RPAYMENTTYPE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RPaymentType
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QListWidget *lstData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *RPaymentType)
    {
        if (RPaymentType->objectName().isEmpty())
            RPaymentType->setObjectName(QString::fromUtf8("RPaymentType"));
        RPaymentType->resize(451, 544);
        verticalLayout = new QVBoxLayout(RPaymentType);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(RPaymentType);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lstData = new QListWidget(frame);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(lstData);
        __qlistwidgetitem->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/visa.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(lstData);
        __qlistwidgetitem1->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/maestro.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(lstData);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/mastercard.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(lstData);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/arca.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(lstData);
        __qlistwidgetitem4->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/american-express.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(lstData);
        __qlistwidgetitem5->setIcon(icon5);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/discount.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem6 = new QListWidgetItem(lstData);
        __qlistwidgetitem6->setIcon(icon6);
        lstData->setObjectName(QString::fromUtf8("lstData"));
        QFont font;
        font.setPointSize(16);
        lstData->setFont(font);
        lstData->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        lstData->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        lstData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lstData->setIconSize(QSize(32, 32));
        lstData->setGridSize(QSize(0, 50));
        lstData->setViewMode(QListView::ListMode);

        verticalLayout_2->addWidget(lstData);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(85, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(85, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(widget);


        verticalLayout->addWidget(frame);


        retranslateUi(RPaymentType);

        lstData->setCurrentRow(-1);


        QMetaObject::connectSlotsByName(RPaymentType);
    } // setupUi

    void retranslateUi(QDialog *RPaymentType)
    {
        RPaymentType->setWindowTitle(QCoreApplication::translate("RPaymentType", "Dialog", nullptr));

        const bool __sortingEnabled = lstData->isSortingEnabled();
        lstData->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = lstData->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("RPaymentType", "Cash", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = lstData->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("RPaymentType", "VISA", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = lstData->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("RPaymentType", "Maestro", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = lstData->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("RPaymentType", "Mastercard", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = lstData->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("RPaymentType", "ArCa", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = lstData->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("RPaymentType", "AmEx", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = lstData->item(6);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("RPaymentType", "Coupon", nullptr));
        lstData->setSortingEnabled(__sortingEnabled);

        btnOk->setText(QCoreApplication::translate("RPaymentType", "Ok", nullptr));
        btnCancel->setText(QCoreApplication::translate("RPaymentType", "No", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RPaymentType: public Ui_RPaymentType {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RPAYMENTTYPE_H
