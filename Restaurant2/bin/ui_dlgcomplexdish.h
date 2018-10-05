/********************************************************************************
** Form generated from reading UI file 'dlgcomplexdish.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCOMPLEXDISH_H
#define UI_DLGCOMPLEXDISH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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

class Ui_DlgComplexDish
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *lbTitle;
    QTableWidget *tblData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnBack;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSelectAll;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnUp;
    QPushButton *btnDown;

    void setupUi(QDialog *DlgComplexDish)
    {
        if (DlgComplexDish->objectName().isEmpty())
            DlgComplexDish->setObjectName(QStringLiteral("DlgComplexDish"));
        DlgComplexDish->resize(774, 574);
        verticalLayout_2 = new QVBoxLayout(DlgComplexDish);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame = new QFrame(DlgComplexDish);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, 9);
        lbTitle = new QLabel(frame);
        lbTitle->setObjectName(QStringLiteral("lbTitle"));
        QFont font;
        font.setPointSize(24);
        lbTitle->setFont(font);
        lbTitle->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbTitle);

        tblData = new QTableWidget(frame);
        tblData->setObjectName(QStringLiteral("tblData"));
        tblData->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblData->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblData->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblData->setSelectionMode(QAbstractItemView::SingleSelection);
        tblData->horizontalHeader()->setVisible(false);
        tblData->horizontalHeader()->setDefaultSectionSize(200);
        tblData->verticalHeader()->setVisible(false);
        tblData->verticalHeader()->setDefaultSectionSize(60);

        verticalLayout->addWidget(tblData);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnBack = new QPushButton(widget);
        btnBack->setObjectName(QStringLiteral("btnBack"));
        btnBack->setMinimumSize(QSize(50, 50));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBack->setIcon(icon);

        horizontalLayout->addWidget(btnBack);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnSelectAll = new QPushButton(widget);
        btnSelectAll->setObjectName(QStringLiteral("btnSelectAll"));
        btnSelectAll->setMinimumSize(QSize(0, 50));

        horizontalLayout->addWidget(btnSelectAll);

        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setMinimumSize(QSize(0, 50));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 50));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnUp = new QPushButton(widget);
        btnUp->setObjectName(QStringLiteral("btnUp"));
        btnUp->setMinimumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon1);

        horizontalLayout->addWidget(btnUp);

        btnDown = new QPushButton(widget);
        btnDown->setObjectName(QStringLiteral("btnDown"));
        btnDown->setMinimumSize(QSize(50, 50));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDown->setIcon(icon2);

        horizontalLayout->addWidget(btnDown);


        verticalLayout->addWidget(widget);


        verticalLayout_2->addWidget(frame);


        retranslateUi(DlgComplexDish);

        QMetaObject::connectSlotsByName(DlgComplexDish);
    } // setupUi

    void retranslateUi(QDialog *DlgComplexDish)
    {
        DlgComplexDish->setWindowTitle(QApplication::translate("DlgComplexDish", "Dish complex", nullptr));
        lbTitle->setText(QApplication::translate("DlgComplexDish", "Select complex", nullptr));
        btnBack->setText(QString());
        btnSelectAll->setText(QApplication::translate("DlgComplexDish", "Select all", nullptr));
        btnOk->setText(QApplication::translate("DlgComplexDish", "OK", nullptr));
        btnCancel->setText(QApplication::translate("DlgComplexDish", "Cancel", nullptr));
        btnUp->setText(QString());
        btnDown->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgComplexDish: public Ui_DlgComplexDish {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCOMPLEXDISH_H
