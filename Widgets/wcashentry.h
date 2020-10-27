#ifndef WCASHENTRY_H
#define WCASHENTRY_H

#include "basewidget.h"
#include "dwselectorcashdesk.h"

#define CASHOP_NONE 0
#define CASHOP_IN 1
#define CASHOP_OUT 2
#define CASHOP_MOVE 3

namespace Ui {
class WCashEntry;
}

class WCashEntry : public BaseWidget
{
    Q_OBJECT

public:
    explicit WCashEntry(QWidget *parent = 0);
    ~WCashEntry();
    void save();
    void load(int doc);
    void enable();
    void disable();
    int debit();
    void debit(int d);
    int credit();
    void credit(int c);
    int cashOp();
    void cashOp(int c);
    QString comment();
    void comment(const QString &c);
    QDate date();
    void date(const QDate &date);
    double amount();
    void amount(double a);
    void docType(int t);
    void docNum(QString n);
    int partner();
    void partner(int p);
    bool isCorrent(QString &errors);
    void setSaveVisible(bool v);
    virtual void selector(int selectorNumber, const QVariant &value);

private slots:
    void on_btnCancel_clicked();
    void on_btnSave_clicked();
    void on_chEnable_stateChanged(int arg1);
    void on_chEnable_clicked(bool checked);
    void on_rbCashIn_clicked();
    void on_rbCashOut_clicked();
    void on_rbCashMove_clicked();

private:
    Ui::WCashEntry *ui;
    DWSelectorCashDesk *dockDebit;
    bool fIn;

signals:
    void rejected();
    void accepted();
};

#endif // WCASHENTRY_H
