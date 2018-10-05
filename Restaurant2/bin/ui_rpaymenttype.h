/********************************************************************************
** Form generated from reading UI file 'rpaymenttype.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RPAYMENTTYPE_H
#define UI_RPAYMENTTYPE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            RPaymentType->setObjectName(QStringLiteral("RPaymentType"));
        RPaymentType->resize(451, 544);
        verticalLayout = new QVBoxLayout(RPaymentType);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(RPaymentType);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lstData = new QListWidget(frame);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/cash.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(lstData);
        __qlistwidgetitem->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/visa.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(lstData);
        __qlistwidgetitem1->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/maestro.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(lstData);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/mastercard.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(lstData);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/arca.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(lstData);
        __qlistwidgetitem4->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/american-express.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(lstData);
        __qlistwidgetitem5->setIcon(icon5);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/discount.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem6 = new QListWidgetItem(lstData);
        __qlistwidgetitem6->setIcon(icon6);
        lstData->setObjectName(QStringLiteral("lstData"));
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
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(85, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

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
        RPaymentType->setWindowTitle(QApplication::translate("RPaymentType", "Dialog", nullptr));

        const bool __sortingEnabled = lstData->isSortingEnabled();
        lstData->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = lstData->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("RPaymentType", "Cash", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = lstData->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("RPaymentType", "VISA", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = lstData->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("RPaymentType", "Maestro", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = lstData->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("RPaymentType", "Mastercard", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = lstData->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("RPaymentType", "ArCa", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = lstData->item(5);
        ___qlistwidgetitem5->setText(QApplication::translate("RPaymentType", "AmEx", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = lstData->item(6);
        ___qlistwidgetitem6->setText(QApplication::translate("RPaymentType", "Coupon", nullptr));
        lstData->setSortingEnabled(__sortingEnabled);

        btnOk->setText(QApplication::translate("RPaymentType", "Ok", nullptr));
        btnCancel->setText(QApplication::translate("RPaymentType", "No", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RPaymentType: public Ui_RPaymentType {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RPAYMENTTYPE_H
