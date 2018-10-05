#ifndef RESTOREPARTNER_H
#define RESTOREPARTNER_H

#include "roweditordialog.h"

namespace Ui {
class REStorePartner;
}

class REStorePartner : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit REStorePartner(QList<QVariant> &values, QWidget *parent = 0);
    ~REStorePartner();
    static void openStorePartners();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::REStorePartner *ui;
};

#endif // RESTOREPARTNER_H
