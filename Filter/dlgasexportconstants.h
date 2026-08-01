#ifndef DLGASEXPORTCONSTANTS_H
#define DLGASEXPORTCONSTANTS_H

#include <QDialog>
#include "asexportconstants.h"

namespace Ui {
class DlgAsExportConstants;
}

class DlgAsExportConstants : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAsExportConstants(QWidget *parent = nullptr);
    ~DlgAsExportConstants();

    void setValues(const AsExportConstants &values);
    void fillValues(AsExportConstants &values) const;

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgAsExportConstants *ui;
};

#endif // DLGASEXPORTCONSTANTS_H
