/********************************************************************************
** Form generated from reading UI file 'rlogin.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RLOGIN_H
#define UI_RLOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RLogin
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *lbWhy;
    QLineEdit *lePass;
    QGridLayout *gridLayout;
    QPushButton *btnCancel;
    QPushButton *btn1;
    QPushButton *btn3;
    QPushButton *btn2;
    QPushButton *btn6;
    QPushButton *btn0;
    QPushButton *btnClear;
    QPushButton *btn5;
    QPushButton *btn7;
    QPushButton *btn8;
    QPushButton *btnOk;
    QPushButton *btn9;
    QPushButton *btn4;

    void setupUi(QDialog *RLogin)
    {
        if (RLogin->objectName().isEmpty())
            RLogin->setObjectName(QStringLiteral("RLogin"));
        RLogin->resize(366, 284);
        horizontalLayout = new QHBoxLayout(RLogin);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(RLogin);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lbWhy = new QLabel(frame);
        lbWhy->setObjectName(QStringLiteral("lbWhy"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        lbWhy->setFont(font);
        lbWhy->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbWhy);

        lePass = new QLineEdit(frame);
        lePass->setObjectName(QStringLiteral("lePass"));
        lePass->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(lePass);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnCancel = new QPushButton(frame);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setMinimumSize(QSize(60, 60));
        btnCancel->setMaximumSize(QSize(2222, 60));
        btnCancel->setFocusPolicy(Qt::NoFocus);
        btnCancel->setIconSize(QSize(32, 32));

        gridLayout->addWidget(btnCancel, 2, 3, 1, 2);

        btn1 = new QPushButton(frame);
        btn1->setObjectName(QStringLiteral("btn1"));
        btn1->setMinimumSize(QSize(60, 60));
        btn1->setMaximumSize(QSize(60, 60));
        btn1->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn1, 0, 0, 1, 1);

        btn3 = new QPushButton(frame);
        btn3->setObjectName(QStringLiteral("btn3"));
        btn3->setMinimumSize(QSize(60, 60));
        btn3->setMaximumSize(QSize(60, 60));
        btn3->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn3, 0, 2, 1, 1);

        btn2 = new QPushButton(frame);
        btn2->setObjectName(QStringLiteral("btn2"));
        btn2->setMinimumSize(QSize(60, 60));
        btn2->setMaximumSize(QSize(60, 60));
        btn2->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn2, 0, 1, 1, 1);

        btn6 = new QPushButton(frame);
        btn6->setObjectName(QStringLiteral("btn6"));
        btn6->setMinimumSize(QSize(60, 60));
        btn6->setMaximumSize(QSize(60, 60));
        btn6->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn6, 1, 0, 1, 1);

        btn0 = new QPushButton(frame);
        btn0->setObjectName(QStringLiteral("btn0"));
        btn0->setMinimumSize(QSize(60, 60));
        btn0->setMaximumSize(QSize(60, 60));
        btn0->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn0, 1, 4, 1, 1);

        btnClear = new QPushButton(frame);
        btnClear->setObjectName(QStringLiteral("btnClear"));
        btnClear->setMinimumSize(QSize(60, 60));
        btnClear->setMaximumSize(QSize(60, 60));

        gridLayout->addWidget(btnClear, 2, 2, 1, 1);

        btn5 = new QPushButton(frame);
        btn5->setObjectName(QStringLiteral("btn5"));
        btn5->setMinimumSize(QSize(60, 60));
        btn5->setMaximumSize(QSize(60, 60));
        btn5->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn5, 0, 4, 1, 1);

        btn7 = new QPushButton(frame);
        btn7->setObjectName(QStringLiteral("btn7"));
        btn7->setMinimumSize(QSize(60, 60));
        btn7->setMaximumSize(QSize(60, 60));
        btn7->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn7, 1, 1, 1, 1);

        btn8 = new QPushButton(frame);
        btn8->setObjectName(QStringLiteral("btn8"));
        btn8->setMinimumSize(QSize(60, 60));
        btn8->setMaximumSize(QSize(60, 60));
        btn8->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn8, 1, 2, 1, 1);

        btnOk = new QPushButton(frame);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setMinimumSize(QSize(60, 60));
        btnOk->setMaximumSize(QSize(2222, 60));
        btnOk->setFocusPolicy(Qt::NoFocus);
        btnOk->setIconSize(QSize(32, 32));

        gridLayout->addWidget(btnOk, 2, 0, 1, 2);

        btn9 = new QPushButton(frame);
        btn9->setObjectName(QStringLiteral("btn9"));
        btn9->setMinimumSize(QSize(60, 60));
        btn9->setMaximumSize(QSize(60, 60));
        btn9->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn9, 1, 3, 1, 1);

        btn4 = new QPushButton(frame);
        btn4->setObjectName(QStringLiteral("btn4"));
        btn4->setMinimumSize(QSize(60, 60));
        btn4->setMaximumSize(QSize(60, 60));
        btn4->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(btn4, 0, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);


        horizontalLayout->addWidget(frame);


        retranslateUi(RLogin);
        QObject::connect(btn0, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn1, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn2, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn3, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn4, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn6, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn5, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn7, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn8, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));
        QObject::connect(btn9, SIGNAL(clicked()), RLogin, SLOT(numberButtonClicked()));

        QMetaObject::connectSlotsByName(RLogin);
    } // setupUi

    void retranslateUi(QDialog *RLogin)
    {
        RLogin->setWindowTitle(QApplication::translate("RLogin", "Login", nullptr));
        lbWhy->setText(QApplication::translate("RLogin", "Why?", nullptr));
        btnCancel->setText(QApplication::translate("RLogin", "Cancel", nullptr));
        btn1->setText(QApplication::translate("RLogin", "1", nullptr));
        btn3->setText(QApplication::translate("RLogin", "3", nullptr));
        btn2->setText(QApplication::translate("RLogin", "2", nullptr));
        btn6->setText(QApplication::translate("RLogin", "6", nullptr));
        btn0->setText(QApplication::translate("RLogin", "0", nullptr));
        btnClear->setText(QApplication::translate("RLogin", "Clear", nullptr));
        btn5->setText(QApplication::translate("RLogin", "5", nullptr));
        btn7->setText(QApplication::translate("RLogin", "7", nullptr));
        btn8->setText(QApplication::translate("RLogin", "8", nullptr));
        btnOk->setText(QApplication::translate("RLogin", "OK", nullptr));
        btn9->setText(QApplication::translate("RLogin", "9", nullptr));
        btn4->setText(QApplication::translate("RLogin", "4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RLogin: public Ui_RLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RLOGIN_H
