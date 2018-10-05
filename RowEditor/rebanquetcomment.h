#ifndef REBANQUETCOMMENT_H
#define REBANQUETCOMMENT_H

#include "roweditordialog.h"

namespace Ui {
class REBanquetComment;
}

class REBanquetComment : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit REBanquetComment(QList<QVariant> &values, QWidget *parent = 0);
    ~REBanquetComment();
    static void openBanquet();
private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::REBanquetComment *ui;
};

#endif // REBANQUETCOMMENT_H
