#ifndef DLGREPORTBUILDERGROUPS_H
#define DLGREPORTBUILDERGROUPS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgReportBuilderGroups;
}

class DlgReportBuilderGroups : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReportBuilderGroups(QWidget *parent = 0);
    ~DlgReportBuilderGroups();

private slots:
    void on_btnClose_clicked();

    void on_btnNew_clicked();

    void on_btnEdit_clicked();

    void on_btnRemove_clicked();

private:
    Ui::DlgReportBuilderGroups *ui;
};

#endif // DLGREPORTBUILDERGROUPS_H
