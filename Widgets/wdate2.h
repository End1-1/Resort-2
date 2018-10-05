#ifndef WDATE2_H
#define WDATE2_H

#include <QWidget>

namespace Ui {
class WDate2;
}

class WDate2 : public QWidget
{
    Q_OBJECT

public:
    explicit WDate2(QWidget *parent = 0);
    ~WDate2();
    QDate d1();
    QDate d2();
    QString ds1();
    QString ds2();
    QString dss1();
    QString dss2();
    QWidget *fw();
    void setD1(const QDate &d1);
    void setD2(const QDate &d2);
private:
    Ui::WDate2 *ui;
    void changeDate(int day);

signals:
    void changed();
private slots:
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
};

#endif // WDATE2_H
