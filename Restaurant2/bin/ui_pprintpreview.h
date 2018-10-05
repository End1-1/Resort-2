/********************************************************************************
** Form generated from reading UI file 'pprintpreview.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PPRINTPREVIEW_H
#define UI_PPRINTPREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PPrintPreview
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnFirst;
    QPushButton *btnBack;
    QLabel *lbPage;
    QPushButton *btnNext;
    QPushButton *btnLast;
    QPushButton *btnZoomOut;
    QPushButton *btnZoopIn;
    QComboBox *cbZoom;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QComboBox *cbPrintSelection;
    QLineEdit *lePages;
    QComboBox *cbPrinters;
    QPushButton *btnPrint;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QScrollArea *saMain;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_3;
    QGraphicsView *gv;

    void setupUi(QDialog *PPrintPreview)
    {
        if (PPrintPreview->objectName().isEmpty())
            PPrintPreview->setObjectName(QStringLiteral("PPrintPreview"));
        PPrintPreview->resize(957, 700);
        verticalLayout_2 = new QVBoxLayout(PPrintPreview);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(PPrintPreview);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnFirst = new QPushButton(widget);
        btnFirst->setObjectName(QStringLiteral("btnFirst"));
        btnFirst->setMinimumSize(QSize(30, 30));
        btnFirst->setMaximumSize(QSize(30, 30));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/double-left.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnFirst->setIcon(icon);
        btnFirst->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnFirst);

        btnBack = new QPushButton(widget);
        btnBack->setObjectName(QStringLiteral("btnBack"));
        btnBack->setMinimumSize(QSize(30, 30));
        btnBack->setMaximumSize(QSize(30, 30));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/left-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBack->setIcon(icon1);
        btnBack->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnBack);

        lbPage = new QLabel(widget);
        lbPage->setObjectName(QStringLiteral("lbPage"));

        horizontalLayout->addWidget(lbPage);

        btnNext = new QPushButton(widget);
        btnNext->setObjectName(QStringLiteral("btnNext"));
        btnNext->setMinimumSize(QSize(30, 30));
        btnNext->setMaximumSize(QSize(30, 30));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/right-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnNext->setIcon(icon2);
        btnNext->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnNext);

        btnLast = new QPushButton(widget);
        btnLast->setObjectName(QStringLiteral("btnLast"));
        btnLast->setMinimumSize(QSize(30, 30));
        btnLast->setMaximumSize(QSize(30, 30));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/double-right.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnLast->setIcon(icon3);
        btnLast->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnLast);

        btnZoomOut = new QPushButton(widget);
        btnZoomOut->setObjectName(QStringLiteral("btnZoomOut"));
        btnZoomOut->setMinimumSize(QSize(30, 30));
        btnZoomOut->setMaximumSize(QSize(30, 30));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomOut->setIcon(icon4);
        btnZoomOut->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnZoomOut);

        btnZoopIn = new QPushButton(widget);
        btnZoopIn->setObjectName(QStringLiteral("btnZoopIn"));
        btnZoopIn->setMinimumSize(QSize(30, 30));
        btnZoopIn->setMaximumSize(QSize(30, 30));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoopIn->setIcon(icon5);
        btnZoopIn->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnZoopIn);

        cbZoom = new QComboBox(widget);
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->addItem(QString());
        cbZoom->setObjectName(QStringLiteral("cbZoom"));

        horizontalLayout->addWidget(cbZoom);

        horizontalSpacer = new QSpacerItem(417, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        cbPrintSelection = new QComboBox(widget);
        cbPrintSelection->addItem(QString());
        cbPrintSelection->addItem(QString());
        cbPrintSelection->addItem(QString());
        cbPrintSelection->setObjectName(QStringLiteral("cbPrintSelection"));

        horizontalLayout->addWidget(cbPrintSelection);

        lePages = new QLineEdit(widget);
        lePages->setObjectName(QStringLiteral("lePages"));
        lePages->setMinimumSize(QSize(100, 0));

        horizontalLayout->addWidget(lePages);

        cbPrinters = new QComboBox(widget);
        cbPrinters->setObjectName(QStringLiteral("cbPrinters"));
        cbPrinters->setMinimumSize(QSize(200, 0));

        horizontalLayout->addWidget(cbPrinters);

        btnPrint = new QPushButton(widget);
        btnPrint->setObjectName(QStringLiteral("btnPrint"));
        btnPrint->setMinimumSize(QSize(30, 30));
        btnPrint->setMaximumSize(QSize(30, 30));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrint->setIcon(icon6);
        btnPrint->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(btnPrint);


        verticalLayout_2->addWidget(widget);

        widget_2 = new QWidget(PPrintPreview);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 2);
        saMain = new QScrollArea(widget_2);
        saMain->setObjectName(QStringLiteral("saMain"));
        saMain->setFrameShape(QFrame::Box);
        saMain->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 953, 658));
        horizontalLayout_3 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(25, 25, 9, 0);
        gv = new QGraphicsView(scrollAreaWidgetContents);
        gv->setObjectName(QStringLiteral("gv"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(gv->sizePolicy().hasHeightForWidth());
        gv->setSizePolicy(sizePolicy2);
        gv->setFrameShape(QFrame::NoFrame);
        gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gv->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gv->setTransformationAnchor(QGraphicsView::NoAnchor);
        gv->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        horizontalLayout_3->addWidget(gv);

        saMain->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(saMain);


        verticalLayout_2->addWidget(widget_2);


        retranslateUi(PPrintPreview);

        cbZoom->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(PPrintPreview);
    } // setupUi

    void retranslateUi(QDialog *PPrintPreview)
    {
        PPrintPreview->setWindowTitle(QApplication::translate("PPrintPreview", "Print preview", nullptr));
        btnFirst->setText(QString());
        btnBack->setText(QString());
        lbPage->setText(QApplication::translate("PPrintPreview", "Page 1 of 1", nullptr));
        btnNext->setText(QString());
        btnLast->setText(QString());
        btnZoomOut->setText(QString());
        btnZoopIn->setText(QString());
        cbZoom->setItemText(0, QApplication::translate("PPrintPreview", "200%", nullptr));
        cbZoom->setItemText(1, QApplication::translate("PPrintPreview", "175%", nullptr));
        cbZoom->setItemText(2, QApplication::translate("PPrintPreview", "150%", nullptr));
        cbZoom->setItemText(3, QApplication::translate("PPrintPreview", "125%", nullptr));
        cbZoom->setItemText(4, QApplication::translate("PPrintPreview", "100%", nullptr));
        cbZoom->setItemText(5, QApplication::translate("PPrintPreview", "75%", nullptr));
        cbZoom->setItemText(6, QApplication::translate("PPrintPreview", "50%", nullptr));
        cbZoom->setItemText(7, QApplication::translate("PPrintPreview", "25%", nullptr));

        label->setText(QApplication::translate("PPrintPreview", "Print", nullptr));
        cbPrintSelection->setItemText(0, QApplication::translate("PPrintPreview", "All pages", nullptr));
        cbPrintSelection->setItemText(1, QApplication::translate("PPrintPreview", "Current page", nullptr));
        cbPrintSelection->setItemText(2, QApplication::translate("PPrintPreview", "Custom", nullptr));

        btnPrint->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PPrintPreview: public Ui_PPrintPreview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PPRINTPREVIEW_H
