#ifndef DLGDATE_H
#define DLGDATE_H

#include <QDialog>

namespace Ui {
class DlgDate;
}

class DlgDate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDate(QWidget *parent = 0);
    ~DlgDate();
    static bool getDate(QDate &date);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgDate *ui;
};

#endif // DLGDATE_H
