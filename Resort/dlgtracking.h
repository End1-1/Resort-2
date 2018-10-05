#ifndef DLGTRACKING_H
#define DLGTRACKING_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgTracking;
}

class DlgTracking : public BaseExtendedDialog
{
    Q_OBJECT

public:    
    static void showTracking(int trackId, const QString &windowId);
private slots:
    void on_chOld_clicked(bool checked);

private:
    explicit DlgTracking(QWidget *parent = 0);
    ~DlgTracking();
    Ui::DlgTracking *ui;
    void loadTrack(const QString &query, int trackId, const QString &windowId);
    int fTrack;
    QString fWindow;

};

#endif // DLGTRACKING_H
