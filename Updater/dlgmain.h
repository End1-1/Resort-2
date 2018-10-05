#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QDialog>

namespace Ui {
class DlgMain;
}

class DlgMain : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMain(QWidget *parent = 0);
    ~DlgMain();

public slots:
    void on_btnUpdate_clicked();

private:
    Ui::DlgMain *ui;
    void start();
    void stop();
    bool fStarted;
};

extern bool autoStart;

#endif // DLGMAIN_H
