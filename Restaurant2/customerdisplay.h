#ifndef CUSTOMERDISPLAY_H
#define CUSTOMERDISPLAY_H

#include <QString>
#include <QWidget>

struct CustomerDisplayLine {
    QString name;
    QString qty;
    QString total;
    QString comment;
};

struct CustomerDisplayOrder {
    QList<CustomerDisplayLine> lines;
    QString serviceAmount;
    QString grandTotal;
};

class QFrame;
class QLabel;
class QScrollArea;
class QVBoxLayout;
class QWidget;

class CustomerDisplay : public QWidget
{
    Q_OBJECT
public:
    static CustomerDisplay *instance();
    static void tryAttachSecondScreen();
    static void destroyInstance();

    void showWelcome();
    void showOrder(const CustomerDisplayOrder &order);
    void setBranchName(const QString &name);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    explicit CustomerDisplay(QWidget *parent = nullptr);
    void rebuildStyles();
    void updateLogo();
    void setIdleLayout(bool idle);
    void clearItems();
    void addItemRow(const CustomerDisplayLine &line);

    static CustomerDisplay *fInstance;

    QLabel *fLogoLabel = nullptr;
    QFrame *fOrderHeader = nullptr;
    QLabel *fHeaderLogo = nullptr;
    QLabel *fBranchLabel = nullptr;
    QWidget *fCenterColumn = nullptr;
    QFrame *fHeaderPanel = nullptr;
    QLabel *fBrandLabel = nullptr;
    QLabel *fContextLabel = nullptr;
    QLabel *fEmptyHint = nullptr;
    QScrollArea *fScroll = nullptr;
    QWidget *fItemsHost = nullptr;
    QVBoxLayout *fItemsLayout = nullptr;
    QFrame *fFooter = nullptr;
    QLabel *fServiceCaption = nullptr;
    QLabel *fServiceAmount = nullptr;
    QLabel *fTotalCaption = nullptr;
    QLabel *fTotalAmount = nullptr;
    QLabel *fTotalCurrency = nullptr;
};

#endif // CUSTOMERDISPLAY_H
