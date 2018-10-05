/********************************************************************************
** Form generated from reading UI file 'splash.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPLASH_H
#define UI_SPLASH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Splash
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *lbMovie;

    void setupUi(QDialog *Splash)
    {
        if (Splash->objectName().isEmpty())
            Splash->setObjectName(QStringLiteral("Splash"));
        Splash->resize(436, 70);
        Splash->setMinimumSize(QSize(70, 70));
        Splash->setMaximumSize(QSize(1111, 70));
        verticalLayout = new QVBoxLayout(Splash);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(Splash);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbMovie = new QLabel(frame);
        lbMovie->setObjectName(QStringLiteral("lbMovie"));
        lbMovie->setScaledContents(false);
        lbMovie->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lbMovie);


        verticalLayout->addWidget(frame);


        retranslateUi(Splash);

        QMetaObject::connectSlotsByName(Splash);
    } // setupUi

    void retranslateUi(QDialog *Splash)
    {
        Splash->setWindowTitle(QApplication::translate("Splash", "Splash", nullptr));
        lbMovie->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Splash: public Ui_Splash {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASH_H
