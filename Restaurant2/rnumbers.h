#ifndef RNUMBERS_H
#define RNUMBERS_H

#include "baseextendeddialog.h"

namespace Ui {
class RNumbers;
}

class RNumbers : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RNumbers(QWidget *parent = 0);
    ~RNumbers();
    static bool getFloat(float &number, float max, const QString &title, QWidget *parent = nullptr);
    static bool getInt(int &num, const QString &title, QWidget *parent = nullptr);
    static bool getString(QString &str, const QString &title, QWidget *parent = nullptr);
    static bool getPassword(QString &str, const QString &title, QWidget *parent = nullptr);
private:
    Ui::RNumbers *ui;
    float fMax;
private slots:
    void btnNumClick();
    void on_btnClear_clicked();
    void on_btnReject_clicked();
    void on_btnAccept_clicked();
    void on_leText_returnPressed();
};

#endif // RNUMBERS_H
