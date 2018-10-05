/********************************************************************************
** Form generated from reading UI file 'rnumbers.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RNUMBERS_H
#define UI_RNUMBERS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RNumbers
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *btn2;
    QPushButton *btn8;
    QPushButton *btn3;
    QPushButton *btn0;
    QPushButton *btn9;
    QPushButton *btnAccept;
    QPushButton *btn6;
    QPushButton *btnReject;
    QPushButton *btn4;
    QPushButton *btn1;
    QPushButton *btn7;
    QPushButton *btn5;
    QPushButton *btnClear;
    QLineEdit *leText;

    void setupUi(QDialog *RNumbers)
    {
        if (RNumbers->objectName().isEmpty())
            RNumbers->setObjectName(QStringLiteral("RNumbers"));
        RNumbers->resize(232, 364);
        horizontalLayout = new QHBoxLayout(RNumbers);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(RNumbers);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btn2 = new QPushButton(frame);
        btn2->setObjectName(QStringLiteral("btn2"));
        btn2->setMinimumSize(QSize(60, 60));
        btn2->setMaximumSize(QSize(60, 60));
        btn2->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn2, 1, 1, 1, 1);

        btn8 = new QPushButton(frame);
        btn8->setObjectName(QStringLiteral("btn8"));
        btn8->setMinimumSize(QSize(60, 60));
        btn8->setMaximumSize(QSize(60, 60));
        btn8->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn8, 3, 1, 1, 1);

        btn3 = new QPushButton(frame);
        btn3->setObjectName(QStringLiteral("btn3"));
        btn3->setMinimumSize(QSize(60, 60));
        btn3->setMaximumSize(QSize(60, 60));
        btn3->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn3, 1, 2, 1, 1);

        btn0 = new QPushButton(frame);
        btn0->setObjectName(QStringLiteral("btn0"));
        btn0->setMinimumSize(QSize(60, 60));
        btn0->setMaximumSize(QSize(60, 60));
        btn0->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn0, 4, 0, 1, 1);

        btn9 = new QPushButton(frame);
        btn9->setObjectName(QStringLiteral("btn9"));
        btn9->setMinimumSize(QSize(60, 60));
        btn9->setMaximumSize(QSize(60, 60));
        btn9->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn9, 3, 2, 1, 1);

        btnAccept = new QPushButton(frame);
        btnAccept->setObjectName(QStringLiteral("btnAccept"));
        btnAccept->setMinimumSize(QSize(60, 60));
        btnAccept->setMaximumSize(QSize(60, 60));
        btnAccept->setBaseSize(QSize(40, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAccept->setIcon(icon);
        btnAccept->setIconSize(QSize(32, 32));

        gridLayout->addWidget(btnAccept, 4, 1, 1, 1);

        btn6 = new QPushButton(frame);
        btn6->setObjectName(QStringLiteral("btn6"));
        btn6->setMinimumSize(QSize(60, 60));
        btn6->setMaximumSize(QSize(60, 60));
        btn6->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn6, 2, 2, 1, 1);

        btnReject = new QPushButton(frame);
        btnReject->setObjectName(QStringLiteral("btnReject"));
        btnReject->setMinimumSize(QSize(60, 60));
        btnReject->setMaximumSize(QSize(60, 60));
        btnReject->setBaseSize(QSize(40, 0));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnReject->setIcon(icon1);
        btnReject->setIconSize(QSize(32, 32));

        gridLayout->addWidget(btnReject, 4, 2, 1, 1);

        btn4 = new QPushButton(frame);
        btn4->setObjectName(QStringLiteral("btn4"));
        btn4->setMinimumSize(QSize(60, 60));
        btn4->setMaximumSize(QSize(60, 60));
        btn4->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn4, 2, 0, 1, 1);

        btn1 = new QPushButton(frame);
        btn1->setObjectName(QStringLiteral("btn1"));
        btn1->setMinimumSize(QSize(60, 60));
        btn1->setMaximumSize(QSize(60, 60));
        btn1->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn1, 1, 0, 1, 1);

        btn7 = new QPushButton(frame);
        btn7->setObjectName(QStringLiteral("btn7"));
        btn7->setMinimumSize(QSize(60, 60));
        btn7->setMaximumSize(QSize(60, 60));
        btn7->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn7, 3, 0, 1, 1);

        btn5 = new QPushButton(frame);
        btn5->setObjectName(QStringLiteral("btn5"));
        btn5->setMinimumSize(QSize(60, 60));
        btn5->setMaximumSize(QSize(60, 60));
        btn5->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(btn5, 2, 1, 1, 1);

        btnClear = new QPushButton(frame);
        btnClear->setObjectName(QStringLiteral("btnClear"));
        btnClear->setMinimumSize(QSize(60, 60));
        btnClear->setMaximumSize(QSize(555, 60));
        btnClear->setBaseSize(QSize(40, 0));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/backspace.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClear->setIcon(icon2);
        btnClear->setIconSize(QSize(32, 32));

        gridLayout->addWidget(btnClear, 0, 2, 1, 1);

        leText = new QLineEdit(frame);
        leText->setObjectName(QStringLiteral("leText"));
        QFont font;
        font.setPointSize(14);
        leText->setFont(font);

        gridLayout->addWidget(leText, 0, 0, 1, 2);


        verticalLayout->addLayout(gridLayout);


        horizontalLayout->addWidget(frame);


        retranslateUi(RNumbers);
        QObject::connect(btn0, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn1, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn2, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn3, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn4, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn5, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn6, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn7, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn8, SIGNAL(clicked()), RNumbers, SLOT(btnNumClick()));
        QObject::connect(btn9, SIGNAL(clicked(bool)), RNumbers, SLOT(btnNumClick()));

        QMetaObject::connectSlotsByName(RNumbers);
    } // setupUi

    void retranslateUi(QDialog *RNumbers)
    {
        RNumbers->setWindowTitle(QApplication::translate("RNumbers", "Dialog", nullptr));
        btn2->setText(QApplication::translate("RNumbers", "2", nullptr));
        btn8->setText(QApplication::translate("RNumbers", "8", nullptr));
        btn3->setText(QApplication::translate("RNumbers", "3", nullptr));
        btn0->setText(QApplication::translate("RNumbers", "0", nullptr));
        btn9->setText(QApplication::translate("RNumbers", "9", nullptr));
        btnAccept->setText(QString());
        btn6->setText(QApplication::translate("RNumbers", "6", nullptr));
        btnReject->setText(QString());
        btn4->setText(QApplication::translate("RNumbers", "4", nullptr));
        btn1->setText(QApplication::translate("RNumbers", "1", nullptr));
        btn7->setText(QApplication::translate("RNumbers", "7", nullptr));
        btn5->setText(QApplication::translate("RNumbers", "5", nullptr));
        btnClear->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RNumbers: public Ui_RNumbers {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RNUMBERS_H
