#ifndef RERESTMENUPART_H
#define RERESTMENUPART_H

#include "roweditordialog.h"

namespace Ui {
class RERestMenuPart;
}

class RERestMenuPart : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestMenuPart(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestMenuPart();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::RERestMenuPart *ui;
};

#endif // RERESTMENUPART_H
