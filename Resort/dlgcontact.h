#ifndef DLGCONTACT_H
#define DLGCONTACT_H

#include "roweditordialog.h"

namespace Ui {
class DlgContact;
}

class DlgContact : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit DlgContact(QList<QVariant> &values, QWidget *parent = 0);
    ~DlgContact();
    virtual void setValues();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgContact *ui;
};

#endif // DLGCONTACT_H
