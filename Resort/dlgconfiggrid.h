#ifndef DLGCONFIGGRID_H
#define DLGCONFIGGRID_H

#include "basedialog.h"

namespace Ui {
class DlgConfigGrid;
}

class GOWidget;

class DlgConfigGrid : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgConfigGrid(QWidget *parent = 0);
    ~DlgConfigGrid();
    static bool config(QString &fontName, int &fontSize, bool &fontBold, QWidget *parent);
private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgConfigGrid *ui;
    GOWidget *fAddWidget;
};

#endif // DLGCONFIGGRID_H
