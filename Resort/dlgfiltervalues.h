#ifndef DLGFILTERVALUES_H
#define DLGFILTERVALUES_H

#include <QDialog>

namespace Ui {
class DlgFilterValues;
}

class DlgFilterValues : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFilterValues(QStringList &values, QWidget *parent = 0);
    ~DlgFilterValues();
private slots:
    void on_buttonBox_accepted();
private:
    Ui::DlgFilterValues *ui;
    QStringList &fValues;
};

#endif // DLGFILTERVALUES_H
