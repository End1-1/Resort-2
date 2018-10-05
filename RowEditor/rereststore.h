#ifndef RERESTSTORE_H
#define RERESTSTORE_H

#include "roweditordialog.h"

namespace Ui {
class RERestStore;
}

class RERestStore : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestStore(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestStore();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::RERestStore *ui;
};

#endif // RERESTSTORE_H
