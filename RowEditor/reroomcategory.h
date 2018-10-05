#ifndef REROOMCATEGORY_H
#define REROOMCATEGORY_H

#include "roweditordialog.h"

namespace Ui {
class RERoomCategory;
}

class RERoomCategory : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERoomCategory(QList<QVariant> &values, QWidget *parent = 0);
    ~RERoomCategory();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::RERoomCategory *ui;
};

#endif // REROOMCATEGORY_H
