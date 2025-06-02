#ifndef DLGPASSWORD_H
#define DLGPASSWORD_H

#include <QDialog>

namespace Ui
{
class DlgPassword;
}

class DlgPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPassword(QWidget *parent);

    ~DlgPassword();

    static bool getQty(const QString &title, double& qty, double& min);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_lePassword_returnPressed();

    void on_lePassword_textChanged(const QString &arg1);

    void on_btnClear_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::DlgPassword* ui;

    void click(const QString &text);

    double fMax;

    double fMin;

};

#endif // DLGPASSWORD_H
