#ifndef DLGONETWOALL_H
#define DLGONETWOALL_H

#include <QDialog>

namespace Ui {
class DlgOneTwoAll;
}

class DlgOneTwoAll : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOneTwoAll(QWidget *parent = nullptr);
    ~DlgOneTwoAll();
    static int getOption(const QString &title, const QString &all, const QString &one, const QString &two, QWidget *parent = nullptr);

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

    void on_rbAll_clicked();

    void on_rbOne_clicked();

    void on_rbTwo_clicked();

private:
    Ui::DlgOneTwoAll *ui;
    int fResult;
};

#endif // DLGONETWOALL_H