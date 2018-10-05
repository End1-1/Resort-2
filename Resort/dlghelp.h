#ifndef DLGHELP_H
#define DLGHELP_H

#include "basedialog.h"

namespace Ui {
class DlgHelp;
}

class DlgHelp : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgHelp(const QString &helpFile, QWidget *parent = 0);
    ~DlgHelp();
    static void openHelp(const QString &helpFile);
private:
    Ui::DlgHelp *ui;
};

#endif // DLGHELP_H
