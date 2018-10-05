#ifndef RECOMPLIMENTARYCOMMENT_H
#define RECOMPLIMENTARYCOMMENT_H

#include "roweditordialog.h"

namespace Ui {
class REComplimentaryComment;
}

class REComplimentaryComment : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit REComplimentaryComment(QList<QVariant> &values, QWidget *parent = 0);
    ~REComplimentaryComment();
    static void openComplimentaryComment();
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::REComplimentaryComment *ui;
};

#endif // RECOMPLIMENTARYCOMMENT_H
