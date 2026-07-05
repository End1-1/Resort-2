#ifndef DLGSESSIONOPEN_H
#define DLGSESSIONOPEN_H

#include "baseextendeddialog.h"

namespace Ui
{
class DlgSessionOpen;
}

class DlgSessionOpen : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgSessionOpen(int branch, QWidget *parent = nullptr,
                            const QString &printedBy = QString(), const QString &printer = QString());
    ~DlgSessionOpen();
    void prepareToShow();
    static bool ensureOpen(int branch, QWidget *parent = nullptr,
                           const QString &printedBy = QString(), const QString &printer = QString());

private slots:
    void on_btnOpen_clicked();
    void on_btnPrintTotal_clicked();
    void on_btnCancel_clicked();

private:
    Ui::DlgSessionOpen *ui;
    int fBranch;
    QString fPrintedBy;
    QString fPrinter;
};

#endif // DLGSESSIONOPEN_H
