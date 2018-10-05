#ifndef RERESTPRINTER_H
#define RERESTPRINTER_H

#include "roweditordialog.h"

namespace Ui {
class RERestPrinter;
}

class RERestPrinter : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestPrinter(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestPrinter();
private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
private:
    Ui::RERestPrinter *ui;
};

#endif // RERESTPRINTER_H
