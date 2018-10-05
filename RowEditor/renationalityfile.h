#ifndef RENATIONALITYFILE_H
#define RENATIONALITYFILE_H

#include "roweditordialog.h"

namespace Ui {
class RENationalityFile;
}

class RENationalityFile : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RENationalityFile(QList<QVariant> &values, QWidget *parent = 0);
    ~RENationalityFile();
    static void openEditor();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RENationalityFile *ui;
};

#endif // RENATIONALITYFILE_H
