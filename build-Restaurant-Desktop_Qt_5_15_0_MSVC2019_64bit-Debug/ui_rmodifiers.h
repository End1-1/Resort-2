/********************************************************************************
** Form generated from reading UI file 'rmodifiers.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RMODIFIERS_H
#define UI_RMODIFIERS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RModifiers
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tblMod;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *RModifiers)
    {
        if (RModifiers->objectName().isEmpty())
            RModifiers->setObjectName(QString::fromUtf8("RModifiers"));
        RModifiers->resize(650, 572);
        verticalLayout = new QVBoxLayout(RModifiers);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(RModifiers);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tblMod = new QTableWidget(frame);
        if (tblMod->columnCount() < 3)
            tblMod->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblMod->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblMod->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblMod->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tblMod->setObjectName(QString::fromUtf8("tblMod"));
        QFont font;
        font.setPointSize(12);
        tblMod->setFont(font);
        tblMod->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblMod->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblMod->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblMod->setSelectionMode(QAbstractItemView::MultiSelection);
        tblMod->horizontalHeader()->setVisible(false);
        tblMod->verticalHeader()->setVisible(false);
        tblMod->verticalHeader()->setDefaultSectionSize(60);

        verticalLayout_2->addWidget(tblMod);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(widget);


        verticalLayout->addWidget(frame);


        retranslateUi(RModifiers);

        QMetaObject::connectSlotsByName(RModifiers);
    } // setupUi

    void retranslateUi(QDialog *RModifiers)
    {
        RModifiers->setWindowTitle(QCoreApplication::translate("RModifiers", "Modifiers", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblMod->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("RModifiers", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblMod->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("RModifiers", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblMod->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("RModifiers", "New Column", nullptr));
        btnOk->setText(QCoreApplication::translate("RModifiers", "OK", nullptr));
        btnCancel->setText(QCoreApplication::translate("RModifiers", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RModifiers: public Ui_RModifiers {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RMODIFIERS_H
