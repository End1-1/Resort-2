#ifndef RECARDEXGROUP_H
#define RECARDEXGROUP_H

#include "roweditordialog.h"

namespace Ui {
class RECardexGroup;
}

class RECardexGroup : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECardexGroup(QList<QVariant> &values, QWidget *parent = 0);
    ~RECardexGroup();
    static void openEditor();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RECardexGroup *ui;
};

#endif // RECARDEXGROUP_H
