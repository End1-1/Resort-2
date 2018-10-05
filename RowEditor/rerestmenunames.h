#ifndef RERESTMENUNAMES_H
#define RERESTMENUNAMES_H

#include "roweditordialog.h"

namespace Ui {
class RERestMenuNames;
}

class RERestMenuNames : public RowEditorDialog
{
    Q_OBJECT
public:
    explicit RERestMenuNames(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestMenuNames();
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
private:
    Ui::RERestMenuNames *ui;
};

#endif // RERESTMENUNAMES_H
