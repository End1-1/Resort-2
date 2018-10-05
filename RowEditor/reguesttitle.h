#ifndef REGUESTTITLE_H
#define REGUESTTITLE_H

#include "roweditordialog.h"

namespace Ui {
class REGuestTitle;
}

class REGuestTitle : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit REGuestTitle(QList<QVariant> &values, QWidget *parent = 0);
    ~REGuestTitle();
    static void openEditor();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::REGuestTitle *ui;
};

#endif // REGUESTTITLE_H
