/********************************************************************************
** Form generated from reading UI file 'dlgdeptholder.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGDEPTHOLDER_H
#define UI_DLGDEPTHOLDER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <eqlineedit.h>
#include <rkeyboard.h>

QT_BEGIN_NAMESPACE

class Ui_DlgDeptHolder
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    RKeyboard *wKbd;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnCreateNew;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_2;
    EQLineEdit *leName;
    QLabel *label_2;
    EQLineEdit *leInfo;
    QLabel *label;
    QListWidget *lstHolders;

    void setupUi(QDialog *DlgDeptHolder)
    {
        if (DlgDeptHolder->objectName().isEmpty())
            DlgDeptHolder->setObjectName(QString::fromUtf8("DlgDeptHolder"));
        DlgDeptHolder->resize(937, 498);
        horizontalLayout_2 = new QHBoxLayout(DlgDeptHolder);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame = new QFrame(DlgDeptHolder);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        wKbd = new RKeyboard(frame);
        wKbd->setObjectName(QString::fromUtf8("wKbd"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(wKbd->sizePolicy().hasHeightForWidth());
        wKbd->setSizePolicy(sizePolicy);

        gridLayout->addWidget(wKbd, 3, 0, 1, 2);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnSave = new QPushButton(widget);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMinimumSize(QSize(150, 50));
        btnSave->setMaximumSize(QSize(16777215, 222));
        btnSave->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon);
        btnSave->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnSave);

        btnCreateNew = new QPushButton(widget);
        btnCreateNew->setObjectName(QString::fromUtf8("btnCreateNew"));
        btnCreateNew->setMinimumSize(QSize(150, 50));
        btnCreateNew->setMaximumSize(QSize(16777215, 222));
        btnCreateNew->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/revive.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCreateNew->setIcon(icon1);
        btnCreateNew->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCreateNew);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setMinimumSize(QSize(150, 50));
        btnCancel->setMaximumSize(QSize(16777215, 222));
        btnCancel->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCancel->setIcon(icon2);
        btnCancel->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addWidget(widget, 4, 0, 1, 2);

        leName = new EQLineEdit(frame);
        leName->setObjectName(QString::fromUtf8("leName"));

        gridLayout->addWidget(leName, 0, 1, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        leInfo = new EQLineEdit(frame);
        leInfo->setObjectName(QString::fromUtf8("leInfo"));

        gridLayout->addWidget(leInfo, 1, 1, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lstHolders = new QListWidget(frame);
        lstHolders->setObjectName(QString::fromUtf8("lstHolders"));
        lstHolders->setMinimumSize(QSize(0, 200));
        lstHolders->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(lstHolders, 2, 0, 1, 2);


        horizontalLayout_2->addWidget(frame);


        retranslateUi(DlgDeptHolder);

        QMetaObject::connectSlotsByName(DlgDeptHolder);
    } // setupUi

    void retranslateUi(QDialog *DlgDeptHolder)
    {
        DlgDeptHolder->setWindowTitle(QCoreApplication::translate("DlgDeptHolder", "Dialog", nullptr));
        btnSave->setText(QCoreApplication::translate("DlgDeptHolder", "Select", nullptr));
        btnCreateNew->setText(QCoreApplication::translate("DlgDeptHolder", "Create new", nullptr));
        btnCancel->setText(QCoreApplication::translate("DlgDeptHolder", "Cancel", nullptr));
        label_2->setText(QCoreApplication::translate("DlgDeptHolder", "Info", nullptr));
        label->setText(QCoreApplication::translate("DlgDeptHolder", "Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgDeptHolder: public Ui_DlgDeptHolder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGDEPTHOLDER_H
